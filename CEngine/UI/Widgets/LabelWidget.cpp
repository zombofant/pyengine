/**********************************************************************
File name: LabelWidget.cpp
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
#include "LabelWidget.hpp"

namespace PyEngine {

/* PyEngine::LabelledWidget */

LabelledWidget::LabelledWidget():
    Widget(),
    _label_rect(NotARect),
    _label(this)
{

}

void LabelledWidget::do_align()
{
    Style &style = computed_style();
    _label_rect = get_absolute_rect();
    _label_rect.shrink(style.padding());
    _label_rect.shrink(style.border().get_box());

    _label.set_width(_label_rect.get_width());
    _label.set_height(_label_rect.get_height());
}

coord_dimensions_t LabelledWidget::get_dimensions()
{
    Style &style = computed_style();
    coord_dimensions_t label_dim = _label.get_dimensions();
    coord_int_t width, height;
    std::tie(width, height) = label_dim;

    if (!Auto::is_auto(style.get_width())) {
        width = style.get_width();
    }
    if (!Auto::is_auto(style.get_height())) {
        height = style.get_height();
    }

    Box border_box = style.border().get_box();
    width += style.padding().get_horizontal() +
             border_box.get_horizontal();
    height += style.padding().get_vertical() +
              border_box.get_vertical();

    return coord_dimensions_t(width, height);
}

void LabelledWidget::invalidate_computed_style()
{
    Widget::invalidate_computed_style();
    _label.invalidate_layout();
}

void LabelledWidget::invalidate_context()
{
    Widget::invalidate_context();
    _label.invalidate_context();
}

void LabelledWidget::render()
{
    Widget::render();
    _label.render(_label_rect);
}

/* PyEngine::LabelWidget */

LabelWidget::LabelWidget():
    LabelledWidget()
{

}

LabelWidget::LabelWidget(const std::string &text):
    LabelledWidget()
{
    set_text(text);
}

void LabelWidget::set_text(const std::string &value)
{
    _label.set_text(value);
}

}
