/**********************************************************************
File name: TextureAtlas.cpp
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
#include "TextureAtlas.hpp"
#include "Misc/Exception.hpp"

#include <cstring>

namespace PyUni {
namespace GL {

/* PyUni::GL::TextureAtlas */

TextureAtlas::TextureAtlas(const GLuint textureID,
        const GLsizei initialWidth, const GLsizei initialHeight):
    _packer(initialWidth, initialHeight),
    _width(initialWidth),
    _height(initialHeight),
    ownsTexture(textureID == 0)
{
    if (textureID == 0)
    {
        glGenTextures(1, &glID);
    } else {
        glID = textureID;
    }
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA8, _width, _height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
}

TextureAtlas::~TextureAtlas()
{
    if (ownsTexture)
        glDeleteTextures(1, &glID);
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

void TextureAtlas::setSegment(const Rect &rect, const void *data,
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
    glBindTexture(GL_TEXTURE_2D, glID);
}

void TextureAtlas::gc()
{
    uint32 changes = 0;
    std::vector<Rect> &freeRects = _packer.GetFreeRectangles();
    std::vector<Rect> &usedRects = _packer.GetUsedRectangles();
    for (   auto it = _handles.begin();
            it != _handles.end();
            it++)
    {
        if ((*it).unique())
        {
            AllocationHandle handle = *it;
            const Rect &handleRect = handle->rect;
            it = _handles.erase(it);
            for (   auto usedIt = usedRects.begin();
                    usedIt != usedRects.end();
                    usedIt++)
            {
                Rect &r = *usedIt;
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

TextureAtlas::AllocationHandle TextureAtlas::upload(AbstractImage2D *image, uint32 *grew)
{
    const GLsizei width = image->getWidth();
    const GLsizei height = image->getHeight();
    // TODO: Evaluate which parameters are best / make them configurable
    Rect r = _packer.Insert(width, height, false, 
        GuillotineBinPack::RectBestAreaFit,
        GuillotineBinPack::SplitMinimizeArea);
    if ((r.height == 0) || (r.width == 0))
    {
        if (grew)
        {
            *grew += 1;
            grow();
            return upload(image, grew);
        } else {
            return AllocationHandle();
        }
    }
    if (r.height == width)
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
        setSegment(r, buffer, true,
            &rmin, &rmax, &smin, &smax);
        free(buffer);
        AllocationHandle alloc = AllocationHandle(new Allocation(r, true,
            rmin, rmax, smin, smax));
        _handles.push_back(alloc);
        return alloc;
    }
    else
    {
        GLVertexFloat rmin = 0., rmax = 0., smin = 0., smax = 0.;
        setSegment(r, image->getPixelData(), false,
            &rmin, &rmax, &smin, &smax);
        AllocationHandle alloc = AllocationHandle(new Allocation(r, false,
            rmin, rmax, smin, smax));
        _handles.push_back(alloc);
        return alloc;
    }
}

bool operator == (const Rect &a, const Rect &b)
{
    return  (a.x == b.x) &&
            (a.y == b.y) &&
            (a.width == b.width) &&
            (a.height == b.height);
}

/* PyUni::GL::TextureAtlas::Allocation */

TextureAtlas::Allocation::Allocation(Rect aRect,
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
