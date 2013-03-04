/**********************************************************************
File name: GeometryObject.hpp
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
#ifndef _PYE_GL_GEOMETRY_OBJECT_H
#define _PYE_GL_GEOMETRY_OBJECT_H

#include "BufferMap.hpp"
#include "GeometryBuffer.hpp"

namespace PyEngine {
namespace GL {

class VertexIndexListMap: public BufferMap {
    public:
        VertexIndexListMap(const VertexIndexListHandle vertices);
        virtual ~VertexIndexListMap();
    private:
        const VertexIndexListHandle _verticesHandle;
        VertexIndexList *_vertices;
    protected:
        void rangeCheck(const size_t index);
    public:
        virtual size_t map(const size_t index);
};

}
}

#endif
