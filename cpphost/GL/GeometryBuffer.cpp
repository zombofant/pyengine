/**********************************************************************
File name: GeometryBuffer.cpp
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
#include "GeometryBuffer.hpp"
#include <cassert>

namespace PyUni {
namespace GL {

/* PyUni::GL::VertexFormat */

VertexFormat::VertexFormat(const unsigned int aNPosition,
        const unsigned int aNColour,
        const unsigned int aNTexCoord0, 
        const unsigned int aNTexCoord1, 
        const unsigned int aNTexCoord2, 
        const unsigned int aNTexCoord3, 
        const bool aNormal, 
        const unsigned int aNVertexAttrib0, 
        const unsigned int aNVertexAttrib1, 
        const unsigned int aNVertexAttrib2, 
        const unsigned int aNVertexAttrib3):        
        nPosition(aNPosition),
        nColour(aNColour),
        nTexCoord0(aNTexCoord0),
        nTexCoord1(aNTexCoord1),
        nTexCoord2(aNTexCoord2),
        nTexCoord3(aNTexCoord3),
        nVertexAttrib0(aNVertexAttrib0),
        nVertexAttrib1(aNVertexAttrib1),
        nVertexAttrib2(aNVertexAttrib2),
        nVertexAttrib3(aNVertexAttrib3),
        normal(aNormal),
        posOffset(0),
        colourOffset(posOffset + nPosition * glTypeSize),
        texCoord0Offset(colourOffset + nColour * glTypeSize),
        texCoord1Offset(texCoord0Offset + nTexCoord0 * glTypeSize),
        texCoord2Offset(texCoord1Offset + nTexCoord1 * glTypeSize),
        texCoord3Offset(texCoord2Offset + nTexCoord2 * glTypeSize),
        normalOffset(texCoord3Offset + nTexCoord3 * glTypeSize),
        vertexAttrib0Offset(normalOffset + (normal?3:0) * glTypeSize),
        vertexAttrib1Offset(vertexAttrib0Offset + nVertexAttrib0 * glTypeSize),
        vertexAttrib2Offset(vertexAttrib1Offset + nVertexAttrib1 * glTypeSize),
        vertexAttrib3Offset(vertexAttrib2Offset + nVertexAttrib2 * glTypeSize),
        vertexSize(vertexAttrib3Offset + nVertexAttrib3 * glTypeSize),
        vertexLength(nPosition + nColour + nTexCoord0 + nTexCoord1 +
            nTexCoord2 + nTexCoord3 + nVertexAttrib0 + nVertexAttrib1 +
            nVertexAttrib2 + nVertexAttrib3 + (normal?3:0))
{
    assert((nPosition >= 2) && (nPosition <= 4));
    assert((nColour >= 3) && (nColour <= 4));
    assert((nTexCoord0 <= 4));
    assert((nTexCoord1 <= 4));
    assert((nTexCoord2 <= 4));
    assert((nTexCoord3 <= 4));
    assert((nVertexAttrib0 <= 4));
    assert((nVertexAttrib1 <= 4));
    assert((nVertexAttrib2 <= 4));
    assert((nVertexAttrib3 <= 4));
    assert(vertexSize > 0); // this is a true assertion; with the first assert, this should never throw.
}

/* PyUni::GL::GenericGeometryBuffer */

GenericGeometryBuffer::GenericGeometryBuffer(const VertexFormatHandle vertexFormat, const GLenum aPurpose):
    GenericBuffer(vertexFormat->vertexSize, GL_ARRAY_BUFFER, aPurpose),
    _vertexFormat(vertexFormat),
    _handles(),
    _freeVertices(),
    _dirtyVertices()
{
    
}

void GenericGeometryBuffer::doExpand(const GLsizei oldCapacity, const GLsizei newCapacity)  
{
    GenericBuffer::doExpand(oldCapacity, newCapacity);
    for (VertexIndex i = oldCapacity; i < newCapacity; i++) {
        _freeVertices.push_back(i);
    }
}

void GenericGeometryBuffer::gc_one(const VertexIndexListHandle handle) {
    VertexIndexList *list = handle.get();
    // freeVertices.splice(freeVertices.begin(), *list);
    for (VertexIndexList::iterator it = list->begin();
        it != list->end();
        it++)
    {
        _freeVertices.push_back(*it);
    }
}

void GenericGeometryBuffer::get(const GLsizei index, const GLsizei offset, GLVertexFloat *value, const GLsizei n) {
    memcpy(value, data + (map(index) * itemSize) + offset, n * glTypeSize);    
}

GLsizei GenericGeometryBuffer::map(const GLsizei index) {
    if (bufferMap.get()) {
        return bufferMap->map(index);
    } else {
        return index;
    }
}

void GenericGeometryBuffer::set(const GLsizei index, const GLsizei offset, const GLVertexFloat *value, const GLsizei n) {
    const GLsizei mappedIndex = map(index);
    memcpy(data + (mappedIndex * itemSize) + offset, value, n * glTypeSize);
    _dirtyVertices.insert(mappedIndex);
}

VertexIndexListHandle GenericGeometryBuffer::allocateVertices(const GLsizei count) {
    if ((GLsizei)_freeVertices.size() < count) {
        gc();
        while ((GLsizei)_freeVertices.size() < count) {
            expand();
        }
    }
    
    VertexIndexList *result = new VertexIndexList();
    for (GLsizei i = 0; i < count; i++) {
        result->push_back(_freeVertices.front());
        _freeVertices.pop_front();
    }
    
    VertexIndexListHandle handle = VertexIndexListHandle(result);
    _handles.push_back(handle);
    return handle;
}

void GenericGeometryBuffer::gc() {
    bool changed = true;
    while (changed) {
        changed = false;
        for (VertexIndexListHandleList::iterator it = _handles.begin();
            it != _handles.end();
            it++)
        {
            VertexIndexListHandle handle = *it;
            if (handle.use_count() == 1) {
                gc_one(handle);
                _handles.erase(it);
                changed = true;
                break;
            }
        }
    }
}

/*GeometryBufferDriverHandle GenericGeometryBuffer::createDriver(const VertexFormatHandle handle) 
{
    return createDriver(*handle);
}*/

BufferMapHandle GenericGeometryBuffer::getMap() 
{
    return bufferMap;
}

void GenericGeometryBuffer::autoFlush() {
    std::cout << "auto-flushing geometry buffer " << glID << std::endl;
    VertexIndex min, max;
    std::set<VertexIndex>::iterator it = _dirtyVertices.begin();
    min = *it;
    max = *it;
    
    for (it++;
        it != _dirtyVertices.end();
        it++) 
    {
        if (min > *it) {
            min = *it;
        }
        if (max < *it) {
            max = *it;
        }
    }
    doFlushRange(min, (max - min) + 1);
    _dirtyVertices.clear();
}

void GenericGeometryBuffer::setMap(BufferMapHandle aValue) {
    bufferMap = aValue;
}

}
}
