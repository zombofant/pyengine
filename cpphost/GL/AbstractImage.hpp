/**********************************************************************
File name: AbstractImage.hpp
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
#ifndef _PYUNI_GL_ABSTRACT_IMAGE_H
#define _PYUNI_GL_ABSTRACT_IMAGE_H

#include "Base.hpp"

#include <cairo/cairo.h>

namespace PyUni {
namespace GL {

struct AbstractImage2D
{
    public:
        virtual GLenum getFormat() const = 0;
        virtual GLsizei getHeight() const = 0;
        virtual const void *getPixelData() const = 0;
        virtual GLsizei getPixelSize() const = 0;
        virtual GLenum getType() const = 0;
        virtual GLsizei getWidth() const = 0;
    public:
        virtual void texImage2D(const GLenum target, const GLint level,
            const GLint internalFormat) const = 0;
        virtual void texSubImage2D(const GLenum target,
            const GLint level, const GLint internalFormat,
            const GLint x, const GLint y) const = 0;

        /**
         * @brief Create a cairo ImageSurface from the data and return
         * the pointer. 
         *
         * Contents are copied on creation, so this operation should
         * be considered expensive. This will only work if the given
         * OpenGL format is one of `GL_RGBA`, `GL_BGRA` or `GL_RGB`,
         * otherwise an exception is thrown.
         *
         * For `GL_RGBA`, it is neccessary to reverse the bytes order,
         * making it an even more expensive operation than it already
         * is.
         *
         * For `GL_RGB`, the data needs to be expanded to 32bit words
         * instead of 24bit words, so this is also expensive.
         *
         * The cheapest source format is `GL_ABGR`.
         */
        cairo_surface_t *cairoSurface();

};

typedef boost::shared_ptr<AbstractImage2D> AbstractImage2DHandle;

}
}

#endif
