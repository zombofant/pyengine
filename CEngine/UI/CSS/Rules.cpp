/**********************************************************************
File name: Rules.cpp
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
#include "Rules.hpp"

namespace PyEngine { namespace UI {

/* PyEngine::UI::MarginRule */

void MarginRule::apply_to(Style &to) const
{
    apply_to_box(to.margin());
}

/* PyEngine::UI::PaddingRule */

void PaddingRule::apply_to(Style &to) const
{
    apply_to_box(to.padding());
}

/* PyEngine::UI::BackgroundRule */

void BackgroundRule::apply_to(Style &to) const
{
    if (has_effect()) {
        to.set_background(value());
    }
}

/* PyEngine::UI::TextColourRule */

void TextColourRule::apply_to(Style &to) const
{
    if (has_effect()) {
        to.set_text_colour(value());
    }
}

/* PyEngine::UI::BoxSpacingXRule */

void BoxSpacingXRule::apply_to(Style &to) const
{
    if (has_effect()) {
        to.set_box_spacing_x(value());
    }
}

/* PyEngine::UI::BoxSpacingYRule */

void BoxSpacingYRule::apply_to(Style &to) const
{
    if (has_effect()) {
        to.set_box_spacing_y(value());
    }
}

/* PyEngine::UI::FlexRule */

void FlexRule::apply_to(Style &to) const
{
    if (has_effect()) {
        to.set_flex(value());
    }
}

/* PyEngine::UI::FontFamilyRule */

FontFamilyRule::FontFamilyRule():
    _is_set(false),
    _value()
{

}

void FontFamilyRule::apply_to(Style &to) const
{
    if (has_effect()) {
        to.set_font_family(_value);
    }
}

bool FontFamilyRule::has_effect() const
{
    return _is_set;
}

void FontFamilyRule::set(const std::string &value)
{
    _is_set = true;
    _value = value;
}

const std::string& FontFamilyRule::value() const
{
    return _value;
}

void FontFamilyRule::unset()
{
    _is_set = false;
}

/* PyEngine::UI::FontSizeRule */

void FontSizeRule::apply_to(Style &to) const
{
    if (has_effect()) {
        to.set_font_size(value());
    }
}

/* PyEngine::UI::FontSizeRule */

void HeightRule::apply_to(Style &to) const
{
    if (has_effect()) {
        to.set_height(value());
    }
}

/* PyEngine::UI::WidthRule */

void WidthRule::apply_to(Style &to) const
{
    if (has_effect()) {
        to.set_width(value());
    }
}

/* PyEngine::UI::FontWeightRule */

void FontWeightRule::apply_to(Style &to) const
{
    if (has_effect()) {
        to.set_font_weight(value());
    }
}

/* PyEngine::UI::TextAlignRule */

void TextAlignRule::apply_to(Style &to) const
{
    if (has_effect()) {
        to.set_text_align(value());
    }
}

/* PyEngine::UI::BorderRule */

BorderRule::BorderRule():
    AbstractRule(),
    _top_width(),
    _left_width(),
    _right_width(),
    _bottom_width(),
    _top_fill(),
    _left_fill(),
    _right_fill(),
    _bottom_fill(),
    _tl_radius(),
    _tr_radius(),
    _bl_radius(),
    _br_radius()
{

}

void BorderRule::apply_edge(const int_value &width,
                            const fill_value &fill,
                            BorderEdge &edge) const
{
    if (width.is_set) {
        edge.set_width(width.value);
    }
    if (fill.is_set) {
        edge.set_fill(fill.value);
    }
}

void BorderRule::set_top_width(const css_coord_int_t &value)
{
    _top_width.set(value);
}

void BorderRule::set_left_width(const css_coord_int_t &value)
{
    _left_width.set(value);
}

void BorderRule::set_right_width(const css_coord_int_t &value)
{
    _right_width.set(value);
}

void BorderRule::set_bottom_width(const css_coord_int_t &value)
{
    _bottom_width.set(value);
}

void BorderRule::set_all_widths(const css_coord_int_t &value)
{
    _top_width.set(value);
    _left_width.set(value);
    _right_width.set(value);
    _bottom_width.set(value);
}

void BorderRule::set_top_fill(const FillPtr &value)
{
    _top_fill.set(value);
}

void BorderRule::set_left_fill(const FillPtr &value)
{
    _left_fill.set(value);
}

void BorderRule::set_right_fill(const FillPtr &value)
{
    _right_fill.set(value);
}

void BorderRule::set_bottom_fill(const FillPtr &value)
{
    _bottom_fill.set(value);
}

void BorderRule::set_all_fills(const FillPtr &value)
{
    _top_fill.set(value);
    _left_fill.set(value);
    _right_fill.set(value);
    _bottom_fill.set(value);
}

void BorderRule::set_top_left_radius(const css_coord_int_t &value)
{
    _tl_radius.set(value);
}

void BorderRule::set_top_right_radius(const css_coord_int_t &value)
{
    _tr_radius.set(value);
}

void BorderRule::set_bottom_left_radius(const css_coord_int_t &value)
{
    _bl_radius.set(value);
}

void BorderRule::set_bottom_right_radius(const css_coord_int_t &value)
{
    _br_radius.set(value);
}

void BorderRule::set_all_radii(const css_coord_int_t &value)
{
    _tl_radius.set(value);
    _tr_radius.set(value);
    _bl_radius.set(value);
    _br_radius.set(value);
}

void BorderRule::apply_to(Style &to) const
{
    Border &border = to.border();
    apply_edge(_top_width, _top_fill, border.top());
    apply_edge(_left_width, _left_fill, border.left());
    apply_edge(_right_width, _right_fill, border.right());
    apply_edge(_bottom_width, _bottom_fill, border.bottom());

    if (_tl_radius.is_set) {
        border.set_top_left_radius(_tl_radius.value);
    }
    if (_tr_radius.is_set) {
        border.set_top_right_radius(_tr_radius.value);
    }
    if (_bl_radius.is_set) {
        border.set_bottom_left_radius(_bl_radius.value);
    }
    if (_br_radius.is_set) {
        border.set_bottom_right_radius(_br_radius.value);
    }
}

bool BorderRule::has_effect() const
{
    return (
        _top_width.is_set ||
        _top_fill.is_set ||
        _left_width.is_set ||
        _left_fill.is_set ||
        _right_width.is_set ||
        _right_fill.is_set ||
        _bottom_width.is_set ||
        _bottom_fill.is_set ||
        _tl_radius.is_set ||
        _tr_radius.is_set ||
        _bl_radius.is_set ||
        _br_radius.is_set);
}

/* PyEngine::UI::VerticalAlignRule */

void VerticalAlignRule::apply_to(Style &to) const
{
    if (has_effect()) {
        to.set_vertical_align(value());
    }
}

}
}
