/**********************************************************************
File name: GL.cpp
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
#include "GL.hpp"

#include <glew.h>
#include <iostream>

#include <boost/python/slice.hpp>

namespace PyUni {

using namespace boost::python;
using namespace PyUni::GL;

void parseSlice(slice *slice, GLsizei *start, GLsizei *stop, GLsizei *step, const GLsizei len)
{
    object  pyStart = slice->start(),
            pyStop = slice->stop(),
            pyStep = slice->step();
    
    if (!pyStart.is_none())
    {
        *start = extract<GLsizei>(pyStart)();
        if (*start < 0)
            *start = len - *start;
    }
    
    if (!pyStop.is_none())
    {
        *stop = extract<GLsizei>(pyStop)();
        if (*stop < 0)
            *stop = len - *stop;
    }
    
    if (!pyStep.is_none())
    {
        if (!step)
        {
            std::cerr << "slice step not allowed here" << std::endl;
            throw std::exception();
        }
        else
        {
            *step = extract<GLsizei>(pyStep)();
        }
    }
}

GeometryBufferView::AttributeSlice *AttributeView_slice(GeometryBufferView::AttributeView &view, object arg)
{
    GLsizei len = view.getLength();
    GLsizei start = 0, stop = len, step = 1;
    GLsizei attribOffset = 0, attribLength = view.getAttributeLength();
    
    extract<int> getInt(arg);
    if (getInt.check())
    {
        start = getInt();
        if (start < 0)
            start = len - start;
        stop = start + 1;
    }
    else
    {
        extract<slice> getSlice(arg);
        if (getSlice.check())
        {
            slice tmp = getSlice();
            parseSlice(&tmp, &start, &stop, &step, len);
        }
        else
        {
            tuple pyTuple = extract<tuple>(arg)();
            if (boost::python::len(pyTuple) != 2)
            {
                std::cerr << "too many arguments" << std::endl;
                throw std::exception();
            }
            slice tmp = extract<slice>(pyTuple[0])();
            parseSlice(&tmp, &start, &stop, &step, len);

            extract<slice> getAttribSlice(pyTuple[1]);
            if (getAttribSlice.check())
            {
                slice tmp = getAttribSlice();
                parseSlice(&tmp, &attribOffset, &attribLength, 0, view.getAttributeLength());
            }
            else
            {
                attribOffset = extract<int>(pyTuple[1])();
                attribLength = 1;
            }
        }
    }
    return view.slice(start, stop, step, attribOffset, attribLength);
}

template <class SliceT>
PyObject *AttributeSlice_get(SliceT *slice)
{
    const GLsizei len = slice->getLength() * slice->getAttributeLength();
    GLVertexFloat *buffer = (GLVertexFloat*)malloc(slice->getSize());
    GLVertexFloat *ptr = buffer;
    PyObject *pyList = PyList_New(len);
    slice->get(buffer);
    for (GLsizei i = 0; i < len; i++)
    {
        PyList_SET_ITEM(pyList, i, PyFloat_FromDouble(*ptr));
        ptr++;
    }
    free(buffer);
    return pyList;
}

template <class SliceT>
void AttributeSlice_set(SliceT *slice, list bpList)
{
    const GLsizei len = slice->getLength() * slice->getAttributeLength();
    PyObject *pyList = bpList.ptr();
    if (PyList_Size(pyList) != len)
    {
        std::cerr << "need exactly " << len << " items, got " << PyList_Size(pyList) << "." << std::endl;
        throw std::exception();
    }
    GLVertexFloat *buffer = (GLVertexFloat*)malloc(slice->getSize());
    GLVertexFloat *ptr = buffer;
    for (GLsizei i = 0; i < len; i++)
    {
        const PyObject *val = PyList_GET_ITEM(pyList, i);
        if (!PyFloat_Check(val))
        {
            free(buffer);
            throw std::exception();
        }
        *ptr = PyFloat_AS_DOUBLE(val);
        // std::cout << *ptr << std::endl;
        ptr++;
    }
    slice->set(buffer);
    free(buffer);
}

BOOST_PYTHON_MODULE(_cuni_gl)
{
    /* Base.hpp */
    
    class_<StructWrap, boost::shared_ptr<StructWrap>, boost::noncopyable>("Struct")
        .def("bind", pure_virtual(&Struct::bind))
        .def("unbind", pure_virtual(&Struct::unbind))
    ;
    implicitly_convertible<boost::shared_ptr<StructWrap>, StructHandle>();

    class_<ClassWrap, bases<Struct>, boost::shared_ptr<ClassWrap>, boost::noncopyable>("Class", init<>())
        .def_readwrite("id", &Class::glID)
    ;
    implicitly_convertible<boost::shared_ptr<ClassWrap>, ClassHandle>();


    /* GenericBuffer.hpp */

    class_<GenericBuffer, bases<Class>, boost::shared_ptr<GenericBuffer>, boost::noncopyable>("GenericBuffer", no_init)
        .def("bind", &GenericBuffer::bind)
        .def("unbind", &GenericBuffer::unbind)
        .def("flush", &GenericBuffer::flush)
        .def("readBack", &GenericBuffer::readBack)
    ;
    

    /* GeometryBuffer.hpp */
    
    class_<VertexFormat, VertexFormatHandle, boost::noncopyable>("VertexFormat",
        init<const unsigned int, const unsigned int, const unsigned int,
            const unsigned int, const unsigned int, const unsigned int,
            const bool, const unsigned int, const unsigned int,
            const unsigned int, const unsigned int>())
    ;

    class_<VertexIndexList, VertexIndexListHandle, boost::noncopyable>("VertexIndexList", no_init);
    
    class_<GeometryBuffer, bases<GenericBuffer>, boost::noncopyable>("GeometryBuffer",
            init<const VertexFormatHandle, GLenum>())
        .def("allocateVertices", &GeometryBuffer::allocateVertices)
        .def("bind", &GeometryBuffer::bind)
        .def("draw", &GeometryBuffer::draw)
        .def("unbind", &GeometryBuffer::unbind)
        .def("gc", &GeometryBuffer::gc)
    ;


    /* GeomertyBufferView.hpp */
    
    class_<GeometryBufferView::AttributeView, boost::noncopyable>("AttributeView", no_init)
        .def("__getitem__", &AttributeView_slice, return_value_policy<reference_existing_object>())
        .def("__len__", &GeometryBufferView::AttributeView::getLength)
        .add_property("AttributeLength", &GeometryBufferView::AttributeView::getAttributeLength)
        .add_property("Size", &GeometryBufferView::AttributeView::getSize)
        .def("get", &AttributeSlice_get<GeometryBufferView::AttributeView>)
        .def("set", &AttributeSlice_set<GeometryBufferView::AttributeView>)
    ;

    class_<GeometryBufferView::AttributeSlice, boost::noncopyable>("AttributeSlice", no_init)
        .def("__len__", &GeometryBufferView::AttributeSlice::getLength)
        .add_property("Size", &GeometryBufferView::AttributeSlice::getSize)
        .def("get", &AttributeSlice_get<GeometryBufferView::AttributeSlice>)
        .def("set", &AttributeSlice_set<GeometryBufferView::AttributeSlice>)
    ;

    class_<GeometryBufferView, GeometryBufferViewHandle, boost::noncopyable>("GeometryBufferView",
            init<   const GeometryBufferHandle,
                    const VertexIndexListHandle>())
        .add_property("Vertex",
            make_function(&GeometryBufferView::getPositionView,
                return_value_policy<reference_existing_object>())
        )
        .add_property("Colour",
            make_function(&GeometryBufferView::getColourView,
                return_value_policy<reference_existing_object>())
        )
        .def("TexCoord", &GeometryBufferView::getTexCoordView,
                return_value_policy<reference_existing_object>())
        .add_property("Normal",
            make_function(&GeometryBufferView::getNormalView,
                return_value_policy<reference_existing_object>())
        )
        .def("Attrib", &GeometryBufferView::getPositionView,
                return_value_policy<reference_existing_object>())
        .def("__len__", &GeometryBufferView::getLength)
    ;


    /* IndexBuffer.hpp */

    class_<IndexEntry, IndexEntryHandle, boost::noncopyable>("IndexEntry", no_init)
        .def_readonly("Vertices", &IndexEntry::vertices)
    ;

    class_<GenericIndexBuffer, bases<GenericBuffer>, GenericIndexBufferHandle, boost::noncopyable>("IndexBuffer", no_init)
        .def("__len__", &GenericIndexBuffer::getCount)
        .def("clear", &GenericIndexBuffer::clear)
        .def("draw", &GenericIndexBuffer::draw)
        .def("drawUnbound", &GenericIndexBuffer::drawUnbound)
        .def("dump", &GenericIndexBuffer::dump)
    ;

    class_<StreamIndexBuffer, bases<GenericIndexBuffer>, StreamIndexBufferHandle, boost::noncopyable>("StreamIndexBuffer", init<const GLenum>())
        // .def("__init__", init<>())
        .def("add", &StreamIndexBuffer::add)
    ;

    class_<StaticIndexBuffer, bases<GenericIndexBuffer>, StaticIndexBufferHandle, boost::noncopyable>("StaticIndexBuffer", init<const GLenum>())
        // .def("__init__", init<>())
        .def("getIndex", &StaticIndexBuffer::getIndex)
        .def("add", &StaticIndexBuffer::add)
        .def("clear", &StaticIndexBuffer::clear)
        .def("drawHandle", &StaticIndexBuffer::drawHandle)
        .def("gc", &StaticIndexBuffer::gc)
        .def("remove", &StaticIndexBuffer::remove)
        .def("resolveIndexEntry", &StaticIndexBuffer::resolveIndexEntry)
    ;


    /* StateManagement.hpp */
    
    class_<GroupWrap, boost::shared_ptr<GroupWrap>, boost::noncopyable>("Group", init<int>())
        // .def("__init__", init<>())
        .def("execute", &GroupWrap::__bp_execute)
        .add_property("IndexBuffer", &Group::getIndexBuffer)
        .def("__cmp__", &Group::compare)
    ;

    class_<ParentGroupWrap, bases<Group>, boost::shared_ptr<ParentGroupWrap>, boost::noncopyable>("ParentGroup", init<int>())
        // .def("__init__", init<>())
        .def("execute", &ParentGroupWrap::__bp_execute)
        .def("setUp", &ParentGroupWrap::__bp_setUp)
        .def("tearDown", &ParentGroupWrap::__bp_tearDown)
        .def("add", &ParentGroup::add)
        .def("remove", &ParentGroup::remove)
    ;
    
    class_<StateGroup, bases<ParentGroup>, StateGroupHandle, boost::noncopyable>("StateGroup", init<StructHandle, int>())
        // .def("__init__", init<StructHandle>())
        .def("setUp", &StateGroup::setUp)
        .def("tearDown", &StateGroup::tearDown)
    ;

    class_<TransformGroup, bases<ParentGroup>, TransformGroupHandle, boost::noncopyable>("TransformGroup", no_init)
        .def("setUp", &TransformGroup::setUp)
        .def("tearDown", &TransformGroup::tearDown)
    ;
}

void addGLToInittab()
{
    PyImport_AppendInittab("_cuni_gl", &init_cuni_gl);
}


}
