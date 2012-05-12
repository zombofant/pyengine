/**********************************************************************
File name: CairoHelpers.cpp
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
#include "CairoHelpers.hpp"

#include <Python.h>
#include <cairo/cairo.h>

namespace PyUni {

using namespace boost::python;

Pycairo_CAPI_t *Pycairo_CAPI;

struct cairo_surface_from_Pycairo
{
    cairo_surface_from_Pycairo()
    {
        boost::python::converter::registry::push_back(
            &convertible,
            &construct,
            boost::python::type_id<cairo_surface_t*>());
    }

    static void* convertible(PyObject* obj_ptr)
    {
        if (!PyObject_IsInstance(obj_ptr, (PyObject *)Pycairo_CAPI->Surface_Type))
            return 0;
        return ((PycairoSurface*)obj_ptr)->surface;
        
    }

    static void construct(
        PyObject* obj_ptr,
        boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        cairo_surface_t *surface = ((PycairoSurface*)obj_ptr)->surface;
        void* storage = (
            (boost::python::converter::rvalue_from_python_storage<cairo_surface_t*>*)
            data)->storage.bytes;
        new (storage) (cairo_surface_t*)(surface);
        data->convertible = storage;
    }
};

void setupCairoHelpers() {
    /* We are NOT using Pycairo_IMPORT, because it uses an
     * undocumented(!) python API function and produces warnings which
     * look as if they came from *our* code. */

    PyObject *module = PyImport_ImportModule("cairo");
    if (!module) {
        throw error_already_set();
    }

    
    PyObject *capiObject = PyObject_GetAttrString(module, "CAPI");
    if (!capiObject) {
        Py_DECREF(module);
        throw error_already_set();
    }

    Pycairo_CAPI = (Pycairo_CAPI_t *)PyCObject_AsVoidPtr(capiObject);
    Py_DECREF(capiObject);
}

}
