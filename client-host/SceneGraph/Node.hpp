
#ifndef PYUNI_SCENEGRAPH_NODE_H
#define PYUNI_SCENEGRAPH_NODE_H

#include <vector>

#include "Spatial.hpp"

namespace PyUni {

class Node : public Spatial
{
    public:
        Node();
        ~Node();

        void addChild(Spatial *child);
        void removeChild(Spatial *child);

    protected:
        virtual void updateWorldData();

        std::vector<Spatial*> children; 
};

}

#endif

