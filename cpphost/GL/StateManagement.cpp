/**********************************************************************
File name: StateManagement.cpp
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
#include "StateManagement.hpp"

namespace PyUni {
namespace GL {

/* PyUni::GL::Group */

Group::Group(const GroupHandle parent, int order):
    _parent(parent),
    _order(order)
{

}

int Group::compare(const Group &other) const
{
    if (_parent)
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
    }
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

}
}
