/**********************************************************************
File name: Spatial.cpp
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
#include "Spatial.hpp"

#include <iostream>
#include <GL/glew.h>

namespace PyUni {
namespace SceneGraph {

Spatial::Spatial():
    _parent()
{
}

Spatial::~Spatial()
{
}

SpatialHandle Spatial::getParent()
{
    return _parent.lock();
}

void Spatial::setParent(SpatialHandle s)
{
    _parent = s;
}

void Spatial::updateGeometry(bool initiator)
{
    updateWorldData();
}

void Spatial::updateWorldData()
{
    SpatialHandle p = getParent();

    if(p.get())
    {
        worldTransformation = localTransformation*p->worldTransformation;
    }
    else
    {
        worldTransformation = localTransformation;
    }
}

void Spatial::translate(double x, double y, double z)
{
    localTransformation = localTransformation * Matrix4().Translation(Vector3(x, y, z));
}

void Spatial::setTranslation(double x, double y, double z)
{
    localTransformation = localTransformation.Translation(Vector3(x,y,z));
}

void Spatial::rotate(double angle, double x, double y, double z)
{
    localTransformation = localTransformation * Matrix4().Rotation(Vector3(x, y, z), angle);
}

void Spatial::setRotation(double angle, double x, double y, double z)
{
    localTransformation = localTransformation.Rotation(Vector3(x, y, z), angle);
}

void Spatial::scale(double x, double y, double z)
{
    localTransformation = localTransformation * Matrix4().Scale(Vector3(x, y, z));
}

void Spatial::setScale(double x, double y, double z)
{
    localTransformation = localTransformation.Scale(Vector3(x, y, z));
}

void Spatial::resetTransformation()
{
    localTransformation = Matrix4();
}

void Spatial::applyTransformation()
{
    glLoadMatrixd(worldTransformation.coeff);
}

}
}

