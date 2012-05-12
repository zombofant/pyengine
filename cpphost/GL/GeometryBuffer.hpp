/**********************************************************************
File name: GeometryBuffer.hpp
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
#ifndef _PYUNI_GL_GEOMETRY_BUFFERS_H
#define _PYUNI_GL_GEOMETRY_BUFFERS_H

#include <cstring>
#include <limits>
#include <list>
#include <set>
#include <vector>
#include <string>

#include <GL/glew.h>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/shared_ptr.hpp>

#include "Math/Vectors.hpp"
#include "Base.hpp"
#include "BufferMap.hpp"
#include "GenericBuffer.hpp"

namespace PyUni {
namespace GL {

typedef float GLVertexFloat;
typedef GLsizei VertexIndex;
typedef std::vector<VertexIndex> VertexIndexList;
typedef boost::shared_ptr<VertexIndexList> VertexIndexListHandle;
typedef boost::weak_ptr<VertexIndexList> WeakVertexIndexListHandle;
typedef std::vector<VertexIndexListHandle> VertexIndexListHandleList;

#define glType GL_FLOAT
#define glTypeSize sizeof(GLVertexFloat)

#define BUFFER_TEX_COORD_COUNT 4
#define BUFFER_VERTEX_ATTRIB_COUNT 4

struct VertexFormat {
    const unsigned int nPosition, nColour, nTexCoord0, 
        nTexCoord1, nTexCoord2, nTexCoord3, nVertexAttrib0,
        nVertexAttrib1, nVertexAttrib2, nVertexAttrib3;
    const bool normal;
    const size_t posOffset, colourOffset, texCoord0Offset, 
        texCoord1Offset, texCoord2Offset, texCoord3Offset,
        normalOffset, vertexAttrib0Offset, vertexAttrib1Offset,
        vertexAttrib2Offset, vertexAttrib3Offset;
    const size_t vertexSize, vertexLength;
    
    
    VertexFormat(const unsigned int aNPosition = 0,
        const unsigned int aNColour = 0,
        const unsigned int aNTexCoord0 = 0, 
        const unsigned int aNTexCoord1 = 0, 
        const unsigned int aNTexCoord2 = 0, 
        const unsigned int aNTexCoord3 = 0, 
        const bool aNormal = false, 
        const unsigned int aNVertexAttrib0 = 0, 
        const unsigned int aNVertexAttrib1 = 0, 
        const unsigned int aNVertexAttrib2 = 0, 
        const unsigned int aNVertexAttrib3 = 0);
    
    bool isCompatible(const VertexFormat &format) const {
        return !((format.nPosition > nPosition) ||
            (format.nColour > nColour) ||
            (format.nTexCoord0 > nTexCoord0) ||
            (format.nTexCoord1 > nTexCoord1) ||
            (format.nTexCoord2 > nTexCoord2) ||
            (format.nTexCoord3 > nTexCoord3) ||
            (format.normal != normal) ||
            (format.nVertexAttrib0 > nVertexAttrib0) ||
            (format.nVertexAttrib1 > nVertexAttrib1) ||
            (format.nVertexAttrib2 > nVertexAttrib2) ||
            (format.nVertexAttrib3 > nVertexAttrib3));
    }

    static VertexFormat *copy(const VertexFormat *vf);
};
typedef boost::shared_ptr<VertexFormat> VertexFormatHandle;


class GeometryBuffer: public GenericBuffer {
    public:
        GeometryBuffer(const VertexFormatHandle vertexFormat, const GLenum aPurpose);
    protected:
        const VertexFormat *_vertexFormat;
        VertexIndexListHandleList _handles;
        std::list<VertexIndex> _freeVertices;
        std::set<VertexIndex> _dirtyVertices;
        
        BufferMapHandle bufferMap;
    protected:
        virtual void autoFlush();
        virtual void doExpand(const GLsizei oldCapacity, const GLsizei newCapacity);
        void gc_one(const VertexIndexListHandle handle);
        void get(const GLsizei index, const GLsizei offset, GLVertexFloat *value, const GLsizei n);
        GLsizei map(const GLsizei index);
        virtual bool needsFlush() const { return _dirtyVertices.size() > 0; };
        void set(const GLsizei index, const GLsizei offset, const GLVertexFloat *value, const GLsizei n);
    public:
        VertexIndexListHandle allocateVertices(const GLsizei count);
        void gc();
    public:
        // virtual GeometryBufferDriverHandle createDriver(const VertexFormat &format) = 0;
        // GeometryBufferDriverHandle createDriver(const VertexFormatHandle format);
        GLVertexFloat *getData() { return (GLVertexFloat*)data; }
        const VertexFormat *getFormat() const { return _vertexFormat; }
        BufferMapHandle getMap();
        void invalidateRange(const GLsizei minIndex, const GLsizei maxIndex);
        void setMap(BufferMapHandle aValue);
    public:
        virtual void bind();
        void draw(const VertexIndexListHandle &handle, const GLenum mode);
        virtual void unbind();
};

typedef boost::shared_ptr<GeometryBuffer> GeometryBufferHandle;

}
}

#endif
