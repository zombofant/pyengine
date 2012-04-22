#ifndef _PYUNI_GL_CAIRO_UTILS_H
#define _PYUNI_GL_CAIRO_UTILS_H

#include <cairo/cairo.h>
#include <glew.h>

namespace PyUni {
namespace GL {

void glTexCairoSurfaceSubImage2D(GLenum target,
    GLint level,
    GLint xoffset, GLint yoffset,
    cairo_surface_t *surface);

}
}

#endif
