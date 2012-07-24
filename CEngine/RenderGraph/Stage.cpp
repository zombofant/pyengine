/**********************************************************************
File name: Stage.cpp
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
#include "Stage.hpp"

namespace PyUni {
namespace RenderGraph {

/* PyUni::RenderGraph::Node */

Node::Node(StageHandle parent):
    _parent(parent)
{
    
}

Node::~Node()
{

}

StageHandle Node::getParent()
{
    return _parent.lock();
}

/* PyUni::RenderGraph::Stage */

Stage::Stage(StageHandle parent):
    Node::Node(parent),
    _nodes()
{

}

Stage::~Stage()
{

}

bool Stage::allowNode(Node *node)
{
    return true;
}

void Stage::executeChildren()
{
    for (auto it = _nodes.begin(); it != _nodes.end(); it++)
    {
        const NodeHandle node = *it;
        node->execute();
    }
}

bool Stage::addNode(NodeHandle node)
{
    if (!allowNode(&*node))
        return false;
    _nodes.push_back(node);
    return true;
}

void Stage::execute()
{
    executeChildren();
}

}
}
