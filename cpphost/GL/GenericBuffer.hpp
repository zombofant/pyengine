/**********************************************************************
File name: GenericBuffer.hpp
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
#ifndef _PYUNI_GL_GENERIC_BUFFERS_H
#define _PYUNI_GL_GENERIC_BUFFERS_H

#include <GL/glew.h>
#include "Base.hpp"
#include <limits>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <list>
#include <set>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace PyUni {
namespace GL {

class GenericBuffer: public Class {
    public:
        GenericBuffer(const GLsizei aItemSize, const GLenum aKind, const GLenum aPurpose);
        virtual ~GenericBuffer();
    protected:
        GLsizei capacity;
        unsigned char *data;
    protected:
        const GLenum bufferPurpose;
        const GLenum bufferKind;
        const GLsizei itemSize;
    protected:
        virtual void autoFlush();
        virtual void doExpand(const GLsizei oldCapacity, const GLsizei newCapacity);
        void doFlushAll();
        void doFlushRange(const GLsizei minItem, const GLsizei count);
        void expand();
        virtual void freeBuffer();
        virtual void initBuffer();
        virtual bool needsFlush() const { return false; };
        void rangeCheck(const GLsizei index);
        virtual void requireBuffer();
    public:
        virtual void bind();
        void flush();
        void readBack();
        virtual void unbind();
    public:
        GLsizei getCapacity() const { return capacity; }
        GLsizei getItemSize() const { return itemSize; }
};


}
}

#endif
