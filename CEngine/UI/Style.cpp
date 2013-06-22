/**********************************************************************
File name: Style.cpp
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
#include "Style.hpp"

namespace PyEngine {

template <typename A>
bool cmp_css_ptr(
    const CSSInheritable<A>& a,
    const CSSInheritable<A>& b)
{
    if (a.is_inherit() || b.is_inherit()) {
        return false;
    }
    return *a.get() == *b.get();
}

/* PyEngine::Style */

Style::Style():
    _background(new Transparent()),
    _padding(0),
    _margin(0),
    _border(0, CSSFillPtr(new Transparent())),
    _box_spacing_x(0),
    _box_spacing_y(0),
    _width(Auto()),
    _height(Auto()),
    _flex(0),
    _text_colour(Inherit),
    _text_align(Inherit),
    _font_weight(Inherit),
    _font_family(Inherit),
    _font_size(Inherit),
    _ellipsize(Inherit),
    _vertical_align(Inherit),
    _shear_x(0),
    _shear_y(0),
    _font_cache(nullptr)
{

}

Style::Style(const Style& ref):
    _background(ref._background),
    _padding(ref._padding),
    _margin(ref._margin),
    _border(ref._border),
    _box_spacing_x(ref._box_spacing_x),
    _box_spacing_y(ref._box_spacing_y),
    _width(ref._width),
    _height(ref._height),
    _flex(ref._flex),
    _text_colour(ref._text_colour),
    _text_align(ref._text_align),
    _font_weight(ref._font_weight),
    _font_family(ref._font_family),
    _font_size(ref._font_size),
    _ellipsize(ref._ellipsize),
    _vertical_align(ref._vertical_align),
    _shear_x(ref._shear_x),
    _shear_y(ref._shear_y),
    _font_cache(pango_font_description_copy(ref._font_cache))
{

}

Style& Style::operator=(const Style& ref)
{
    _background = ref._background;
    _padding = ref._padding;
    _margin = ref._margin;
    _border = ref._border;
    _box_spacing_x = ref._box_spacing_x;
    _box_spacing_y = ref._box_spacing_y;
    _width = ref._width;
    _height = ref._height;
    _flex = ref._flex;
    _text_colour = ref._text_colour;
    _text_align = ref._text_align;
    _font_weight = ref._font_weight;
    _font_family = ref._font_family;
    _font_size = ref._font_size;
    _ellipsize = ref._ellipsize;
    _vertical_align = ref._vertical_align;
    _shear_x = ref._shear_x;
    _shear_y = ref._shear_y;
    _font_cache = pango_font_description_copy(ref._font_cache);
    return *this;
}

Style::~Style()
{
    if (_font_cache) {
        pango_font_description_free(_font_cache);
        _font_cache = nullptr;
    }
}

StyleDiff Style::calc_diff(const Style& other) const
{
    StyleDiff diffs;
    if ((_background != other._background) ||
        (_text_colour != other._text_colour) ||
        (_shear_x != other._shear_x) ||
        (_shear_y != other._shear_y) ||
        (_border != other._border) ||
        (_font_size != other._font_size) ||
        (_font_weight != other._font_weight) ||
        (_font_family != other._font_family) ||
        (_text_align != other._text_align) ||
        (_vertical_align != other._vertical_align))
    {
        diffs.set(SD_VISUAL);
    }

    if ((_border != other._border) ||
        (_width != other._width) ||
        (_height != other._height) ||
        (_flex != other._flex) ||
        (_box_spacing_x != other._box_spacing_x) ||
        (_box_spacing_y != other._box_spacing_y) ||
        (_margin != other._margin) ||
        (_padding != other._padding))
    {
        diffs.set(SD_LAYOUT);
    }

    return std::move(diffs);
}

void Style::deinherit_with(const Style &other_style)
{
    _background.deinherit_with(
        other_style.get_background());
    _padding.deinherit_with(
        other_style._padding);
    _margin.deinherit_with(
        other_style._margin);
    _border.deinherit_with(
        other_style._border);
    _box_spacing_x.deinherit_with(
        other_style.get_box_spacing_x());
    _box_spacing_y.deinherit_with(
        other_style.get_box_spacing_y());
    _width.deinherit_with(
        other_style.get_width());
    _height.deinherit_with(
        other_style.get_height());
    _flex.deinherit_with(
        other_style.get_flex());
    _text_align.deinherit_with(
        other_style.get_text_align());
    _font_weight.deinherit_with(
        other_style.get_font_weight());
    _font_family.deinherit_with(
        other_style.get_font_family());
    _font_size.deinherit_with(
        other_style.get_font_size());
    _ellipsize.deinherit_with(
        other_style.get_ellipsize());
    _vertical_align.deinherit_with(
        other_style.get_vertical_align());
    _shear_x.deinherit_with(
        other_style.get_shear_x());
    _shear_y.deinherit_with(
        other_style.get_shear_y());
    _invalidate_font();
}

bool Style::operator==(const Style &other)
{
    return (
        cmp_css_ptr(_background, other._background) &&
        (_padding == other._padding) &&
        (_margin == other._margin) &&
        (_border == other._border) &&
        (_box_spacing_x == other._box_spacing_x) &&
        (_box_spacing_y == other._box_spacing_y) &&
        (_width == other._width) &&
        (_height == other._height) &&
        (_flex == other._flex) &&
        (_text_align == other._text_align) &&
        (_font_weight == other._font_weight) &&
        (_font_family == other._font_family) &&
        (_font_size == other._font_size) &&
        (_ellipsize == other._ellipsize) &&
        (_vertical_align == other._vertical_align) &&
        (_shear_x == other._shear_x) &&
        (_shear_y == other._shear_y));
}

/* free functions */

Style DefaultStyle()
{
    Style result;
    result.set_text_colour(FillPtr(new Colour(0, 0, 0, 1)));
    result.set_text_align(PANGO_ALIGN_LEFT);
    result.set_font_weight(PANGO_WEIGHT_NORMAL);
    result.set_font_family("sans");
    result.set_font_size(12 * PANGO_SCALE);
    result.set_ellipsize(PANGO_ELLIPSIZE_END);
    result.set_vertical_align(CSS_VERTICAL_ALIGN_TOP);
    return std::move(result);
}

}