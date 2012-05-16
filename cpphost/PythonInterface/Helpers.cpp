#include "Helpers.hpp"

#include <boost/python.hpp>

namespace PyUni {

using namespace boost::python;

void ValueError(const std::string &message)
{
    PyErr_SetString(PyExc_ValueError, message.c_str());
    bpy::throw_error_already_set();
}

PyObject *extractUTF8String(PyObject *text, const char **data, int *len)
{
    PyObject *utf8Text = text;
    if (PyString_Check(text)) {
        // we hope its utf8 here...
        Py_INCREF(utf8Text);
    }
    else if (!PyUnicode_Check(text)) {
        PyErr_SetString(PyExc_TypeError, "Need unicode or (utf8 encoded) str");
        throw error_already_set();
    }
    utf8Text = PyUnicode_AsUTF8String(text);
    if (!utf8Text) {
        throw error_already_set();
    }

    *data = PyString_AS_STRING(utf8Text);

    if (len != NULL)
        *len = PyString_GET_SIZE(utf8Text);

    return utf8Text;
}

}

// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:

