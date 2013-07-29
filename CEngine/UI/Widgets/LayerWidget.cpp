/**********************************************************************
File name: LayerWidget.cpp
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
#include "LayerWidget.hpp"

#include "RootWidget.hpp"
#include "WindowWidget.hpp"

namespace PyEngine { namespace UI {

/* PyEngine::UI::LayerWidget */

/* PyEngine::UI::DesktopLayer */

void DesktopLayer::do_align()
{
    const Rect r = absolute_rect();
    for (auto& child: *this) {
        Margin m = child->computed_style().margin();
        coord_int_t w, h;
        std::tie(w, h) = child->get_dimensions();
        Rect obj = Rect(r.get_left(), r.get_top(), w, h);
        m.deautoify(obj, r);
        obj = r;
        obj.shrink(m);
        child->absolute_rect() = obj;
        child->invalidate_alignment();
    }
}

bool DesktopLayer::is_element(const std::string &name) const
{
    return (name == "desktoplayer");
}

/* PyEngine::UI::WindowLayer */

void WindowLayer::do_align()
{
    for (auto& child: *this) {
        ModalWindowLayer *modal = dynamic_cast<ModalWindowLayer*>(child);
        if (modal) {
            modal->absolute_rect() = absolute_rect();
        }
    }
}

bool WindowLayer::is_element(const std::string &name) const
{
    return (name == "windowlayer");
}

/* PyEngine::UI::PopupLayer */

WidgetPtr PopupLayer::hittest(const Point &p)
{
    if (_current_root_menu) {
        WidgetPtr hit = LayerWidget::hittest(p);
        if ((hit == this) && (_current_root_menu->absolute_rect().contains(p)))
        {
            return _current_root_menu->hittest(p);
        }
        return hit;
    } else {
        return LayerWidget::hittest(p);
    }
}

bool PopupLayer::hittest_with_chain(const Point &p, HitChain &chain)
{
    if (!_hittest(p)) {
        return false;
    }

    if (_current_root_menu) {
        _current_root_menu->hittest_with_chain(p, chain);
    }

    chain.push_back(this);
    return true;
}

bool PopupLayer::is_element(const std::string &name) const
{
    return (name == "popuplayer");
}

bool PopupLayer::ev_mouse_click(
    int x, int y,
    MouseButton button,
    KeyModifiers modifiers,
    unsigned int nth)
{
    if (_current_root_menu) {
        /* TODO: close menu */
    }
    return false;
}

/* PyEngine::UI::ModalWindowLayer */

ModalWindowLayer::ModalWindowLayer(
        ModalWindowLayer::WindowWidgetPtr window):
    _window(window),
    _close_hook(_window->on_close_hook().connect(
        sigc::mem_fun(this, &ModalWindowLayer::_handle_window_close)))
{
    add(_window);
    RootPtr root = get_root();
    if (root) {
        root->focus(_window);
    }
}

ModalWindowLayer::~ModalWindowLayer()
{
    _close_hook.disconnect();
}

void ModalWindowLayer::_parent_changed()
{
    ParentWidget::_parent_changed();
    WidgetPtr parent = get_parent();
    if (parent) {
        absolute_rect() = parent->absolute_rect();
        invalidate();
    }
    RootPtr root = get_root();
    if (root && _window) {
        root->focus(_window);
    }
}

void ModalWindowLayer::_handle_window_close(WidgetPtr sender)
{
    invalidate();
    _close_hook.disconnect();
    ParentPtr parent = get_parent();
    if (parent) {
        parent->remove(this);
    }
}

bool ModalWindowLayer::is_element(const std::string &name) const
{
    return (name == "modalwindowlayer");
}

}
}
