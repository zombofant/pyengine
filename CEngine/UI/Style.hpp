/**********************************************************************
File name: Style.hpp
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
#ifndef _PYE_UI_STYLE_H
#define _PYE_UI_STYLE_H

#include <string>
#include <bitset>
#include <limits>

#include <pango/pango.h>

#include "CSS.hpp"
#include "Fill.hpp"
#include "Border.hpp"
#include "Shapes.hpp"

namespace PyEngine {

enum VerticalAlign {
    CSS_VERTICAL_ALIGN_TOP,
    CSS_VERTICAL_ALIGN_MIDDLE,
    CSS_VERTICAL_ALIGN_BOTTOM
};

enum StyleDiffOption {
    SD_LAYOUT,
    SD_VISUAL
};

typedef std::bitset<std::numeric_limits<StyleDiffOption>::max()> StyleDiff;

class Style
{
public:
    Style();
    Style(const Style& ref);
    Style& operator=(const Style& ref);
    ~Style();

private:
    CSSFillPtr _background;
    CSSBox _padding;
    Margin _margin;
    Border _border;
    css_coord_int_t _box_spacing_x, _box_spacing_y;
    css_coord_int_t _width, _height;
    CSSInheritable<uint32_t> _flex;
    CSSFillPtr _text_colour;
    CSSInheritable<PangoAlignment> _text_align;
    CSSInheritable<PangoWeight> _font_weight;
    CSSInheritable<std::string> _font_family;
    CSSInheritable<uint32_t> _font_size;
    CSSInheritable<PangoEllipsizeMode> _ellipsize;
    CSSInheritable<VerticalAlign> _vertical_align;
    css_coord_int_t _shear_x, _shear_y;
    PangoFontDescription* _font_cache;

protected:
    inline void _invalidate_font() {
        _font_cache = nullptr;
    };

public:
    StyleDiff calc_diff(const Style& other) const;

    inline const CSSFillPtr& get_background() const {
        return _background;
    };
    inline CSSBox& padding() {
        return _padding;
    };
    inline Margin& margin() {
        return _margin;
    };
    inline Border& border() {
        return _border;
    };
    inline const css_coord_int_t& get_box_spacing_x() const {
        return _box_spacing_x;
    };
    inline const css_coord_int_t& get_box_spacing_y() const {
        return _box_spacing_y;
    };
    inline const css_coord_int_t& get_width() const {
        return _width;
    };
    inline const css_coord_int_t& get_height() const {
        return _height;
    };
    inline const CSSInheritable<uint32_t>& get_flex() const {
        return _flex;
    };
    inline const CSSFillPtr& get_text_colour() const {
        return _text_colour;
    };
    inline const CSSInheritable<PangoAlignment>& get_text_align() const {
        return _text_align;
    };
    inline const CSSInheritable<PangoWeight>& get_font_weight() const {
        return _font_weight;
    };
    inline const CSSInheritable<std::string>& get_font_family() const {
        return _font_family;
    };
    inline const CSSInheritable<uint32_t>& get_font_size() const {
        return _font_size;
    };
    inline const CSSInheritable<PangoEllipsizeMode>& get_ellipsize() const {
        return _ellipsize;
    };
    inline const CSSInheritable<VerticalAlign>& get_vertical_align() const {
        return _vertical_align;
    };
    inline const css_coord_int_t get_shear_x() const {
        return _shear_x;
    };
    inline const css_coord_int_t get_shear_y() const {
        return _shear_y;
    };

    inline void set_background(const CSSFillPtr& value) {
        _background = value;
    };
    inline void set_box_spacing_x(const css_coord_int_t& value) {
        _box_spacing_x = value;
    };
    inline void set_box_spacing_y(const css_coord_int_t& value) {
        _box_spacing_y = value;
    };
    inline void set_flex(const CSSInheritable<uint32_t>& value) {
        _flex = value;
    };
    inline void set_text_colour(const CSSFillPtr& value) {
        _text_colour = value;
    };
    inline void set_text_align(const CSSInheritable<PangoAlignment>& value) {
        _text_align = value;
    };
    inline void set_font_weight(const CSSInheritable<PangoWeight>& value) {
        _invalidate_font();
        _font_weight = value;
    };
    inline void set_font_family(const CSSInheritable<std::string>& value) {
        _invalidate_font();
        _font_family = value;
    };
    inline void set_font_size(const CSSInheritable<uint32_t>& value) {
        _invalidate_font();
        _font_size = value;
    };
    inline void set_ellipsize(const CSSInheritable<PangoEllipsizeMode>& value) {
        _ellipsize = value;
    };
    inline void set_vertical_align(const CSSInheritable<VerticalAlign>& value) {
        _vertical_align = value;
    };
    inline void set_shear_x(const css_coord_int_t& value) {
        _shear_x = value;
    };
    inline void set_shear_y(const css_coord_int_t& value) {
        _shear_y = value;
    };

    PangoFontDescription* get_font();

};

Style DefaultStyle();

}

#endif
