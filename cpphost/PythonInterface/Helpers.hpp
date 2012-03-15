#ifndef _PYUNI_PYTHON_HELPERS_H
#define _PYUNI_PYTHON_HELPERS_H

#include <boost/python.hpp>
// #include <iostream>

namespace PyUni {

using namespace boost::python;

template <class KeyT>
void KeyError(const KeyT &key)
{
    PyErr_SetString(PyExc_KeyError, "Key not found");
    throw_error_already_set();
}

void ValueError(const std::string &message)
{
    PyErr_SetString(PyExc_ValueError, message.c_str());
    throw_error_already_set();
}

template<class Container, class Iterator>
class AbstractIteratorWrapper
{
    public:
        typedef Container ContainerT;
        typedef Iterator IteratorT;
    public:
        AbstractIteratorWrapper(IteratorT it, IteratorT end):
            _it(it),
            _end(end)
        {

        }
    protected:
        IteratorT _it;
        IteratorT _end;
    protected:
        void StopIteration()
        {
            PyErr_SetString(PyExc_StopIteration, "No more data.");
            throw_error_already_set();
        }
};

template<class Container, class Iterator, class ResultT>
class ListIteratorWrapper: public AbstractIteratorWrapper<Container, Iterator>
{
    public:
        ListIteratorWrapper(Iterator *it, Iterator *end):
            AbstractIteratorWrapper<Container, Iterator>::AbstractIteratorWrapper(it, end)
        {
    
        }
    public:
        ResultT next()
        {
            if (this->_it == this->_end)
                this->StopIteration();
            ResultT result = *(this->_it);
            this->_it++;
            return result;
        }
};

template<class Container, class Iterator, class ResultT>
class MapKeyIterator: public AbstractIteratorWrapper<Container, Iterator>
{
    public:
        MapKeyIterator(Iterator it, Iterator end):
            AbstractIteratorWrapper<Container, Iterator>::AbstractIteratorWrapper(it, end)
        {
    
        }
    public:
        ResultT next()
        {
            if (this->_it == this->_end)
                this->StopIteration();
            ResultT result = this->_it->first;
            this->_it++;
            return result;
        }
};

template<class Container, class Iterator, class ResultT>
class MapValueIterator: public AbstractIteratorWrapper<Container, Iterator>
{
    public:
        MapValueIterator(Iterator it, Iterator end):
            AbstractIteratorWrapper<Container, Iterator>::AbstractIteratorWrapper(it, end)
        {
    
        }
    public:
        ResultT next()
        {
            if (this->_it == this->_end)
                this->StopIteration();
            ResultT result = this->_it->second;
            this->_it++;
            return result;
        }
};

template<class Container, class Iterator>
class MapItemsIterator: public AbstractIteratorWrapper<Container, Iterator>
{
    public:
        MapItemsIterator(Iterator it, Iterator end):
            AbstractIteratorWrapper<Container, Iterator>::AbstractIteratorWrapper(it, end)
        {
    
        }
    public:
        tuple next()
        {
            if (this->_it == this->_end)
                this->StopIteration();
            tuple result = make_tuple(object(this->_it->first), object(this->_it->second));
            this->_it++;
            return result;
        }
};

template<class IteratorWrapperT>
struct IteratorRegister
{
    typedef typename IteratorWrapperT::IteratorT IteratorT;
    typedef typename IteratorWrapperT::ContainerT ContainerT;
    
    inline static object passThrough(const object &o)
    {
        return o;
    }
    
    static void wrap(const char* pythonName)
    {
        class_<IteratorWrapperT>(pythonName, no_init)
            .def("__iter__", &passThrough)
            .def("next", &IteratorWrapperT::next)
        ;
    }

    static IteratorWrapperT *__iter__(const ContainerT &container)
    {
        return new IteratorWrapperT(container.begin(), container.end());
    }

    static IteratorWrapperT *__iter__c(const ContainerT &container)
    {
        return new IteratorWrapperT(container.cbegin(), container.cend());
    }
};

template<class MapT, class MapWrapperT>
struct MapHelper
{
    typedef typename MapT::key_type KeyT;
    typedef typename MapT::mapped_type ValueT;
    typedef typename MapT::const_iterator IteratorT;
    typedef MapItemsIterator<MapT, IteratorT> ItemsIteratorWrapperT;
    typedef MapKeyIterator<MapT, IteratorT, KeyT> KeysIteratorWrapperT;
    typedef MapValueIterator<MapT, IteratorT, ValueT> ValuesIteratorWrapperT;
    typedef IteratorRegister<ItemsIteratorWrapperT> ItemsIteratorRegT;
    typedef IteratorRegister<KeysIteratorWrapperT> KeysIteratorRegT;
    typedef IteratorRegister<ValuesIteratorWrapperT> ValuesIteratorRegT;

    static bool __contains__(const MapT &map, const KeyT &key)
    {
        return (map.find(key) != map.end());
    }

    static ValueT __getitem__(MapT &map, const KeyT &key)
    {
        if (__contains__(map, key))
        {
            return ValueT(map[key]);
        }
        KeyError(*key); // throws inconditionally
        return ValueT();
    }

    static void __setitem__(MapT &map, KeyT &key, ValueT value)
    {
        map[key] = value;
    }

    static void __delitem__(MapT &map, const KeyT &key)
    {
        if (__contains__(map, key))
            map.erase(key);
        else
            KeyError(key);
    }

    static ItemsIteratorWrapperT *iteritems(const MapT &map)
    {
        return ItemsIteratorRegT::__iter__c(map);
    }

    static KeysIteratorWrapperT *iterkeys(const MapT &map)
    {
        return KeysIteratorRegT::__iter__c(map);
    }

    static ValuesIteratorWrapperT *itervalues(const MapT &map)
    {
        return ValuesIteratorRegT::__iter__c(map);
    }
};

}

#endif
