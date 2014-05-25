/**********************************************************************
File name: TextureAtlas.hpp
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
#ifndef _PYE_GL_TEXTURE_ATLAS_H
#define _PYE_GL_TEXTURE_ATLAS_H

#include "CEngine/Misc/Int.hpp"
typedef std::size_t size_t;
#include "CEngine/Contrib/BinPack/GuillotineBinPack.h"

#include "Base.hpp"
#include "AbstractImage.hpp"
#include "GeometryBuffer.hpp"

namespace PyEngine {
namespace GL {

class TextureAtlas: public Class
{
public:
    struct Allocation
    {
    public:
        Allocation(rbp::Rect aRect, bool aRotated,
                   const GLVertexFloat armin,
                   const GLVertexFloat armax,
                   const GLVertexFloat asmin,
                   const GLVertexFloat asmax);

    public:
        const rbp::Rect rect;
        const bool rotated;
        const GLVertexFloat rmin, rmax, smin, smax;

    };

    typedef std::shared_ptr<Allocation> AllocationHandle;

public:
    TextureAtlas(const GLenum internal_format,
                 const GLsizei initial_width = 1024,
                 const GLsizei initial_height = 1024);
    TextureAtlas(const GLuint textureid,
                 bool take_ownership);
    ~TextureAtlas();

private:
    rbp::GuillotineBinPack _packer;
    const GLsizei _width, _height;
    std::vector<AllocationHandle> _handles;

protected:
    void grow();
    void set_segment(
        const rbp::Rect &rect,
        const void *data,
        bool rotated,
        GLVertexFloat *rmin, GLVertexFloat *rmax,
        GLVertexFloat *smin, GLVertexFloat *smax);

public:
    bool owns_texture;

public:
    inline GLsizei get_height() const
    {
        return _height;
    }

    inline GLsizei get_width() const
    {
        return _width;
    }

public:
    void bind() override;

    void gc();

    void unbind() override;

    /**
     * Assumes the texture is bound
     */
    AllocationHandle upload(AbstractImage2D *image);

};

typedef std::shared_ptr<TextureAtlas> TextureAtlasHandle;

}
}

bool operator == (const rbp::Rect &a, const rbp::Rect &b);

#endif
