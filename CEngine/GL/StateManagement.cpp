/**********************************************************************
File name: StateManagement.cpp
This file is part of: Pythonic Engine

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

For feedback and questions about pyengine please e-mail one of the
authors named in the AUTHORS file.
**********************************************************************/
#include "StateManagement.hpp"

namespace PyEngine {
namespace GL {

/* PyEngine::GL::Group */

Group::Group(int order):
    _order(order),
    _indexBuffer(new StaticIndexBuffer()),
    _ibHandle(_indexBuffer)
{

}

void Group::drawGeometry() const
{
    _indexBuffer->draw(GL_TRIANGLES);
}

int Group::compare(const Group &other) const
{
    /*if (_parent)
    {
        if (other._parent)
        {
            int test = _parent->compare(other._parent);
            if (test != 0)
                return test;
        } else
        {
            return 1;
        }
    }*/
    if (_order > other._order)
    {
        return 1;
    } else if (_order < other._order)
    {
        return -1;
    } else
    {
        return 0;
    }
}

bool Group::operator == (const Group &other) const
{
    return compare(other) == 0;
}

bool Group::operator != (const Group &other) const
{
    return compare(other) != 0;
}

bool Group::operator <  (const Group &other) const
{
    return compare(other) < 0;
}

bool Group::operator <= (const Group &other) const
{
    return compare(other) <= 0;
}

bool Group::operator >  (const Group &other) const
{
    return compare(other) > 0;
}

bool Group::operator >= (const Group &other) const
{
    return compare(other) >= 0;
}

void Group::execute()
{
    drawGeometry();
}

/* PyEngine::GL::ParentGroup */

ParentGroup::ParentGroup(int order):
    Group::Group(order),
    _children()
{

}

void ParentGroup::executeChildren()
{
    for (auto it = _children.begin();
        it != _children.end();
        it++)
    {
        const GroupHandle handle = *it;
        handle->execute();
    }
}

void ParentGroup::add(GroupHandle handle)
{
    _children.push_back(handle);
}

void ParentGroup::remove(GroupHandle handle)
{
    Group *group = handle.get();
    for (auto it = _children.begin();
        it != _children.end();
        it++)
    {
        const GroupHandle cmp = *it;
        if (cmp.get() == group)
        {
            _children.erase(it);
            return;
        }
    }
}

void ParentGroup::execute()
{
    setUp();
    this->Group::execute();
    executeChildren();
    tearDown();
}

void ParentGroup::setUp()
{

}

void ParentGroup::tearDown()
{

}

/* PyEngine::GL::StateGroup */

StateGroup::StateGroup(StructHandle glObject, int order):
    ParentGroup::ParentGroup(order),
    _glObjectHandle(glObject),
    _glObject(_glObjectHandle.get())
{
    
}

void StateGroup::setUp()
{
    _glObject->bind();
}

void StateGroup::tearDown()
{
    _glObject->unbind();
}   

/* PyEngine::GL::TransformGroup */

TransformGroup::TransformGroup(const Matrix4f *matrix, int order):
    ParentGroup::ParentGroup(order),
    _matrix(matrix)
{
    
}

void TransformGroup::setUp()
{
    glLoadMatrixf(_matrix->coeff);
}

void TransformGroup::tearDown()
{
    glLoadIdentity();
}

}
}
