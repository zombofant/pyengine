#include "Style.hpp"

namespace PyEngine {

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

}
