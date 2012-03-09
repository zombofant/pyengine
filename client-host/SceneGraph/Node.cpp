
#include <assert.h>

#include "Node.hpp"

namespace PyUni {

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

