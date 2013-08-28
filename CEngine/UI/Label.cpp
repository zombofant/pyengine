/**********************************************************************
File name: Label.cpp
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
#include "Label.hpp"

#include <pango/pangocairo.h>

#include "CEngine/UI/CSS/Style.hpp"

namespace PyEngine { namespace UI {

/* PyEngine::UI::Label */

Label::Label(Widget *widget):
    _widget(widget),
    _width(-1),
    _height(-1),
    _context_invalidated(true),
    _layout_invalidated(true),
    _text(),
    _justify(false),
    _cairo(nullptr),
    _pango(nullptr),
    _layout(nullptr)
{

}

Label::~Label()
{
    if (_layout) {
        g_object_unref(_layout);
    }
}

void Label::update_context()
{
    if (!_context_invalidated) {
        return;
    }
    assert(_widget);
    _cairo = _widget->get_cairo_context();
    _pango = _widget->get_pango_context();
    if (_layout) {
        g_object_unref(_layout);
    }
    _layout = nullptr;

    _context_invalidated = false;
    invalidate_layout();
}

void Label::update_layout()
{
    update_context();

    if (!_layout_invalidated) {
        return;
    }

    if (!_layout) {
        _layout = pango_layout_new(_pango);
    }

    coord_int_t width = (_width < 0 ? _width : _width * PANGO_SCALE);
    coord_int_t height = (_height < 0 ? _height : _height * PANGO_SCALE);

    pango_layout_set_width(_layout, width);
    pango_layout_set_height(_layout, height);

    Style &style = _widget->computed_style();
    pango_layout_set_alignment(_layout, style.get_text_align());
    pango_layout_set_ellipsize(_layout, style.get_ellipsize());
    pango_layout_set_justify(_layout, _justify);
    pango_layout_set_wrap(_layout, PANGO_WRAP_WORD);
    pango_layout_set_font_description(_layout, style.get_font());
    pango_layout_set_text(_layout, _text.c_str(), _text.length());

    _layout_invalidated = false;
}

coord_dimensions_t Label::get_dimensions()
{
    update_layout();
    PangoRectangle logical;
    pango_layout_get_pixel_extents(_layout, nullptr, &logical);
    return coord_dimensions_t(logical.width, logical.height);
}

void Label::invalidate_context()
{
    _context_invalidated = true;
}

void Label::invalidate_layout()
{
    _layout_invalidated = true;
}

void Label::render(const Rect &in_box)
{
    update_layout();
    const Style &style = _widget->computed_style();
    PangoRectangle logical;
    pango_layout_get_pixel_extents(_layout, nullptr, &logical);

    /* XXX: Pango docs say, we shall take into account the x and y
     * values of the extent. If we do, however, layout positioning
     * gets all wrong!! WTF?
     * <http://developer.gnome.org/pango/stable/pango-Layout-Objects.html#pango-layout-get-pixel-extents> */

    const coord_int_t x = in_box.get_left();
    const coord_int_t y = in_box.get_top() +
        Style::apply_vertical_align(style.get_vertical_align(),
                                    logical.height,
                                    in_box.get_height());

    cairo_translate(_cairo, x, y);
    style.get_text_colour().get()->set_source(_cairo, in_box);
    pango_cairo_show_layout(_cairo, _layout);
    cairo_translate(_cairo, -x, -y);
}

void Label::set_height(coord_int_t value)
{
    value = (value < 0 ? -1 : value);
    if (value == _height) {
        return;
    }

    _height = value;
    invalidate_layout();
}

void Label::set_text(const std::string &value)
{
    if (_text == value) {
        return;
    }
    _text = value;
    invalidate_layout();
}

void Label::set_width(coord_int_t value)
{
    value = (value < 0 ? -1 : value);
    if (value == _width) {
        return;
    }

    _width = value;
    invalidate_layout();
}

}
}
