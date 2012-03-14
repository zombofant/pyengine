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

#include <GL/glew.h>
#include <iostream>

#include <boost/python/slice.hpp>

namespace PyUni {

using namespace boost::python;

GL::GeometryBufferView::AttributeSlice *AttributeView_slice(GL::GeometryBufferView::AttributeView &view, object arg)
{
    extract<slice> getSlice(arg);
    // for now, we do not check, as we do only support slices
    slice val = getSlice();
    object  pyStart = val.start(),
            pyStop = val.stop(),
            pyStep = val.step();

    GLsizei len = view.getLength();
    GLsizei start = 0, stop = len, step = 1;
    
    if (!pyStart.is_none())
    {
        start = extract<GLsizei>(pyStart)();
        if (start < 0)
            start = len - start;
    }
    
    if (!pyStop.is_none())
    {
        stop = extract<GLsizei>(pyStop)();
        if (stop < 0)
            stop = len - stop;
    }
    
    if (!pyStep.is_none())
    {
        step = extract<GLsizei>(pyStep)();
    }
    
    return view.slice(start, stop, step);
}

PyObject *AttributeSlice_get(GL::GeometryBufferView::AttributeSlice *slice)
{
    const GLsizei len = slice->getLength() * slice->getAttributeLength();
    GL::GLVertexFloat *buffer = (GL::GLVertexFloat*)malloc(slice->getSize());
    GL::GLVertexFloat *ptr = buffer;
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

PyObject *AttributeSlice_set(GL::GeometryBufferView::AttributeSlice *slice, list bpList)
{
    const GLsizei len = slice->getLength() * slice->getAttributeLength();
    PyObject *pyList = bpList.ptr();
    if (PyList_Size(pyList) != len)
    {
        throw std::exception();
    }
    GL::GLVertexFloat *buffer = (GL::GLVertexFloat*)malloc(slice->getSize());
    GL::GLVertexFloat *ptr = buffer;
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
    return pyList;
}

BOOST_PYTHON_MODULE(_cuni_gl)
{
    class_<GL::VertexFormat, GL::VertexFormatHandle, boost::noncopyable>("VertexFormat",
        init<const unsigned int, const unsigned int, const unsigned int,
            const unsigned int, const unsigned int, const unsigned int,
            const bool, const unsigned int, const unsigned int,
            const unsigned int, const unsigned int>())
    ;

    class_<StructWrap, boost::shared_ptr<StructWrap>, boost::noncopyable>("Struct")
        .def("bind", pure_virtual(&GL::Struct::bind))
        .def("unbind", pure_virtual(&GL::Struct::unbind))
    ;
    implicitly_convertible<boost::shared_ptr<StructWrap>, GL::StructHandle>();

    class_<ClassWrap, bases<GL::Struct>, boost::shared_ptr<ClassWrap>, boost::noncopyable>("Class", init<>())
        .add_property("ID", &GL::Class::getID)
    ;
    implicitly_convertible<boost::shared_ptr<ClassWrap>, GL::ClassHandle>();

    class_<GL::VertexIndexList, GL::VertexIndexListHandle, boost::noncopyable>("VertexIndexList", no_init);

    class_<GL::GenericGeometryBuffer, bases<GL::Class>, boost::noncopyable>("GeometryBuffer",
            init<const GL::VertexFormatHandle, GLenum>())
        .def("allocateVertices", &GL::GenericGeometryBuffer::allocateVertices)
    ;

    class_<GL::GeometryBufferView::AttributeView, boost::noncopyable>("AttributeView", no_init)
        .def("__getitem__", &AttributeView_slice, return_value_policy<reference_existing_object>())
        .def("getLength", &GL::GeometryBufferView::AttributeView::getLength)
        .def("getSize", &GL::GeometryBufferView::AttributeView::getSize)
    ;

    class_<GL::GeometryBufferView::AttributeSlice, boost::noncopyable>("AttributeSlice", no_init)
        .def("getLength", &GL::GeometryBufferView::AttributeSlice::getLength)
        .def("getSize", &GL::GeometryBufferView::AttributeSlice::getSize)
        .def("get", &AttributeSlice_get)
        .def("set", &AttributeSlice_set)
    ;

    class_<GL::GeometryBufferView, GL::GeometryBufferViewHandle, boost::noncopyable>("GeometryBufferView",
            init<   const GL::GenericGeometryBufferHandle,
                    const GL::VertexFormatHandle,
                    const GL::VertexIndexListHandle>())
        .def("vertices", &GL::GeometryBufferView::getPositionView,
                return_value_policy<reference_existing_object>())
    ;
}

void addGLToInittab()
{
    PyImport_AppendInittab("_cuni_gl", &init_cuni_gl);
}


}
