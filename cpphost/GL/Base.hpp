/**********************************************************************
File name: Base.hpp
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
#ifndef _PYUNI_GL_BASE_H
#define _PYUNI_GL_BASE_H

#include <GL/glew.h>
#include <string>
#include <boost/shared_ptr.hpp>

namespace PyUni {
namespace GL {
    
/*class Error: public Exception {
    public:
        Error(const std::string aMessage);
        ~Error() throw() {};
    private:
        const std::string message;
    public:
        const char *what() {
            return message.c_str();
        }
};*/

class Struct {
    public:
        virtual void bind() = 0;
        virtual void unbind() = 0;
};

class Class: public Struct {
    public:
        Class();
    protected:
        GLuint glID;
    public:
        GLuint getID() const { return glID; }
};

typedef boost::shared_ptr<Struct> StructHandle;
typedef boost::shared_ptr<Class> ClassHandle;

// void raiseLastGLError();

}
}

#endif
