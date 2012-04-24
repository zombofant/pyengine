/**********************************************************************
File name: AbstractImage.cpp
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
#include "AbstractImage.hpp"

#include "Misc/Exception.hpp"
#include "Misc/Int.hpp"

#include <string.h>
#include <glew.h>

#include <iostream>

namespace PyUni {
namespace GL {

static cairo_user_data_key_t destroyHook = {0};

void destroyBuffer(void *data) {
    free(data);
}

/* PyUni::GL::AbstractImage2D */

/**
 * @brief simple alias for memcpy, as this is trivial.
 */
void copyABGR(const GLvoid *src, unsigned char *dst, sizeuint len) {
    memcpy((void*)dst, (const void*)src, len);
}

/**
 * @brief copy and reverse byte order.
 */
void copyRGBA(const GLvoid *src, unsigned char *dest, sizeuint len) {
    const int32 *curSrc = (const int32*)src;
    const int32 *endSrc = (const int32*)((const unsigned char*)src + len);
    int32 *curDst = (int32*)dest;

    for (; curSrc < endSrc; curSrc++) {
        *curDst = (
            ((*curSrc & 0x000000FF) << 16) |  // R → B
            ((*curSrc & 0x0000FF00) <<  0) |  // G → G
            ((*curSrc & 0x00FF0000) >> 16) |  // B → R
            ((*curSrc & 0xFF000000) >>  0)    // A → A
        );
    }
}

struct int24 {
    int8 r, g, b;
};

/**
 * @brief copy, reverse byte order and extend by one byte.
 */
void copyRGB(const GLvoid *src, unsigned char *dest, sizeuint len) {
    const int24 *curSrc = (const int24*)src;
    const int24 *endSrc = (const int24*)((const unsigned char*)src + len);
    int32 *curDst = (int32*)dest;

    for (; curSrc < endSrc; curSrc++) {
        *curDst = (
            (curSrc->r <<  0) |
            (curSrc->g <<  8) |
            (curSrc->b << 16)
        );
    }
}

typedef void copyFuncPtr(const GLvoid *, unsigned char *, sizeuint);

cairo_surface_t *AbstractImage2D::cairoSurface() {

    const GLsizei width = getWidth(), height = getHeight();
    const GLvoid *data = getPixelData();
    const GLenum format = getFormat();

    copyFuncPtr *copyFunc;
    cairo_format_t cairoFmt;

    switch (format)
    {
    case GL_RGBA:
        std::cout << "non-identity (RGBA)" << std::endl;
        // copyFunc = copyRGBA;
        copyFunc = copyABGR;
        cairoFmt = CAIRO_FORMAT_ARGB32;
        break;
    case GL_BGRA:
        std::cout << "identity (ABGR)" << std::endl;
        copyFunc = copyABGR;
        cairoFmt = CAIRO_FORMAT_ARGB32;
        break;
    case GL_RGB:
        std::cout << "non-identity (RGB)" << std::endl;
        copyFunc = copyRGB;
        cairoFmt = CAIRO_FORMAT_RGB24;
        break;
    default:
        throw Exception("Unsupported format in AbstractImage2D::cairoSurface");
    }

    // cairo surfaces always have a pixel size of 4 bytes
    unsigned char *buffer = (unsigned char *)malloc(width * height * 4);
    copyFunc(data, buffer, width * height * getPixelSize());
    cairo_surface_t *surf = cairo_image_surface_create_for_data(buffer,
        cairoFmt, width, height, 4*width);

    if (cairo_surface_status(surf) == CAIRO_STATUS_INVALID_STRIDE) {
        cairo_surface_destroy(surf);
        free(buffer);
        throw Exception("Cairo returned an invalid surface: CAIRO_STATUS_INVALID_STRIDE");
    }

    cairo_surface_set_user_data(surf, &destroyHook, buffer, &destroyBuffer);
    return surf;
}

}
}
