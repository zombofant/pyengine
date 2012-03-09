
#ifndef PYUNI_SCENEGRAPH_SCENEGRAPH_H
#define PYUNI_SCENEGRAPH_SCENEGRAPH_H

#include "Node.hpp"

namespace PyUni {

class SceneGraph
{
    public:
        SceneGraph();
        ~SceneGraph();

        void update();
        void draw();

        inline Node* getRoodNode() const { return root; }

    protected:
        Node *root;
};

}

#endif

