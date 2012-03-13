#include "Stage.hpp"

namespace PyUni {
namespace RenderGraph {

/* PyUni::RenderGraph::Node */

Node::Node(Stage *parent):
    _parent(parent)
{
    
}

Node::~Node()
{

}

/* PyUni::RenderGraph::Stage */

Stage::Stage(Stage *parent):
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
