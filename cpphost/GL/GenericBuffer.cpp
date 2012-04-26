/**********************************************************************
File name: GenericBuffer.cpp
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
#include "GenericBuffer.hpp"

#include <cstddef>
#include <exception>
#include <stdexcept>
#include <iostream>

namespace PyUni {
namespace GL {

/* PyUni::GL::GenericBuffer */

GenericBuffer::GenericBuffer(const GLsizei aItemSize, const GLenum aKind, const GLenum aPurpose):
    Class(),
    capacity(0),
    data(0),
    bufferPurpose(aPurpose),
    bufferKind(aKind),
    itemSize(aItemSize)
{
    
}

GenericBuffer::~GenericBuffer() {
    if (glID != 0) {
        freeBuffer();
    }
    if (data) {
        free(data);
    }
}

void GenericBuffer::autoFlush() {
    doFlushAll();
}

void GenericBuffer::doExpand(const GLsizei oldCapacity, const GLsizei newCapacity) {
    
}

void GenericBuffer::doFlushAll() {
    glBufferSubData(bufferKind, 0, capacity * itemSize, data);
    raiseLastGLError();
}

void GenericBuffer::doFlushRange(const GLsizei minItem, const GLsizei count) {
    std::cerr << minItem << " " << count << " " << itemSize << std::endl;
    glBufferSubData(bufferKind, minItem * itemSize, count * itemSize, &data[minItem * itemSize]);
    raiseLastGLError();
}

void GenericBuffer::expand() {
    const GLsizei newCapacity = (capacity ? capacity * 2 : 128);
    const GLsizei oldSize = capacity * itemSize;
    const GLsizei newSize = newCapacity * itemSize;
    
    data = (unsigned char*)realloc(data, newSize);
    
    if (glID != 0) {
        glBindBuffer(bufferKind, glID);
        glBufferData(bufferKind, newSize, 0, bufferPurpose);
        glBufferSubData(bufferKind, 0, oldSize, data);
    }
    
    const GLsizei oldCapacity = capacity;
    capacity = newCapacity;
    doExpand(oldCapacity, newCapacity);
}
    
void GenericBuffer::freeBuffer() {
    glDeleteBuffers(1, &glID);
    glID = 0;
}

void GenericBuffer::initBuffer() {
    glGenBuffers(1, &glID);
    // std::cout << "initialized buffer " << glID << " capacity is currently " << capacity << std::endl;
    raiseLastGLError();
    if (capacity > 0) {
        glBindBuffer(bufferKind, glID);
        // std::cout << "writing " << capacity * itemSize << " bytes ( = " << capacity << " items) to the buffer as initalization" << std::endl;
        raiseLastGLError();
        glBufferData(bufferKind, capacity * itemSize, data, bufferPurpose);
        raiseLastGLError();
    }
}

void GenericBuffer::rangeCheck(const GLsizei index) {
    if ((index < 0) || (index >= capacity)) {
        throw std::out_of_range("buffer index out of range");
    }
}

void GenericBuffer::requireBuffer() {
    if (glID == 0) {
        initBuffer();
        raiseLastGLError();
    }
    if (glID == 0) {
        // throw Exception("buffer initialized with id 0.");
        std::cerr << "buffer initialized with id 0." << std::endl;
    }
}

void GenericBuffer::bind() {
    requireBuffer();
    glBindBuffer(bufferKind, glID);
    if (needsFlush()) {
        autoFlush();
    }
}

void GenericBuffer::flush() {
    if (glID == 0) {
        return;
    }
    bind();
    autoFlush();
}

void GenericBuffer::readBack() {
    requireBuffer();
    if (glID == 0) {
        return;
    }
    bind();
    // std::cout << "reading back " << capacity * itemSize << " bytes from GPU buffer #" << glID << std::endl;
    glGetBufferSubData(bufferKind, 0, capacity * itemSize, data);
}

void GenericBuffer::unbind() {
    glBindBuffer(bufferKind, 0);
}


}
}
