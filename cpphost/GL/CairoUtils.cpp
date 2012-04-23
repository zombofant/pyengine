#include "Base.hpp"
#include "CairoUtils.hpp"
#include "Misc/Exception.hpp"

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
