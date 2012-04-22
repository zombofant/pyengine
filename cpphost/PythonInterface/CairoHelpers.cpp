#include "CairoHelpers.hpp"
#include <cairo/cairo.h>
#include <Python.h>

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
