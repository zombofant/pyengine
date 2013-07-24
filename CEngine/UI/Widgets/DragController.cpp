/**********************************************************************
File name: DragController.cpp
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
#include "DragController.hpp"

#include "RootWidget.hpp"

namespace PyEngine {

/* PyEngine::DragControllerBase */

DragControllerBase::DragControllerBase(
        RootWidget *root_widget,
        unsigned int mouse_button):
    _root_widget(root_widget),
    _mouse_button(mouse_button)
{

}

DragControllerBase::~DragControllerBase()
{

}

void DragControllerBase::abort()
{
    release();
}

void DragControllerBase::release()
{
    _root_widget->stop_drag();
}

void DragControllerBase::ev_key_down(
    Key::Key key,
    KeyModifiers modifiers)
{
    if (key == Key::Escape) {
        abort();
    }
}

void DragControllerBase::ev_key_up(
    Key::Key key,
    KeyModifiers modifiers)
{

}

void DragControllerBase::ev_mouse_down(
    int x, int y,
    MouseButton button,
    KeyModifiers modifiers)
{

}

void DragControllerBase::ev_mouse_move(
    int x, int y,
    MouseButtons buttons,
    KeyModifiers modifiers)
{
    ev_drag_move(x, y, modifiers);
}

void DragControllerBase::ev_mouse_up(
    int x, int y,
    MouseButton button,
    KeyModifiers modifiers)
{
    if (button == _mouse_button) {
        release();
    }
}

void DragControllerBase::ev_drag_move(
    int x, int y,
    KeyModifiers modifiers)
{

}

/* PyEngine::DragMoveWidget */

DragMoveWidget::DragMoveWidget(
        RootWidget *root_widget,
        unsigned int mouse_button,
        int x, int y,
        const WidgetPtr &widget):
    DragControllerBase(root_widget, mouse_button),
    _startx(x),
    _starty(y),
    _widget(widget),
    _startxoffs(0),
    _startyoffs(0)
{
    const Rect &widget_rect = _widget->get_absolute_rect();
    _startxoffs = _startx - widget_rect.get_x();
    _startyoffs = _starty - widget_rect.get_y();
}

void DragMoveWidget::ev_widget_move(int x, int y)
{
    Rect &widget_rect = _widget->absolute_rect();
    _widget->invalidate();
    widget_rect.set_x(x);
    widget_rect.set_y(y);
    _widget->invalidate();
}

void DragMoveWidget::ev_drag_move(int x, int y, KeyModifiers modifiers)
{
    int px = x - _startx;
    int py = y - _starty;
    ev_widget_move(px, py);
}


}
