/**********************************************************************
File name: TextureAtlas.hpp
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
#ifndef _PYUNI_GL_TEXTURE_ATLAS_H
#define _PYUNI_GL_TEXTURE_ATLAS_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "Base.hpp"
#include "GeometryBuffer.hpp"
#include "AbstractImage.hpp"
#include "Contrib/BinPack/SkylineBinPack.h"

#include "Misc/Int.hpp"

namespace PyUni {
namespace GL {

class TextureAtlas: public Class
{
    public:
        struct Allocation {
            public:
                Allocation(Rect aRect, bool aRotated,
                    const GLVertexFloat armin,
                    const GLVertexFloat armax,
                    const GLVertexFloat asmin,
                    const GLVertexFloat asmax);
            public:
                const Rect rect;
                const bool rotated;
                const GLVertexFloat rmin, rmax, smin, smax; 
        };
        typedef boost::shared_ptr<Allocation> AllocationHandle;
    public:
        TextureAtlas(const GLuint textureID,
            const GLsizei initialWidth = 1024,
            const GLsizei initialHeight = 1024);
        ~TextureAtlas();
    private:
        GuillotineBinPack _packer;
        const GLsizei _width, _height;
        std::vector<AllocationHandle> _handles;
    protected:
        void grow();
        void setSegment(const Rect &rect, const void *data,
            bool rotated,
            GLVertexFloat *rmin, GLVertexFloat *rmax,
            GLVertexFloat *smin, GLVertexFloat *smax);
    public:
        bool ownsTexture;
    public:
        inline GLsizei getHeight() const { return _height; };
        inline GLsizei getWidth() const { return _width; };
    public:
        virtual void bind();
        
        void gc();

        virtual void unbind();
        
        /**
         * Assumes the texture is bound
         */
        AllocationHandle upload(AbstractImage2D *image, uint32 *grew);
};
typedef boost::shared_ptr<TextureAtlas> TextureAtlasHandle;

bool operator == (const Rect &a, const Rect &b);

}
}

#endif
