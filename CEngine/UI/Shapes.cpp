/**********************************************************************
File name: Shapes.cpp
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
#include "Shapes.hpp"

namespace PyEngine {

/* PyEngine::BoxError */

BoxError::BoxError(const std::string& what_arg):
    std::invalid_argument(what_arg)
{

}

BoxError::BoxError(const char* what_arg):
    std::invalid_argument(what_arg)
{

}

/* PyEngine::RectError */

RectError::RectError(const std::string& what_arg):
    std::invalid_argument(what_arg)
{

}

RectError::RectError(const char* what_arg):
    std::invalid_argument(what_arg)
{

}

/* PyEngine::Point */

Point::Point():
    GenericPoint<coord_int_t>()
{

}

Point::Point(coord_int_t x, coord_int_t y):
    GenericPoint<coord_int_t>(x, y)
{

}

Point::Point(const Point& ref):
    GenericPoint<coord_int_t>(ref)
{

}

Point& Point::operator=(const Point& ref)
{
    this->GenericPoint::operator=(ref);
    return *this;
}

/* PyEngine::Box */

Box::Box():
    GenericBox<coord_int_t>()
{

}

Box::Box(coord_int_t value):
    GenericBox<coord_int_t>(value)
{

}

Box::Box(coord_int_t left, coord_int_t right,
         coord_int_t top, coord_int_t bottom):
    GenericBox<coord_int_t>(left, right, top, bottom)
{

}

Box::Box(const Box& ref):
    GenericBox<coord_int_t>(ref)
{

}

Box& Box::operator= (const Box& ref)
{
    this->GenericBox<coord_int_t>::operator=(ref);
    return *this;
}

void Box::_check_value(const coord_int_t value) const
{
    if (value == (coord_int_t)Auto()) {
        throw BoxError("Non-margin boxes do not support AUTO value.");
    }
    if (value < 0) {
        throw BoxError("Non-margin boxes do not allow negative values.");
    }
}

/* PyEngine::FloatBox */

FloatBox::FloatBox():
    GenericBox<coord_float_t>()
{

}

FloatBox::FloatBox(coord_float_t value):
    GenericBox<coord_float_t>(value)
{

}

FloatBox::FloatBox(coord_float_t left, coord_float_t right,
         coord_float_t top, coord_float_t bottom):
    GenericBox<coord_float_t>(left, right, top, bottom)
{

}

FloatBox::FloatBox(const FloatBox& ref):
    GenericBox<coord_float_t>(ref)
{

}

FloatBox& FloatBox::operator=(const FloatBox& ref)
{
    this->GenericBox<coord_float_t>::operator=(ref);
    return *this;
}

void FloatBox::_check_value(const coord_float_t value) const
{
    if (value == (coord_float_t)Auto()) {
        throw BoxError("Non-margin boxes do not support AUTO value.");
    }
    if (value < 0) {
        throw BoxError("Non-margin boxes do not allow negative values.");
    }
}

bool FloatBox::operator==(const Box& b) const
{
    return ((_left == b.get_left()) &&
            (_top == b.get_top()) &&
            (_right == b.get_right()) &&
            (_bottom == b.get_bottom()));
}

bool FloatBox::operator!=(const Box& b) const
{
    return !(operator==(b));
}

/* PyEngine::Rect */

Rect::Rect():
    GenericRect<coord_int_t>()
{

}

Rect::Rect(coord_int_t top, coord_int_t left):
    GenericRect<coord_int_t>(top, left)
{

}

Rect::Rect(coord_int_t left, coord_int_t top,
           coord_int_t right, coord_int_t bottom):
    GenericRect<coord_int_t>(left, top, right, bottom)
{

}

Rect::Rect(const Rect &ref):
    GenericRect<coord_int_t>(ref)
{

}

Rect& Rect::operator= (const Rect &ref)
{
    this->GenericRect<coord_int_t>::operator=(ref);
    return *this;
}


/* PyEngine::Margin */

Margin::Margin():
    Box::Box()
{

}

Margin::Margin(coord_int_t value):
    Box::Box(value)
{

}

Margin::Margin(coord_int_t left, coord_int_t top,
               coord_int_t right, coord_int_t bottom):
    Box::Box(left, top, right, bottom)
{

}

Margin::Margin(const Box& ref):
    Box::Box(ref)
{

}

Margin::Margin(const Margin& ref):
    Box::Box(ref)
{

}

Margin& Margin::operator= (const Margin& ref)
{
    this->Box::operator=(ref);
    return *this;
}

Margin& Margin::operator= (const Box& ref)
{
    this->Box::operator=(ref);
    return *this;
}

}
