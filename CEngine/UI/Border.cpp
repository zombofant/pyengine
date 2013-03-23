/**********************************************************************
File name: Border.cpp
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
#include "Border.hpp"

namespace PyEngine {

/* PyEngine::BorderComponent */

BorderComponent::~BorderComponent()
{

}

/* PyEngine::BorderEdge */

BorderEdge::BorderEdge():
    _width(0),
    _fill()
{

}

BorderEdge::BorderEdge(css_coord_int_t width,
                       CSSFillPtr fill):
    _width(width),
    _fill(fill)
{
    if (!width.is_inherit() && _width < 0) {
        throw std::invalid_argument("Border edge width must be non-negative.");
    }
}

BorderEdge::BorderEdge(const BorderComponent& ref):
    _width(ref.get_width()),
    _fill(ref.get_fill())
{

}

BorderEdge::BorderEdge(const BorderEdge& ref):
    _width(ref.get_width()),
    _fill(ref.get_fill())
{

}

BorderEdge& BorderEdge::operator=(const BorderComponent& ref)
{
    _width = ref.get_width();
    _fill = ref.get_fill();
    return *this;
}

BorderEdge& BorderEdge::operator=(const BorderEdge& ref)
{
    _width = ref._width;
    _fill = ref._fill;
    return *this;
}

BorderEdge::~BorderEdge()
{

}

CSSFillPtr BorderEdge::get_fill() const
{
    return _fill;
}

css_coord_int_t BorderEdge::get_width() const
{
    return _width;
}

void BorderEdge::set_fill(const CSSFillPtr& value)
{
    _fill = value;
}

void BorderEdge::set_width(const css_coord_int_t& value)
{
    _width = value;
}

/* PyEngine::Border */

Border::Border():
    _edges(),
    _corners({{0, 0, 0, 0}})
{

}

Border::Border(css_coord_int_t width, CSSFillPtr fill):
    _edges(),
    _corners({{0, 0, 0, 0}})
{
    for (auto edge: _edges) {
        edge.set_width(width);
        edge.set_fill(fill);
    }
}

Border::Border(const BorderComponent& ref):
    _edges(),
    _corners({{0, 0, 0, 0}})
{
    *this = ref;
}

Border::Border(const Border& ref):
    _edges(ref._edges),
    _corners(ref._corners)
{

}

Border& Border::operator=(const BorderComponent& ref)
{
    const css_coord_int_t width = ref.get_width();
    const CSSFillPtr fill = ref.get_fill();
    for (auto edge: _edges) {
        edge.set_width(width);
        edge.set_fill(fill);
    }
    _corners.fill(0);

    return *this;
}

Border& Border::operator=(const Border& ref)
{
    _edges = ref._edges;
    _corners = ref._corners;
    return *this;
}

Border::~Border()
{

}

CSSFillPtr Border::get_fill() const
{
    throw std::logic_error("Cannot determine fill of Border.");
}

css_coord_int_t Border::get_width() const
{
    throw std::logic_error("Cannot determine width of Border.");
}

void Border::set_fill(const CSSFillPtr& value)
{
    for (auto edge: _edges) {
        edge.set_fill(value);
    }
}

void Border::set_radius(const css_coord_int_t& value)
{
    _corners.fill(value);
}

void Border::set_width(const css_coord_int_t& value)
{
    for (auto edge: _edges) {
        edge.set_width(value);
    }
}

Box Border::get_box() const
{
    return Box(
        _edges[0]._width,
        _edges[2]._width,
        _edges[1]._width,
        _edges[3]._width);
}

FloatBox Border::get_half_box() const
{
    return FloatBox(
        (coord_float_t)_edges[0]._width / 2,
        (coord_float_t)_edges[2]._width / 2,
        (coord_float_t)_edges[1]._width / 2,
        (coord_float_t)_edges[3]._width / 2);
}

}
