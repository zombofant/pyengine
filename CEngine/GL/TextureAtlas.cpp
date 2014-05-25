/**********************************************************************
File name: TextureAtlas.cpp
This file is part of: Pythonic Engine

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

For feedback and questions about pyengine please e-mail one of the
authors named in the AUTHORS file.
**********************************************************************/
#include "TextureAtlas.hpp"

#include <cstring>

#include "CEngine/Misc/Exception.hpp"

namespace PyEngine {
namespace GL {

/* PyEngine::GL::TextureAtlas */

TextureAtlas::TextureAtlas(
        const GLenum internal_format,
        const GLsizei initial_width,
        const GLsizei initial_height):
    _packer(initial_width, initial_height),
    _width(initial_width),
    _height(initial_height),
    owns_texture(true)
{
    glGenTextures(1, &_glid);

    bind();
    glTexImage2D(
        GL_TEXTURE_2D, 0,
        internal_format,
        _width, _height, 0,
        GL_LUMINANCE,
        GL_UNSIGNED_BYTE, 0);
    raiseLastGLError();
}

TextureAtlas::~TextureAtlas()
{
    if (owns_texture) {
        glDeleteTextures(1, &_glid);
    }
}

void TextureAtlas::grow()
{
    /*void *buffer = malloc(_width * _height);
    glGetTexImage2D(GL_TEXTURE_2D,
    GLsizei newWidth, newHeight;
    if (_width < _height)
    {
        newWidth = _width * 2;
        newHeight = _height;
    } else {
        newHeight = _height * 2;
        newWidth = _width;
    }*/
    throw Exception("FIXME: TextureAtlas::grow not implemented.");
}

void TextureAtlas::set_segment(
    const rbp::Rect &rect,
    const void *data,
    bool rotated,
    GLVertexFloat *rmin, GLVertexFloat *rmax,
    GLVertexFloat *smin, GLVertexFloat *smax)
{
    const GLVertexFloat
        firstMin = ((double)rect.x / (double)_width),
        firstMax = ((double)(rect.x + rect.height) / (double)_height),
        secondMin = ((double)rect.y / (double)_height),
        secondMax = ((double)(rect.y + rect.height) / (double)_height);
    if (rotated)
    {
        *rmin = secondMin;
        *rmax = secondMax;
        *smin = firstMin;
        *smax = firstMax;
    } else {
        *rmin = firstMin;
        *rmax = firstMax;
        *smin = secondMin;
        *smax = secondMax;
    }
    glTexSubImage2D(GL_TEXTURE_2D, 0, rect.x, rect.y, rect.width, rect.height, GL_ALPHA, GL_UNSIGNED_BYTE, data);
}

void TextureAtlas::bind()
{
    glBindTexture(GL_TEXTURE_2D, _glid);
}

void TextureAtlas::gc()
{
    uint32 changes = 0;
    std::vector<rbp::Rect> &freeRects = _packer.GetFreeRectangles();
    std::vector<rbp::Rect> &usedRects = _packer.GetUsedRectangles();
    for (   auto it = _handles.begin();
            it != _handles.end();
            it++)
    {
        if ((*it).unique())
        {
            AllocationHandle handle = *it;
            const rbp::Rect &handleRect = handle->rect;
            it = _handles.erase(it);
            for (   auto usedIt = usedRects.begin();
                    usedIt != usedRects.end();
                    usedIt++)
            {
                rbp::Rect &r = *usedIt;
                if (r == handleRect)
                {
                    freeRects.push_back(r);
                    usedRects.erase(usedIt);
                    break;
                }
            }
            changes += 1;
        }
    }
    if (changes > 0)
    {
        _packer.MergeFreeList();
    }
}

void TextureAtlas::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

TextureAtlas::AllocationHandle TextureAtlas::upload(
    AbstractImage2D *image)
{
    const GLsizei width = image->getWidth();
    const GLsizei height = image->getHeight();
    // TODO: Evaluate which parameters are best / make them configurable
    rbp::Rect r = _packer.Insert(
        width, height, false,
        rbp::GuillotineBinPack::RectBestAreaFit,
        rbp::GuillotineBinPack::SplitMinimizeArea);
    if ((r.height == 0) || (r.width == 0))
    {
        return nullptr;
    }
    if (r.height == width && width != height)
    {
        // we must do some magic to rotate the quad...
        const GLsizei pixelSize = image->getPixelSize();
        const uint8 *pixelData = (const uint8 *)image->getPixelData();
        uint8 *buffer = (uint8*)malloc(width * height * pixelSize);
        for (sizeint row = 0; row < height; row++)
        {
            for (sizeint col = 0; col < width; col++)
            {
                memcpy(&(buffer[col*height+row]), (const void*)pixelData, pixelSize);
                pixelData += pixelSize;
            }
        }
        GLVertexFloat rmin = 0., rmax = 0., smin = 0., smax = 0.;
        set_segment(
            r, buffer, true,
            &rmin, &rmax, &smin, &smax);
        free(buffer);
        AllocationHandle alloc = AllocationHandle(
            new Allocation(r, true,
                           rmin, rmax, smin, smax));
        _handles.push_back(alloc);
        return alloc;
    }
    else
    {
        GLVertexFloat rmin = 0., rmax = 0., smin = 0., smax = 0.;
        set_segment(
            r, image->getPixelData(), false,
            &rmin, &rmax, &smin, &smax);
        AllocationHandle alloc = AllocationHandle(
            new Allocation(r, false,
                           rmin, rmax, smin, smax));
        _handles.push_back(alloc);
        return alloc;
    }
}

/* PyEngine::GL::TextureAtlas::Allocation */

TextureAtlas::Allocation::Allocation(
        rbp::Rect aRect,
        bool aRotated,
        const GLVertexFloat armin, const GLVertexFloat armax,
        const GLVertexFloat asmin, const GLVertexFloat asmax):
    rect(aRect),
    rotated(aRotated),
    rmin(armin),
    rmax(armax),
    smin(asmin),
    smax(asmax)
{

}

}
}

bool operator == (const rbp::Rect &a, const rbp::Rect &b)
{
    return  (a.x == b.x) &&
            (a.y == b.y) &&
            (a.width == b.width) &&
            (a.height == b.height);
}
