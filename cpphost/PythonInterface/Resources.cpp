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

#include "GL.hpp"
#include <boost/shared_ptr.hpp>

namespace PyUni {

using namespace boost::python;
using namespace PyUni::Resources;

BOOST_PYTHON_MODULE(_cuni_resources)
{    
    class_<Image, ImageHandle, bases<AbstractImage2D>, boost::noncopyable>("Image", no_init)
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
