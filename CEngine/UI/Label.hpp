/**********************************************************************
File name: Label.hpp
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
#ifndef _PYE_UI_LABEL_H
#define _PYE_UI_LABEL_H

#include <cairo/cairo.h>
#include <pango/pango.h>

#include "CEngine/UI/Widgets/WidgetBase.hpp"

namespace PyEngine { namespace UI {

class Label
{
public:
    Label(Widget *widget);
    ~Label();

private:
    Widget *_widget;
    coord_int_t _width, _height;
    bool _context_invalidated, _layout_invalidated;
    std::string _text;
    bool _justify;
    cairo_t *_cairo;
    PangoContext *_pango;
    PangoLayout *_layout;

protected:
    void update_context();
    void update_layout();

public:
    coord_dimensions_t get_dimensions();
    void invalidate_context();
    void invalidate_layout();
    void render(const Rect &in_box);

public:
    inline coord_int_t get_height() const {
        return _height;
    };

    inline const std::string& get_text() const {
        return _text;
    };

    inline coord_int_t get_width() const {
        return _width;
    };

    inline Widget* get_widget() const {
        return _widget;
    };

    void set_height(coord_int_t value);
    void set_text(const std::string &value);
    void set_width(coord_int_t value);
};

}
}

#endif
