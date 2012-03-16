/**********************************************************************
File name: IndexBuffer.hpp
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
#ifndef _PYUNI_GL_INDEX_BUFFERS_H
#define _PYUNI_GL_INDEX_BUFFERS_H

#include <glew.h>
#include "Base.hpp"
#include <limits>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <list>
#include <set>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "GeometryBuffer.hpp"

namespace PyUni {
namespace GL {
    
using namespace PyUni;

struct IndexEntry {
    VertexIndex start;
    const VertexIndexListHandle vertices;
    const VertexIndex count;
    
    IndexEntry(const VertexIndex aStart, const VertexIndexListHandle aVertices);
};
typedef boost::shared_ptr<IndexEntry> IndexEntryHandle;
typedef std::list<IndexEntryHandle> IndexEntryHandles;

class GenericIndexBuffer: public GenericBuffer {
    public:
        GenericIndexBuffer(const GLenum aPurpose);
    protected:
        GLsizei count;
    public:
        virtual void clear();
        void draw(const GLenum mode);
        void drawUnbound(const GLenum mode);
        void dump();
    public:
        GLsizei getCount() const { return count; };
};
typedef boost::shared_ptr<GenericIndexBuffer> GenericIndexBufferHandle;

class StreamIndexBuffer: public GenericIndexBuffer {
    public:
        StreamIndexBuffer(const GLenum aPurpose = GL_STREAM_DRAW);
    public:
        void add(const VertexIndexListHandle vertices);
};
typedef boost::shared_ptr<StreamIndexBuffer> StreamIndexBufferHandle;

class StaticIndexBuffer: public GenericIndexBuffer {
    public:
        StaticIndexBuffer(const GLenum aPurpose = GL_DYNAMIC_DRAW);
        virtual ~StaticIndexBuffer();
    private:
        IndexEntryHandles *handles;
    protected:
        void compress();
        void updateHandles(const IndexEntryHandles::iterator startAt, const VertexIndex offset);
    public:
        const IndexEntryHandle add(const VertexIndexListHandle vertices);
        virtual void clear();
        void drawHandle(const IndexEntryHandle handle, const GLenum mode);
        void gc();
        GLsizei getIndex(const GLsizei index) const { return ((GLuint *)data)[index]; }
        void remove(const IndexEntryHandle handle, const bool autoCompress = true);
        const VertexIndexListHandle resolveIndexEntry(const IndexEntryHandle handle) const;
};
typedef boost::shared_ptr<StaticIndexBuffer> StaticIndexBufferHandle;

}
}
#endif
