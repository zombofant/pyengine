/**********************************************************************
File name: SceneGraph.hpp
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
#ifndef _PYE_PYTHON_SCENE_GRAPH_H
#define _PYE_PYTHON_SCENE_GRAPH_H

#include <cassert>
#include <boost/python.hpp>

#include "CEngine/SceneGraph/Node.hpp"
#include "CEngine/SceneGraph/Spatial.hpp"
#include "CEngine/SceneGraph/SceneGraph.hpp"
#include "CEngine/SceneGraph/Leaf.hpp"

namespace PyEngine {

class LeafWrap;
typedef boost::shared_ptr<LeafWrap> LeafWrapHandle;

class LeafWrap: public SceneGraph::Leaf, public boost::python::wrapper<SceneGraph::Leaf>
{
    virtual void draw()
    {
        this->get_override("draw")();
    }
};

void addSceneGraphToInittab();

}

#endif
