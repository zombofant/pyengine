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

Rect::Rect(coord_int_t x, coord_int_t y):
    GenericRect<coord_int_t>(x, y)
{

}

Rect::Rect(coord_int_t x, coord_int_t y,
           coord_int_t width, coord_int_t height):
    GenericRect<coord_int_t>(x, y, width, height)
{

}

Rect::Rect(const Rect &ref):
    GenericRect<coord_int_t>(ref)
{

}

Rect::Rect(const not_a_rect_t&):
    GenericRect<coord_int_t>(NotARect)
{

}

Rect& Rect::operator= (const Rect &ref)
{
    this->GenericRect<coord_int_t>::operator=(ref);
    return *this;
}

Rect& Rect::operator=(const not_a_rect_t&)
{
    this->GenericRect<coord_int_t>::operator=(NotARect);
    return *this;
}

/* PyEngine::CSSBox */

CSSBox::CSSBox():
    GenericBox<css_coord_int_t>()
{

}

CSSBox::CSSBox(css_coord_int_t value):
    GenericBox<css_coord_int_t>(value)
{

}

CSSBox::CSSBox(css_coord_int_t left, css_coord_int_t top,
               css_coord_int_t right, css_coord_int_t bottom):
    GenericBox<css_coord_int_t>(left, top, right, bottom)
{

}

CSSBox::CSSBox(const CSSBox& ref):
    GenericBox<css_coord_int_t>(ref)
{

}

CSSBox::CSSBox(const Box& ref):
    GenericBox<css_coord_int_t>(
        ref.get_left(),
        ref.get_top(),
        ref.get_right(),
        ref.get_bottom())
{

}

CSSBox& CSSBox::operator=(const CSSBox& ref)
{
    this->GenericBox<css_coord_int_t>::operator=(ref);
    return *this;
}

CSSBox& CSSBox::operator=(const Box& ref)
{
    _left = ref.get_left();
    _top = ref.get_top();
    _right = ref.get_right();
    _bottom = ref.get_bottom();
    return *this;
}

void CSSBox::deinherit_with(const CSSBox& other)
{
    _left.deinherit_with(other.get_left());
    _top.deinherit_with(other.get_top());
    _right.deinherit_with(other.get_right());
    _bottom.deinherit_with(other.get_bottom());
}

/* PyEngine::Margin */

Margin::Margin():
    CSSBox()
{

}

Margin::Margin(css_coord_int_t value):
    CSSBox(value)
{

}

Margin::Margin(css_coord_int_t left, css_coord_int_t top,
               css_coord_int_t right, css_coord_int_t bottom):
    CSSBox(left, top, right, bottom)
{

}

Margin::Margin(const Margin& ref):
    CSSBox(ref)
{

}

Margin::Margin(const CSSBox& ref):
    CSSBox(ref)
{

}

Margin::Margin(const Box& ref):
    CSSBox(ref)
{

}

Margin& Margin::operator=(const Margin& ref)
{
    this->CSSBox::operator=(ref);
    return *this;
}

Margin& Margin::operator=(const CSSBox& ref)
{
    this->CSSBox::operator=(ref);
    return *this;
}

Margin& Margin::operator=(const Box& ref)
{
    this->CSSBox::operator=(ref);
    return *this;
}

void Margin::_check_value(const css_coord_int_t value) const
{

}

}
