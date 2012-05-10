/**********************************************************************
File name: SceneGraph.hpp
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
#ifndef _PYUNI_PYTHON_SCENE_GRAPH_H
#define _PYUNI_PYTHON_SCENE_GRAPH_H

#include <boost/python.hpp>

#include "SceneGraph/Node.hpp"
#include "SceneGraph/Spatial.hpp"
#include "SceneGraph/SceneGraph.hpp"
#include "SceneGraph/Leaf.hpp"

namespace PyUni {

class SceneGraphWrap: public SceneGraph::SceneGraph, public boost::python::wrapper<SceneGraph::SceneGraph>
{
    virtual void update()
    {
        this->get_override("update")();
    }

    virtual void draw()
    {
        this->get_override("draw")();
    }
};

class LeafWrap;
typedef boost::shared_ptr<LeafWrap> LeafWrapHandle;

class LeafWrap: public SceneGraph::Leaf, public boost::python::wrapper<SceneGraph::Leaf>
{
    protected:
        LeafWrap():
            SceneGraph::Leaf::Leaf() {};
    public:
    
        static LeafWrapHandle create()
        {
            LeafWrapHandle tmp(new LeafWrap());
            tmp->_weak = tmp;
            return tmp;
        }
};

void addSceneGraphToInittab();

}

#endif
