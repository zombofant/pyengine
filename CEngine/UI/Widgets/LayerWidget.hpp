/**********************************************************************
File name: LayerWidget.hpp
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
#ifndef _PYE_UI_LAYER_WIDGET_H
#define _PYE_UI_LAYER_WIDGET_H

#include "WidgetBase.hpp"

#include "CEngine/UI/Events.hpp"

namespace PyEngine {

static const char *foo;

class LayerWidget: public ParentWidget
{
public:
    LayerWidget() = default;

};

class DesktopLayer: public LayerWidget
{
public:
    DesktopLayer() = default;

public:
    void do_align() override;

};

class WindowLayer: public LayerWidget
{
public:
    WindowLayer() = default;

public:
    void do_align() override;

};

class PopupLayer: public LayerWidget
{
public:
    typedef std::shared_ptr<Widget> MenuWidgetPtr;

public:
    PopupLayer() = default;

private:
    /* TODO: Use proper menu type here */
    MenuWidgetPtr _current_root_menu;

public:
    inline const MenuWidgetPtr& get_current_root_menu() const
    {
        return _current_root_menu;
    };

    void set_current_root_menu(const MenuWidgetPtr &ref)
    {
        if (_current_root_menu) {
            /* TODO: close this menu */
        }
        _current_root_menu = ref;
    };

public:
    WidgetPtr hittest(const Point &p) override;
    bool hittest_with_chain(const Point &p, HitChain &chain) override;

public:
    bool ev_mouse_click(
        int x, int y,
        MouseButton button,
        KeyModifiers modifiers,
        unsigned int nth) override;

};

class Window;

class ModalWindowLayer: public ParentWidget
{
public:
    typedef Window* WindowWidgetPtr;

public:
    ModalWindowLayer() = delete;
    ModalWindowLayer(WindowWidgetPtr window);
    virtual ~ModalWindowLayer();

private:
    WindowWidgetPtr _window;
    sigc::connection _close_hook;

protected:
    void _handle_window_close(WidgetPtr sender);
    void _parent_changed() override;
};

}

#endif
