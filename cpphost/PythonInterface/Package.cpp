/**********************************************************************
File name: Package.cpp
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
#include "Package.hpp"

#include "Log.hpp"
#include "CairoHelpers.hpp"
#include "Pango.hpp"
#include "Window.hpp"
#include "GL.hpp"
#include "Resources.hpp"
#include "SceneGraph.hpp"
#include "RenderGraph.hpp"

namespace PyUni {

using namespace boost::python;

/* PyUni::PyStream */

inline PyObject *getCallable(PyObject *obj)
{
    if (obj == 0)
        return obj;
    if (!PyCallable_Check(obj))
        throw StreamError("Is not callable.");
    Py_INCREF(obj);
    return obj;
}

PyStream::PyStream(PyObject *obj):
    _readCall(getCallable(PyObject_GetAttrString(obj, "read"))),
    _writeCall(getCallable(PyObject_GetAttrString(obj, "write"))),
    _seekCall(getCallable(PyObject_GetAttrString(obj, "seek"))),
    _flushCall(getCallable(PyObject_GetAttrString(obj, "flush"))),
    _tellCall(getCallable(PyObject_GetAttrString(obj, "tell"))),
    _readable((_readCall != 0)),
    _seekable((_seekCall != 0) && (_tellCall != 0)),
    _writeable((_writeCall != 0))
{
    if (!_readable && !_writeable)
    {
        Py_XDECREF(_readCall);
        Py_XDECREF(_writeCall);
        Py_XDECREF(_seekCall);
        Py_XDECREF(_flushCall);
        Py_XDECREF(_tellCall);
        throw StreamError("Seems not to be a file-like (could find neither `read` nor `write`).");
    }
}

PyStream::~PyStream() throw()
{
    Py_XDECREF(_readCall);
    Py_XDECREF(_writeCall);
    Py_XDECREF(_seekCall);
    Py_XDECREF(_flushCall);
    Py_XDECREF(_tellCall);
}

void PyStream::doSeek(const int whence, const sizeint offset) const
{
    if (!_seekable)
    {
        return this->Stream::raiseSeekNotSupportedError();
    }
    PyObject    *pyOffset = PyInt_FromSsize_t(offset),
                *pyWhence = PyInt_FromLong(whence);
    if (!pyOffset || !pyWhence)
    {
        Py_XDECREF(pyOffset);
        Py_XDECREF(pyWhence);
        if (PyErr_Occurred() != 0)
        {
            throw error_already_set();
        }
        throw StreamError("Unknown error while attempting to create python arguments.");
    }
    Py_XDECREF(PyObject_CallFunctionObjArgs(_seekCall, pyOffset, pyWhence, 0));
    Py_DECREF(pyOffset);
    Py_DECREF(pyWhence);
    if (PyErr_Occurred() != 0)
    {
        throw error_already_set();
    }
}

void PyStream::flush()
{
    if (!_flushCall)
        return;
    Py_XDECREF(PyObject_CallObject(_flushCall, 0));
    if (PyErr_Occurred() != 0)
    {
        throw error_already_set();
    }
}

sizeuint PyStream::read(void *data, const sizeuint length)
{
    if (!_readable)
    {
        return this->Stream::read(data, length); // this will throw
    }
    PyObject *pyLen = PyInt_FromSsize_t(length);
    if (!pyLen)
    {
        if (PyErr_Occurred() != 0)
        {
            throw error_already_set();
        }
        throw StreamError("Unknown error while attempting to create python arguments.");
    }
    PyObject *bytes = PyObject_CallFunctionObjArgs(_readCall, pyLen, 0);
    Py_DECREF(pyLen);
    if (PyErr_Occurred() != 0)
    {
        throw error_already_set();
    }
    if (bytes && PyString_Check(bytes))
    {
        sizeint readLen = PyString_GET_SIZE(bytes);
        if (readLen <= 0) // heh, less than would be pretty weird
        {
            Py_DECREF(bytes);
            return 0;
        }
        memcpy(data, (const void*)PyString_AS_STRING(bytes), readLen);
        Py_DECREF(bytes);
        return readLen;
    }
    else
    {
        Py_XDECREF(bytes);
        throw StreamReadError("Read failed: Python did not return a valid object.");
    }
}

sizeuint PyStream::seek(const int whence, const sizeint offset)
{
    doSeek(whence, offset);
    return tell();
}

const sizeuint PyStream::size() const
{
    sizeuint pos = tell();
    try {
        doSeek(SEEK_END, 0);
        sizeuint len = tell();
        doSeek(SEEK_SET, pos);
        return len;
    } catch (...) {
        doSeek(SEEK_SET, pos);
        throw;
    }
}

const sizeuint PyStream::tell() const
{
    PyObject *pos = PyObject_CallObject(_tellCall, 0);
    if (pos && PyNumber_Check(pos))
    {
        sizeint intpos = PyNumber_AsSsize_t(pos, PyExc_OverflowError);
        if (PyErr_Occurred() != 0)
        {
            Py_DECREF(pos);
            throw error_already_set();
        }
        return intpos;
    }
    else
    {
        Py_XDECREF(pos);
        throw StreamError("Seek failed: tell() returned a non-numeric result.");
    }
}

sizeuint PyStream::write(const void *data, const sizeuint length)
{
    PyObject *str = PyString_FromStringAndSize((const char *)data, length);
    Py_XDECREF(PyObject_CallObject(_writeCall, str));
    Py_DECREF(str);
    if (PyErr_Occurred() != 0)
    {
        throw error_already_set();
    }
    return length;
}

PyStreamHandle PyStream::create(boost::python::object obj)
{
    try {
        return boost::shared_ptr<PyStream>(new PyStream(obj.ptr()));
    } catch (StreamError err) {
        PyErr_SetString(PyExc_ValueError, "Is not a valid file-like.");
        throw error_already_set();
    }
}

BOOST_PYTHON_MODULE(_cuni)
{
    class_<PyStream, PyStreamHandle, boost::noncopyable>("Stream", no_init)
        .def("__init__", make_constructor(&PyStream::create))
    ;
    implicitly_convertible<PyStreamHandle, StreamHandle>();
}

void addCUniToInittab()
{
    PyImport_AppendInittab("_cuni", &init_cuni);
    addLogToInittab();
    addPangoToInittab();
    addWindowToInittab();
    addGLToInittab();
    addResourcesToInittab();
    addSceneGraphToInittab();
    addRenderGraphToInittab();
}

}
