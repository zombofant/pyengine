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

namespace PyUni {

using namespace boost::python;

BOOST_PYTHON_MODULE(_cuni_gl)
{
    class_<StructWrap, boost::shared_ptr<StructWrap>, boost::noncopyable>("Struct")
        .def("bind", pure_virtual(&GL::Struct::bind))
        .def("unbind", pure_virtual(&GL::Struct::unbind))
    ;
    implicitly_convertible<boost::shared_ptr<StructWrap>, GL::StructHandle>();
}

void addGLToInittab()
{
    PyImport_AppendInittab("_cuni_gl", &init_cuni_gl);
}


}
