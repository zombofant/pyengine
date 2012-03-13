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

#include <GL/glew.h>
#include "Base.hpp"
#include <limits>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <list>
#include <set>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "BufferMap.hpp"
#include "GenericBuffer.hpp"
#include <string.h>
#include "Math/Vectors.hpp"

namespace PyUni {
namespace GL {
    
using namespace PyUni;

typedef float GLVertexFloat;
typedef GLsizei VertexIndex;
typedef std::vector<VertexIndex> VertexIndexList;
typedef boost::shared_ptr<VertexIndexList> VertexIndexListHandle;
typedef std::vector<VertexIndexListHandle> VertexIndexListHandleList;

#define glType GL_FLOAT
#define glTypeSize sizeof(GLVertexFloat)

struct VertexFormat {
    const unsigned int nPosition, nColour, nTexCoord0, 
        nTexCoord1, nTexCoord2, nTexCoord3, nVertexAttrib0,
        nVertexAttrib1, nVertexAttrib2, nVertexAttrib3;
    const bool normal;
    const GLsizei posOffset, colourOffset, texCoord0Offset, 
        texCoord1Offset, texCoord2Offset, texCoord3Offset,
        normalOffset, vertexAttrib0Offset, vertexAttrib1Offset,
        vertexAttrib2Offset, vertexAttrib3Offset;
    const GLsizei vertexSize;
    
    
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

};
typedef boost::shared_ptr<VertexFormat> VertexFormatHandle;

class GeometryBufferDriver;
typedef boost::shared_ptr<GeometryBufferDriver> GeometryBufferDriverHandle;

class GenericGeometryBuffer: public GenericBuffer {
    public:
        GenericGeometryBuffer(const VertexFormatHandle vertexFormat, const GLenum aPurpose);
    protected:
        const VertexFormatHandle _vertexFormat;
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
        const VertexFormatHandle getFormat() const { return _vertexFormat; }
        BufferMapHandle getMap();
        void setMap(BufferMapHandle aValue);
        
    friend class GeometryBufferDriver;
};

typedef boost::shared_ptr<GenericGeometryBuffer> GenericGeometryBufferHandle;

template <
    unsigned int nPos = 0, 
    unsigned int nColour = 0, 
    unsigned int nTexCoord0 = 0, 
    unsigned int nTexCoord1 = 0, 
    unsigned int nTexCoord2 = 0, 
    unsigned int nTexCoord3 = 0, 
    bool normal = false, 
    unsigned int nVertexAttrib0 = 0, 
    unsigned int nVertexAttrib1 = 0, 
    unsigned int nVertexAttrib2 = 0, 
    unsigned int nVertexAttrib3 = 0>
class GeometryBuffer: public GenericGeometryBuffer {
    public:
        GeometryBuffer(const GLenum aPurpose = GL_DYNAMIC_DRAW):
            GenericGeometryBuffer(VertexFormatHandle(new VertexFormat(nPos, nColour, nTexCoord0, nTexCoord1, nTexCoord2, nTexCoord3, normal, nVertexAttrib0, nVertexAttrib1, nVertexAttrib2, nVertexAttrib3)), aPurpose)
        {
            
        };
    private:
        BOOST_STATIC_ASSERT(nPos >= 2);
        BOOST_STATIC_ASSERT(nPos <= 4);
        BOOST_STATIC_ASSERT((nColour == 0) || (nColour >= 3));
        BOOST_STATIC_ASSERT(nColour <= 4);
        BOOST_STATIC_ASSERT(nTexCoord0 <= 4);
        BOOST_STATIC_ASSERT(nTexCoord1 <= 4);
        BOOST_STATIC_ASSERT(nTexCoord2 <= 4);
        BOOST_STATIC_ASSERT(nTexCoord3 <= 4);
        BOOST_STATIC_ASSERT(nVertexAttrib0 <= 4);
        BOOST_STATIC_ASSERT(nVertexAttrib1 <= 4);
        BOOST_STATIC_ASSERT(nVertexAttrib2 <= 4);
        BOOST_STATIC_ASSERT(nVertexAttrib3 <= 4);
    private:
        static const GLsizei posOffset = 0;
        static const GLsizei colourOffset = posOffset + (nPos * sizeof(GLVertexFloat));
        static const GLsizei texCoord0Offset = colourOffset + (nColour * sizeof(GLVertexFloat));
        static const GLsizei texCoord1Offset = texCoord0Offset + (nTexCoord0 * sizeof(GLVertexFloat));
        static const GLsizei texCoord2Offset = texCoord1Offset + (nTexCoord1 * sizeof(GLVertexFloat));
        static const GLsizei texCoord3Offset = texCoord2Offset + (nTexCoord2 * sizeof(GLVertexFloat));
        static const GLsizei normalOffset = texCoord3Offset + (nTexCoord3 * sizeof(GLVertexFloat));
        static const GLsizei vertexAttrib0Offset = normalOffset + (normal?3:0) * sizeof(GLVertexFloat);
        static const GLsizei vertexAttrib1Offset = vertexAttrib0Offset + (nVertexAttrib0 * sizeof(GLVertexFloat));
        static const GLsizei vertexAttrib2Offset = vertexAttrib1Offset + (nVertexAttrib1 * sizeof(GLVertexFloat));
        static const GLsizei vertexAttrib3Offset = vertexAttrib2Offset + (nVertexAttrib2 * sizeof(GLVertexFloat));
        static const GLsizei vertexSize = (vertexAttrib3Offset + (nVertexAttrib3 * sizeof(GLVertexFloat)));
        
    public:
        virtual void bind() {
            GenericBuffer::bind();
            if (nPos > 0) {
                glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(nPos, glType, vertexSize, (const void*)(posOffset));
            }
            if (nColour > 0) {
                glEnableClientState(GL_COLOR_ARRAY);
                glColorPointer(nColour, glType, vertexSize, (const void*)(colourOffset));
            }
            if (nTexCoord0 > 0) {
                glClientActiveTexture(GL_TEXTURE0);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(nTexCoord0, glType, vertexSize, (const void*)texCoord0Offset);
            }
            if (nTexCoord1 > 0) {
                glClientActiveTexture(GL_TEXTURE1);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(nTexCoord1, glType, vertexSize, (const void*)texCoord1Offset);
            }
            if (nTexCoord2 > 0) {
                glClientActiveTexture(GL_TEXTURE2);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(nTexCoord2, glType, vertexSize, (const void*)texCoord2Offset);
            }
            if (nTexCoord3 > 0) {
                glClientActiveTexture(GL_TEXTURE3);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(nTexCoord3, glType, vertexSize, (const void*)texCoord3Offset);
            }
            if (normal) {
                glEnableClientState(GL_NORMAL_ARRAY);
                glNormalPointer(glType, vertexSize, (const void*)normalOffset);
            }
            if (nVertexAttrib0 > 0) {
                glVertexAttribPointer(0, nVertexAttrib0, glType, GL_FALSE, vertexSize, (const void*)vertexAttrib0Offset);
            }
            if (nVertexAttrib1 > 0) {
                glVertexAttribPointer(1, nVertexAttrib1, glType, GL_FALSE, vertexSize, (const void*)vertexAttrib1Offset);
            }
            if (nVertexAttrib2 > 0) {
                glVertexAttribPointer(2, nVertexAttrib2, glType, GL_FALSE, vertexSize, (const void*)vertexAttrib2Offset);
            }
            if (nVertexAttrib3 > 0) {
                glVertexAttribPointer(3, nVertexAttrib3, glType, GL_FALSE, vertexSize, (const void*)vertexAttrib3Offset);
            }
        }
        
        virtual void unbind() {
            if (nPos > 0) {
                glDisableClientState(GL_VERTEX_ARRAY);
            }
            if (nColour > 0) {
                glDisableClientState(GL_COLOR_ARRAY);
            }
            if (nTexCoord0 > 0) {
                glClientActiveTexture(GL_TEXTURE0);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            if (nTexCoord1 > 0) {
                glClientActiveTexture(GL_TEXTURE1);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            if (nTexCoord2 > 0) {
                glClientActiveTexture(GL_TEXTURE2);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            if (nTexCoord3 > 0) {
                glClientActiveTexture(GL_TEXTURE3);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            if (normal) {
                glDisableClientState(GL_NORMAL_ARRAY);
            }
            if (nVertexAttrib0 > 0) {
                glVertexAttribPointer(0, 0, glType, GL_FALSE, 0, 0);
            }
            if (nVertexAttrib1 > 0) {
                glVertexAttribPointer(1, 0, glType, GL_FALSE, 0, 0);
            }
            if (nVertexAttrib2 > 0) {
                glVertexAttribPointer(2, 0, glType, GL_FALSE, 0, 0);
            }
            if (nVertexAttrib3 > 0) {
                glVertexAttribPointer(3, 0, glType, GL_FALSE, 0, 0);
            }
            GenericBuffer::unbind();
        }
};

/** 
 * This class provides typesafe access to a generic geometry buffer.
 * It exposes the neccessary get* and set* methods to access a buffer
 * of the specified vertex format (or a larger vertex format). For
 * example it should be possible to access a V4F_T2F buffer with a 
 * V3F driver.
 */
class GeometryBufferDriver {
    public:
        class GeometryBufferDrivedView {
            public:
                GeometryBufferDrivedView(
                    GeometryBufferDriver *driver,
                    GLsizei vertexOffset,
                    GLsizei vertexSize,
                    GLsizei elementCount
                );
            private:
                GeometryBufferDriver _driver;
                GLsizei _vertexOffset;
                GLsizei _vertexSize;
                GLsizei _elementCount;
            public:
                void set(const GLsizei index, const VectorFloat &source);
                void set(const GLsizei index, const Vector2f &source);
                void set(const GLsizei index, const Vector3f &source);
                void set(const GLsizei index, const Vector4f &source);

                void get(const GLsizei index, const VectorFloat &dest);
                void get(const GLsizei index, const Vector2f &dest);
                void get(const GLsizei index, const Vector3f &dest);
                void get(const GLsizei index, const Vector4f &dest);
        }
    private:
        GeometryBufferDriver(const GenericGeometryBufferHandle bufferHandle):
            _handle(bufferHandle),
            _vertexFormat(bufferHandle->getFormat())
        {
            
        }
    public:
        static GeometryBufferDriverHandle create(const GenericGeometryBufferHandle bufferHandle, const VertexFormatHandle targetFormat) 
        {
            if (!targetFormat->isCompatible(*(bufferHandle->getFormat().get()))) {
                return GeometryBufferDriverHandle();
            }
            GeometryBufferDriver *driver = new GeometryBufferDriver(bufferHandle);
            return GeometryBufferDriverHandle(driver);
        }
    private:
        GenericGeometryBufferHandle _handle;
        VertexFormatHandle _vertexFormat;
    public:
        GenericGeometryBufferHandle getHandle() { return _handle; }
    public:
        void setPosition(const GLsizei index, const Vector2f &source);
        void setPosition(const GLsizei index, const Vector3f &source);
        void setPosition(const GLsizei index, const Vector4f &source);
        
        void setPosition(const GLsizei index, const Vector2 &source) { setPosition(index, source.toVector2f()); }
        void setPosition(const GLsizei index, const Vector3 &source) { setPosition(index, source.toVector3f()); }
        void setPosition(const GLsizei index, const Vector4 &source) { setPosition(index, source.toVector4f()); }
        
        
        void setColour(const GLsizei index, const Vector3f &source);
        void setColour(const GLsizei index, const Vector4f &source);
        
        void setColour(const GLsizei index, const Vector3 &source) { setColour(index, source.toVector3f()); }
        void setColour(const GLsizei index, const Vector4 &source) { setColour(index, source.toVector4f()); }
        
        void setTexCoord0(const GLsizei index, const GLVertexFloat &source);
        void setTexCoord0(const GLsizei index, const Vector2f &source);
        void setTexCoord0(const GLsizei index, const Vector3f &source);
        void setTexCoord0(const GLsizei index, const Vector4f &source);
        
        void setTexCoord0(const GLsizei index, const Vector2 &source) { setTexCoord0(index, source.toVector2f()); };
        void setTexCoord0(const GLsizei index, const Vector3 &source) { setTexCoord0(index, source.toVector3f()); };
        void setTexCoord0(const GLsizei index, const Vector4 &source) { setTexCoord0(index, source.toVector4f()); };
        
        
        void setTexCoord1(const GLsizei index, const VectorFloat &source);
        void setTexCoord1(const GLsizei index, const Vector2f &source);
        void setTexCoord1(const GLsizei index, const Vector3f &source);
        void setTexCoord1(const GLsizei index, const Vector4f &source);
        
        void setTexCoord1(const GLsizei index, const Vector2 &source) { setTexCoord1(index, source.toVector2f()); };
        void setTexCoord1(const GLsizei index, const Vector3 &source) { setTexCoord1(index, source.toVector3f()); };
        void setTexCoord1(const GLsizei index, const Vector4 &source) { setTexCoord1(index, source.toVector4f()); };
        
        
        void setTexCoord2(const GLsizei index, const VectorFloat &source);
        void setTexCoord2(const GLsizei index, const Vector2f &source);
        void setTexCoord2(const GLsizei index, const Vector3f &source);
        void setTexCoord2(const GLsizei index, const Vector4f &source);
        
        void setTexCoord2(const GLsizei index, const Vector2 &source) { setTexCoord2(index, source.toVector2f()); };
        void setTexCoord2(const GLsizei index, const Vector3 &source) { setTexCoord2(index, source.toVector3f()); };
        void setTexCoord2(const GLsizei index, const Vector4 &source) { setTexCoord2(index, source.toVector4f()); };
        
        
        void setTexCoord3(const GLsizei index, const VectorFloat &source);
        void setTexCoord3(const GLsizei index, const Vector2f &source);
        void setTexCoord3(const GLsizei index, const Vector3f &source);
        void setTexCoord3(const GLsizei index, const Vector4f &source);
        
        void setTexCoord3(const GLsizei index, const Vector2 &source) { setTexCoord3(index, source.toVector2f()); };
        void setTexCoord3(const GLsizei index, const Vector3 &source) { setTexCoord3(index, source.toVector3f()); };
        void setTexCoord3(const GLsizei index, const Vector4 &source) { setTexCoord3(index, source.toVector4f()); };
        
        void setNormal(const GLsizei index, const Vector3f &source);
        void setNormal(const GLsizei index, const Vector3 &source) { setNormal(index, source.toVector3f()); };
        
        void setVertexAttrib0(const GLsizei index, const VectorFloat &source);
        void setVertexAttrib0(const GLsizei index, const Vector2f &source);
        void setVertexAttrib0(const GLsizei index, const Vector3f &source);
        void setVertexAttrib0(const GLsizei index, const Vector4f &source);
        
        void setVertexAttrib0(const GLsizei index, const Vector2 &source) { setVertexAttrib0(index, source.toVector2f()); };
        void setVertexAttrib0(const GLsizei index, const Vector3 &source) { setVertexAttrib0(index, source.toVector3f()); };
        void setVertexAttrib0(const GLsizei index, const Vector4 &source) { setVertexAttrib0(index, source.toVector4f()); };
        
        
        void setVertexAttrib1(const GLsizei index, const VectorFloat &source);
        void setVertexAttrib1(const GLsizei index, const Vector2f &source);
        void setVertexAttrib1(const GLsizei index, const Vector3f &source);
        void setVertexAttrib1(const GLsizei index, const Vector4f &source);
        
        void setVertexAttrib1(const GLsizei index, const Vector2 &source) { setVertexAttrib1(index, source.toVector2f()); };
        void setVertexAttrib1(const GLsizei index, const Vector3 &source) { setVertexAttrib1(index, source.toVector3f()); };
        void setVertexAttrib1(const GLsizei index, const Vector4 &source) { setVertexAttrib1(index, source.toVector4f()); };
        
        
        void setVertexAttrib2(const GLsizei index, const VectorFloat &source);
        void setVertexAttrib2(const GLsizei index, const Vector2f &source);
        void setVertexAttrib2(const GLsizei index, const Vector3f &source);
        void setVertexAttrib2(const GLsizei index, const Vector4f &source);
        
        void setVertexAttrib2(const GLsizei index, const Vector2 &source) { setVertexAttrib2(index, source.toVector2f()); };
        void setVertexAttrib2(const GLsizei index, const Vector3 &source) { setVertexAttrib2(index, source.toVector3f()); };
        void setVertexAttrib2(const GLsizei index, const Vector4 &source) { setVertexAttrib2(index, source.toVector4f()); };
        
        
        void setVertexAttrib3(const GLsizei index, const VectorFloat &source);
        void setVertexAttrib3(const GLsizei index, const Vector2f &source);
        void setVertexAttrib3(const GLsizei index, const Vector3f &source);
        void setVertexAttrib3(const GLsizei index, const Vector4f &source);
        
        void setVertexAttrib3(const GLsizei index, const Vector2 &source) { setVertexAttrib3(index, source.toVector2f()); };
        void setVertexAttrib3(const GLsizei index, const Vector3 &source) { setVertexAttrib3(index, source.toVector3f()); };
        void setVertexAttrib3(const GLsizei index, const Vector4 &source) { setVertexAttrib3(index, source.toVector4f()); };
        
        
        void getPosition(const GLsizei index, Vector2f &dest);
        void getPosition(const GLsizei index, Vector3f &dest);
        void getPosition(const GLsizei index, Vector4f &dest);
        
        void getNormal(const GLsizei index, Vector3f &dest);
};

}
}

#endif
