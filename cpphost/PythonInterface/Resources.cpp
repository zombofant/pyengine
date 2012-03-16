/**********************************************************************
File name: Resources.cpp
This file is part of: Pythonic Universe

LICENSE

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations under
the License.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public license (the  "GPL License"), in which case  the
provisions of GPL License are applicable instead of those above.

FEEDBACK & QUESTIONS

For feedback and questions about pyuni please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/
#include "Resources.hpp"

#include <iostream>
#include <boost/shared_ptr.hpp>

namespace PyUni {

using namespace boost::python;
using namespace std;
using namespace PyUni::Resources;

class PyStreamBuf: public basic_streambuf<char, std::char_traits<char> >
{
    private:
        static const Py_ssize_t _bufferSize = 1024;
        char *_rbuffer, *_wbuffer;
        /*char *_eback, *_gptr, *_egptr;
        char *_pbase, *_pptr, *_epptr;*/
    protected:
        PyObject *_readCall, *_writeCall, *_flushCall;
        PyObject *_pyBufferSize;
        typedef std::char_traits<char> _Tr;
    public:
        PyStreamBuf(PyObject *readCall, PyObject *writeCall, PyObject *flushCall):
            _rbuffer((PyCallable_Check(readCall)?(char*)malloc(_bufferSize+1):0)),
            _wbuffer((PyCallable_Check(writeCall)?(char*)malloc(_bufferSize+1):0)),
            /*_eback(_rbuffer),
            _gptr(_rbuffer),
            _egptr(_rbuffer),
            _pbase(_wbuffer),
            _pptr(_wbuffer),
            _epptr(_wbuffer),*/
            _readCall(readCall),
            _writeCall(writeCall),
            _flushCall(flushCall),
            _pyBufferSize(Py_BuildValue("(i)", _bufferSize))
        {
            Py_XINCREF(_readCall);
            Py_XINCREF(_writeCall);
            Py_XINCREF(_flushCall);
            assert(PyTuple_Check(_pyBufferSize));
            setg(_rbuffer, _rbuffer, _rbuffer);
            setp(_wbuffer, _wbuffer);
        }

        virtual ~PyStreamBuf()
        {
            Py_XDECREF(_readCall);
            Py_XDECREF(_writeCall);
            Py_XDECREF(_flushCall);
            Py_DECREF(_pyBufferSize);
            free(_rbuffer);
            free(_wbuffer);
        }
    public:
        virtual int underflow()
        {
            cerr << "underflow called" << endl;
            if (!_rbuffer)
                return _Tr::eof();
            PyObject *buf = PyObject_CallObject(_readCall, _pyBufferSize);
            assert(PyString_Check(buf));
            Py_ssize_t len = PyString_GET_SIZE(buf);
            cerr << "len = " << len << endl;
            if (len > 0)
            {
                memcpy(_rbuffer, PyString_AS_STRING(buf), len+1);
                cerr << _rbuffer << endl;
                /*_eback = _rbuffer;
                _gptr = _rbuffer;
                _egptr = _rbuffer + len;*/
                setg(_rbuffer, _rbuffer, _rbuffer + len);
                Py_DECREF(buf);
                return *_rbuffer;
            }
            else
            {
                Py_DECREF(buf);
                return _Tr::eof();
            }
        }
        
        virtual int overflow(int c = _Tr::eof())
        {
            if (!_wbuffer)
                return _Tr::eof();
            cerr << "FIXME: do not drop c" << endl;
            sync();
            return c;
        }

        virtual int sync()
        {
            if (!_wbuffer)
                return 0;
            Py_ssize_t len = ((Py_ssize_t)(pptr())) - ((Py_ssize_t)(pbase()));
            PyObject *buffer = PyString_FromStringAndSize(_wbuffer, len);
            Py_DECREF(PyObject_CallFunctionObjArgs(_writeCall, buffer, 0));
            Py_DECREF(buffer);
            setp(_wbuffer, _wbuffer + len);

            if (_flushCall)
                Py_DECREF(PyObject_CallObject(_flushCall, 0));
            return 0;
        }
};

class PyIStream: private PyStreamBuf, public istream
{
    public:
        PyIStream(PyObject *readCall):
            PyStreamBuf(readCall, 0, 0),
            istream(this)
        {
            
        }

        /*PyIStream(const PyIStream &ref):
            PyStreamBuf(ref._readCall, ref._writeCall, ref._flushCall),
            istream(this)
        {
            
        }*/
};

typedef boost::shared_ptr<PyIStream> PyIStreamHandle;

PyIStreamHandle CIStream(object fileLike)
{
    PyObject *obj = fileLike.ptr();
    PyObject *readCall = PyObject_GetAttrString(obj, "read");
    if (!readCall || !PyCallable_Check(readCall))
        return PyIStreamHandle();
    return PyIStreamHandle(new PyIStream(readCall));
}

/*struct IStream_from_python_filelike
{
    static void* convertible(PyObject *obj)
    {
        cout << "convertible called" << endl;
        PyObject *tmp = PyObject_GetAttrString(obj, "read");
        if (!tmp)
            return 0;
        if (!PyCallable_Check(tmp)) {
            Py_DECREF(tmp);
            return 0;
        }
        Py_DECREF(tmp);
        return obj;
    }

    static void construct(PyObject *obj, converter::rvalue_from_python_stage1_data *data)
    {
        PyObject *readCall = PyObject_GetAttrString(obj, "read");
        assert(readCall);

        void *storage = ((converter::rvalue_from_python_storage<PyIStream>*)data)->storage.bytes;

        new (storage) PyIStream(readCall);
        Py_DECREF(readCall);

        data->convertible = storage;
    }

    static PyIStream *extract(PyObject *obj)
    {
        PyObject *readCall = PyObject_GetAttrString(obj, "read");
        assert(readCall);
        return new PyIStream(readCall);        
    }
};*/

BOOST_PYTHON_MODULE(_cuni_resources)
{
    class_<PyIStream, PyIStreamHandle, boost::noncopyable>("CIStream", no_init)
        .def("__init__", make_constructor(&CIStream))
    ;
    implicitly_convertible<PyIStreamHandle, boost::shared_ptr<istream> >();
    
    class_<Image, ImageHandle, boost::noncopyable>("Image", no_init)
        .def("dropData", &Image::dropData)
        .def("texImage2D", &Image::texImage2D)
        .def("texSubImage2D", &Image::texSubImage2D)
        .add_property("IsValid", &Image::getIsValid)
        .def_readonly("Width", &Image::width)
        .def_readonly("Height", &Image::height)
        .def_readonly("Format", &Image::format)
        .def_readonly("Type", &Image::type)
    ;

    def("PNGImage", &Image::PNGImage);
}

void addResourcesToInittab()
{
    PyImport_AppendInittab("_cuni_resources", &init_cuni_resources);
}

}
