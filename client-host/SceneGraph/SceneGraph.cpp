
#include "SceneGraph.hpp"

namespace PyUni {

SceneGraph::SceneGraph()
{
    root = new Node();
}

SceneGraph::~SceneGraph()
{
    delete root;
}

void SceneGraph::update()
{
    root->updateGeometry();
}

void SceneGraph::draw()
{
    root->onDraw();
    root->draw();
}

}

