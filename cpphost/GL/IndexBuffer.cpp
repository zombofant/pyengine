/**********************************************************************
File name: IndexBuffer.cpp
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
#include "IndexBuffer.hpp"

namespace PyUni {
namespace GL {
    
using namespace PyUni;

/* PyUni::GL::IndexEntry */

IndexEntry::IndexEntry(const VertexIndex aStart, const VertexIndexListHandle aVertices):
    start(aStart),
    vertices(aVertices),
    count(aVertices->size())
{
    
}

/* PyUni::GL::GenericIndexBuffer */

GenericIndexBuffer::GenericIndexBuffer(const GLenum aPurpose):
    GenericBuffer(sizeof(GLuint), GL_ELEMENT_ARRAY_BUFFER, aPurpose),
    count(0)
{
    
}

void GenericIndexBuffer::clear() {
    count = 0;
}

void GenericIndexBuffer::draw(const GLenum mode) {
    glDrawElements(mode, count, GL_UNSIGNED_INT, 0);
    // std::cout << count << std::endl;
}

void GenericIndexBuffer::drawUnbound(const GLenum mode) {
    glDrawElements(mode, count, GL_UNSIGNED_INT, data);
    // std::cout << count << std::endl;
}

void GenericIndexBuffer::dump() {
    GLuint *dataptr = (GLuint *)data;
    for (int i = 0; i < count; i++) {
        std::cout << dataptr[i] << " ";
    }
    std::cout << std::endl;
}

/* PyUni::GL::StreamIndexBuffer */

StreamIndexBuffer::StreamIndexBuffer(const GLenum aPurpose):
    GenericIndexBuffer(aPurpose)
{
    
}

void StreamIndexBuffer::add(const VertexIndexListHandle vertices) {
    VertexIndexList *list = vertices.get();
    const GLsizei len = (GLsizei)list->size();
    while (count + len > capacity) {
        expand();
    }
    GLuint *dataptr = (GLuint *)data;
    dataptr += count;
    for (VertexIndexList::iterator it = list->begin();
        it != list->end();
        it++)
    {
        *dataptr = *it;
        dataptr++;
    }
}

/* PyUni::GL::StaticIndexBuffer */

StaticIndexBuffer::StaticIndexBuffer(const GLenum aPurpose):
    GenericIndexBuffer(aPurpose),
    handles(new IndexEntryHandles())
{
    
}

StaticIndexBuffer::~StaticIndexBuffer() {
    delete handles;
}

void StaticIndexBuffer::compress() {
    VertexIndex minChange = -1, maxChange = count;
    VertexIndex nextStart = 0;
    for (IndexEntryHandles::iterator it = handles->begin();
        it != handles->end();
        it++)
    {
        const IndexEntryHandle handle = *it;
        IndexEntry *entry = handle.get();
        
        if (entry->start > nextStart) {
            if ((minChange > nextStart) || (minChange == -1)) {
                minChange = nextStart;
            }
            updateHandles(it, (entry->start - nextStart));
        }
        
        nextStart = entry->start + entry->count;
    }
    count = nextStart;
    if ((minChange < maxChange) && (minChange >= 0)) {
        bind();
        doFlushRange(minChange, (maxChange - minChange) + 1);
    }
}

void StaticIndexBuffer::updateHandles(const IndexEntryHandles::iterator startAt, const GLsizei offset) {
    for (IndexEntryHandles::iterator it(startAt);
        it != handles->end();
        it++)
    {
        const IndexEntryHandle handle = *it;
        IndexEntry *entry = handle.get();
        entry->start += offset;
    }
}

const IndexEntryHandle StaticIndexBuffer::add(const VertexIndexListHandle vertices) {
    VertexIndexList *list = vertices.get();
    const GLsizei addCount = (GLsizei)vertices->size();
    
    
    if (count + addCount > capacity) {
        gc();
        while (count + addCount > capacity) {
            expand();
        }
    }
    
    const IndexEntryHandle handle(new IndexEntry(count, vertices));
    IndexEntry *entry = handle.get();
    
    GLuint *dataptr = (GLuint *)data;
    dataptr += entry->start;
    
    for (VertexIndexList::iterator it = list->begin();
        it != list->end();
        it++)
    {
        *dataptr = *it;
        dataptr++;
    }
    count += entry->count;
    if (glID != 0) {
        glBindBuffer(bufferKind, glID);
        // std::cout << "writethrough" << std::endl;
        glBufferSubData(bufferKind, entry->start * sizeof(GLuint), entry->count * sizeof(GLuint), (unsigned char *)( ((GLuint *)dataptr)+entry->start));
        glBindBuffer(bufferKind, 0);
    }
    
    handles->push_back(handle);
    
    return handle;
}

void StaticIndexBuffer::clear() {
    GenericIndexBuffer::clear();
    handles->clear();
}

void StaticIndexBuffer::drawHandle(const IndexEntryHandle handle, const GLenum mode) {
    IndexEntryHandles::iterator it = find(handles->begin(), handles->end(), handle);
    if (it == handles->end())
        return;
    IndexEntry *entry = handle.get();
    GLuint *dataptr = (GLuint *)data;
    dataptr += entry->start;
    glDrawElements(mode, entry->count, GL_UNSIGNED_INT, (const GLvoid*)dataptr);
}

void StaticIndexBuffer::gc() {
    bool changed = true;
    bool globalChanged = false;
    while (changed) {
        changed = false;
        for (IndexEntryHandles::iterator it = handles->begin();
            it != handles->end();
            it++)
        {
            const IndexEntryHandle handle = *it;
            if (handle.use_count() == 1) {
                remove(handle, false);
                changed = true;
                globalChanged = true;
                break;
            }
        }
    }
    if (globalChanged) {
        compress();
    }
}

void StaticIndexBuffer::remove(const IndexEntryHandle handle, const bool autoCompress) {
    IndexEntryHandles::iterator it = find(handles->begin(), handles->end(), handle);
    if (it == handles->end()) {
        return;
    }
    
    handles->erase(it);
    
    // We must not change count here, as for now we allow removing 
    // handles without deallocating their space (see autoCompress)
    if (autoCompress) {
        compress();
    }
}

const VertexIndexListHandle StaticIndexBuffer::resolveIndexEntry(const IndexEntryHandle handle) const {
    return handle->vertices;
}

}
}
