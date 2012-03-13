/**********************************************************************
File name: Node.cpp
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

#include <assert.h>

#include "Node.hpp"

namespace PyUni {
namespace SceneGraph {

Node::Node()
{
}

Node::~Node()
{
    std::vector<Spatial*>::iterator iter = children.begin();
    for(; iter != children.end(); ++iter)
    {
        if(*iter)
        {
            (*iter)->setParent(0);
            *iter = 0;
        }
    }
}

void Node::addChild(Spatial *child)
{
    assert(child);
    assert(!child->getParent());

    child->setParent(this);

    // try to find a free slot in the current vector
    std::vector<Spatial*>::iterator iter = children.begin();
    for(/**/; iter != children.end(); ++iter)
    {
        if(*iter == 0)
        {
            *iter = child;
            return;
        }
    }

    // extend vector
    children.push_back(child);
}

void Node::removeChild(Spatial *child)
{
    assert(child);

    if(child->getParent() != this)
        return;

    std::vector<Spatial*>::iterator iter = children.begin();
    for(/**/; iter != children.end(); ++iter)
    {
        if(*iter == child)
        {
            (*iter)->setParent(0);
            *iter = 0;
            return;
        }
    }
}

void Node::updateWorldData()
{
    Spatial::updateWorldData();

    std::vector<Spatial*>::iterator iter = children.begin();
    for(/**/; iter != children.end(); ++iter)
    {
        Spatial *child = *iter;
        if(child)
        {
            child->updateGeometry(false);
        }
    }
}

}
}
