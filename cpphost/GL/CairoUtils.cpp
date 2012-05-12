/**********************************************************************
File name: CairoUtils.cpp
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
#include "CairoUtils.hpp"

#include "Misc/Exception.hpp"
#include "Base.hpp"

namespace PyUni {
namespace GL {

void glTexCairoSurfaceSubImage2D(GLenum target,
    GLint level,
    GLint xoffset, GLint yoffset,
    cairo_surface_t *surface)
{
    if (cairo_surface_get_type(surface) != CAIRO_SURFACE_TYPE_IMAGE) {
        throw Exception("Got surface type which is not ImageSurface");
    }

    const cairo_format_t fmt = cairo_image_surface_get_format(surface);
    const GLsizei width = cairo_image_surface_get_width(surface);
    if (cairo_image_surface_get_stride(surface) != cairo_format_stride_for_width(fmt, width)) {
        throw Exception("Unsupported stride in image surface");
    }

    GLenum glType = GL_UNSIGNED_BYTE;
    GLenum glFormat;
    if ((fmt == CAIRO_FORMAT_ARGB32) || (fmt == CAIRO_FORMAT_RGB24)) {
        // cairo people are strange. They store RGB24 in 32bitse without putting
        // that in the format name...
        // so basically we have to rely on cairo to null the alpha channel bits
        // for RGB24 formats, otherwise we'll get unexpected invalid data in the
        // alpha channel.
        glFormat = GL_BGRA;
    } else {
        throw Exception("Got unsupported image surface format");
    }
    
    glTexSubImage2D(target, level, xoffset, yoffset,
        width,
        cairo_image_surface_get_height(surface),
        glFormat,
        glType,
        (const GLvoid*)cairo_image_surface_get_data(surface));
    raiseLastGLError();
}

}
}
