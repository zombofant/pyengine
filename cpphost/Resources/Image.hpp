/**********************************************************************
File name: Image.hpp
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
#ifndef _PYUNI_RESOURCES_IMAGE_H
#define _PYUNI_RESOURCES_IMAGE_H

#include <glew.h>
#include <png.h>
#include <iostream>
#include <boost/shared_ptr.hpp>

namespace PyUni {
namespace Resources {

struct Image;
typedef boost::shared_ptr<Image> ImageHandle;

typedef boost::shared_ptr<std::istream> IStreamHandle;

struct Image
{
    public:
        Image(GLvoid *pixelData,
            const GLsizei aWidth, const GLsizei aHeight,
            const GLenum aFormat, const GLenum aType);
        virtual ~Image();
    protected:
        GLvoid *_pixelData;
    public:
        const GLenum format, type;
        const GLsizei width, height;
    public:
        bool getIsValid() const;
        void dropData();
    public:
        void texImage2D(const GLenum target, const GLint level,
            const GLint internalFormat) const;
        void texSubImage2D(const GLenum target, const GLint level,
            const GLint internalFormat,
            const GLint x, const GLint y) const;
    public:
        static ImageHandle PNGImage(IStreamHandle input);
};

}
}

#endif
