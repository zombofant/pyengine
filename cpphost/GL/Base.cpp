/**********************************************************************
File name: Base.cpp
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
#include "Base.hpp"

#include <exception>
#include <iostream>

#include <GL/glew.h>

namespace PyUni {
namespace GL {

/* PyUni::GL::Error */

Error::Error(const std::string aMessage):
    Exception("OpenGL error: "+aMessage)
{
    
}

/* PyUni::GL::Class */

Class::Class():
    glID(0)
{
    
}

/* free functions */

/*void raiseLastGLError() {
    const GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << (const char *)gluErrorString(error) << std::endl;
        // throw Error((const char *)gluErrorString(error));
    }
    
}*/

}
}
