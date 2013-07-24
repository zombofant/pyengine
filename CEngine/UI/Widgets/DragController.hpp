/**********************************************************************
File name: DragController.hpp
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
#ifndef _PYE_UI_DRAG_CONTROLLER_H
#define _PYE_UI_DRAG_CONTROLLER_H

#include "CEngine/UI/Key.hpp"
#include "CEngine/UI/Static.hpp"

#include "WidgetBase.hpp"

namespace PyEngine {

class RootWidget;

class DragControllerBase
{
public:
    DragControllerBase(RootWidget *root_widget,
                       unsigned int mouse_button);
    DragControllerBase(const DragControllerBase &ref) = delete;
    DragControllerBase& operator=(const DragControllerBase &ref)
        = delete;
    virtual ~DragControllerBase();

protected:
    RootWidget *_root_widget;
    unsigned int _mouse_button;

public:
    virtual void abort();
    virtual void release();

public: /* event handlers */
    virtual void ev_key_down(Key::Key key,
                             KeyModifiers modifiers);
    virtual void ev_key_up(Key::Key key,
                           KeyModifiers modifiers);
    virtual void ev_mouse_down(int x, int y,
                               MouseButton button,
                               KeyModifiers modifiers);
    virtual void ev_mouse_move(int x, int y,
                               MouseButtons buttons,
                               KeyModifiers modifiers);
    virtual void ev_mouse_up(int x, int y,
                             MouseButton button,
                             KeyModifiers modifiers);
    virtual void ev_drag_move(int x, int y,
                              KeyModifiers modifiers);

};

class DragMoveWidget: public DragControllerBase
{
public:
    DragMoveWidget(
        RootWidget *root_widget,
        unsigned int mouse_button,
        int x, int y,
        const WidgetPtr &widget);

private:
    int _startx, _starty;
    WidgetPtr _widget;
    int _startxoffs, _startyoffs;

protected:
    void ev_widget_move(int x, int y);

public:
    void ev_drag_move(int x, int y, KeyModifiers modifiers) override;

};

}

#endif
