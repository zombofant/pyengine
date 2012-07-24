/**********************************************************************
File name: Leaf.hpp
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

For feedback and questions about pyuni please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/
#ifndef _PYE_SCENEGRAPH_LEAF_H
#define _PYE_SCENEGRAPH_LEAF_H

#include <unordered_map>

#include "GL/GeometryBuffer.hpp"
#include "GL/StateManagement.hpp"
#include "Spatial.hpp"

namespace PyEngine {
namespace SceneGraph {

typedef std::unordered_map<PyEngine::GL::StateGroupHandle, PyEngine::GL::VertexIndexListHandle> VertexMap;
typedef boost::shared_ptr<VertexMap> VertexMapHandle;

class Leaf;

typedef boost::shared_ptr<Leaf> LeafHandle;

class Leaf: public Spatial
{
    protected:
        Leaf();
    protected:
        VertexMapHandle _vertexMap;
    public:
        void draw();
        VertexMapHandle getVertexMap() { return _vertexMap; };
};

}
}

namespace std {

template <>
struct hash<PyEngine::GL::StateGroupHandle>
{
    size_t operator() (const PyEngine::GL::StateGroupHandle handle) const
    {
        return (size_t)handle.get();
    }
};


template <>
struct hash<PyEngine::GL::VertexIndexListHandle>
{
    size_t operator() (const PyEngine::GL::VertexIndexListHandle handle) const
    {
        return (size_t)handle.get();
    }
};

}

#endif
