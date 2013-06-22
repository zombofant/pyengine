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

#include "CEngine/UI/RootWidget.hpp"

#include "CEngine/UI/Key.hpp"

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
    unsigned int symbol,
    unsigned int modifiers)
{
    if (symbol == Key::Escape) {
        abort();
    }
}

void DragControllerBase::ev_key_up(
    unsigned int symbol,
    unsigned int modifiers)
{

}

void DragControllerBase::ev_mouse_down(
    int x, int y,
    unsigned int button,
    unsigned int modifiers)
{

}

void DragControllerBase::ev_mouse_move(
    int x, int y,
    unsigned int buttons,
    unsigned int modifiers)
{
    ev_drag_move(x, y, modifiers);
}

void DragControllerBase::ev_mouse_up(
    int x, int y,
    unsigned int button,
    unsigned int modifiers)
{
    if (button == _mouse_button) {
        release();
    }
}

void DragControllerBase::ev_drag_move(
    int x, int y,
    unsigned int modifiers)
{

}

}
