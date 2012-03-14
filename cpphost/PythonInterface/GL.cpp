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

GL::GeometryBufferView::AttributeSlice *AttributeView_slice(GL::GeometryBufferView::AttributeView &view, object arg)
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

template <class SliceT>
PyObject *AttributeSlice_set(SliceT *slice, list bpList)
{
    const GLsizei len = slice->getLength() * slice->getAttributeLength();
    PyObject *pyList = bpList.ptr();
    if (PyList_Size(pyList) != len)
    {
        std::cerr << "need exactly " << len << " items, got " << PyList_Size(pyList) << "." << std::endl;
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
        .def_readwrite("id", &GL::Class::glID)
    ;
    implicitly_convertible<boost::shared_ptr<ClassWrap>, GL::ClassHandle>();

    class_<GL::VertexIndexList, GL::VertexIndexListHandle, boost::noncopyable>("VertexIndexList", no_init);

    class_<GL::GeometryBuffer, bases<GL::Class>, boost::noncopyable>("GeometryBuffer",
            init<const GL::VertexFormatHandle, GLenum>())
        .def("allocateVertices", &GL::GeometryBuffer::allocateVertices)
    ;

    class_<GL::GeometryBufferView::AttributeView, boost::noncopyable>("AttributeView", no_init)
        .def("__getitem__", &AttributeView_slice, return_value_policy<reference_existing_object>())
        .def("__len__", &GL::GeometryBufferView::AttributeView::getLength)
        .add_property("AttributeLength", &GL::GeometryBufferView::AttributeView::getAttributeLength)
        .add_property("Size", &GL::GeometryBufferView::AttributeView::getSize)
        .def("get", &AttributeSlice_get<GL::GeometryBufferView::AttributeView>)
        .def("set", &AttributeSlice_set<GL::GeometryBufferView::AttributeView>)
    ;

    class_<GL::GeometryBufferView::AttributeSlice, boost::noncopyable>("AttributeSlice", no_init)
        .def("__len__", &GL::GeometryBufferView::AttributeSlice::getLength)
        .add_property("Size", &GL::GeometryBufferView::AttributeSlice::getSize)
        .def("get", &AttributeSlice_get<GL::GeometryBufferView::AttributeSlice>)
        .def("set", &AttributeSlice_set<GL::GeometryBufferView::AttributeSlice>)
    ;

    class_<GL::GeometryBufferView, GL::GeometryBufferViewHandle, boost::noncopyable>("GeometryBufferView",
            init<   const GL::GeometryBufferHandle,
                    const GL::VertexIndexListHandle>())
        .add_property("Vertex",
            make_function(&GL::GeometryBufferView::getPositionView,
                return_value_policy<reference_existing_object>())
        )
        .add_property("Colour",
            make_function(&GL::GeometryBufferView::getColourView,
                return_value_policy<reference_existing_object>())
        )
        .def("TexCoord", &GL::GeometryBufferView::getTexCoordView,
                return_value_policy<reference_existing_object>())
        .add_property("Normal",
            make_function(&GL::GeometryBufferView::getNormalView,
                return_value_policy<reference_existing_object>())
        )
        .def("Attrib", &GL::GeometryBufferView::getPositionView,
                return_value_policy<reference_existing_object>())
        .def("__len__", &GL::GeometryBufferView::getLength)
        
    ;
}

void addGLToInittab()
{
    PyImport_AppendInittab("_cuni_gl", &init_cuni_gl);
}


}
