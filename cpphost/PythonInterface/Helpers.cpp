/**********************************************************************
File name: Helpers.cpp
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

