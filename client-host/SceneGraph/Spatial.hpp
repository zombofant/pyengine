
#ifndef PYUNI_SCENEGRAPH_SPATIAL_H
#define PYUNI_SCENEGRAPH_SPATIAL_H

#include "Matrices.hpp"

namespace PyUni {

class Spatial
{
    public:
        virtual ~Spatial();

        //Bound worldBound;
        Matrix4 localTransformation;
        Matrix4 worldTransformation;

        void updateGeometry(bool initiator=true);
        inline Spatial* getParent() { return parent; }

    protected:
        Spatial();

        virtual void updateWorldData();

        Spatial *parent;
};

}

#endif

