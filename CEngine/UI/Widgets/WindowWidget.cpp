/**********************************************************************
File name: WindowWidget.cpp
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
#include "WindowWidget.hpp"

#include "DragController.hpp"
#include "RootWidget.hpp"

namespace PyEngine { namespace UI {

/* PyEngine::UI::TitleBar */

TitleBar::TitleBar():
    HBox(),
    _caption(new LabelWidget())
{
    add(_caption);
}

void TitleBar::set_caption(const std::string &value)
{
    _caption->set_text(value);
}

bool TitleBar::ev_mouse_down(int x, int y, MouseButton button,
                             KeyModifiers modifiers)
{
    if (button <= 3) {
        x += absolute_rect().get_left();
        y += absolute_rect().get_top();
        get_root()->start_drag(std::unique_ptr<DragMoveWidget>(
            new DragMoveWidget(get_root(), button, x, y, get_parent())
            ));
        return true;
    }
    return false;
}

/* PyEngine::UI::Window */

Window::Window():
    VBox(),
    _titlebar(new TitleBar()),
    _modal_result(0),
    _on_modal_close(),
    _on_close_hook()
{
    add(_titlebar);
    absolute_rect().set_width(256);
    absolute_rect().set_height(128);
    _flags |= WidgetFlag::FOCUSABLE;
}

void Window::modal_close()
{
    close();
    _on_modal_close(this);
}

void Window::set_caption(const std::string &value)
{
    _titlebar->set_caption(value);
}

void Window::set_modal_result(Window::ModalResult result)
{
    _modal_result = result;
    modal_close();
}

void Window::close()
{
    _on_close_hook(this);
    get_parent()->remove(this);
}

bool Window::ev_activate()
{
    get_parent()->bring_to_front(this);
    return true;
}

bool Window::ev_deactivate()
{
    return true;
}

bool Window::ev_show_modal()
{
    return true;
}

}
}
