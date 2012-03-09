
#include "Spatial.hpp"

namespace PyUni {

Spatial::Spatial() : parent(0)
{
}

Spatial::~Spatial()
{
}

void Spatial::updateGeometry(bool initiator)
{
    updateWorldData();
}

void Spatial::updateWorldData()
{
    if(parent)
    {
        worldTransformation = parent->worldTransformation*localTransformation;
    }
    else
    {
        worldTransformation = localTransformation;
    }
}

void Spatial::onDraw()
{
}

void Spatial::draw()
{
}

}

