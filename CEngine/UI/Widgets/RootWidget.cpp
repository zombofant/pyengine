/**********************************************************************
File name: RootWidget.cpp
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
#include "RootWidget.hpp"

#include <stdexcept>

#include "CEngine/Misc/SetOperations.hpp"

namespace PyEngine {

/* PyEngine::RootWidget */

RootWidget::RootWidget():
    ParentWidget(),
    EventSink(),
    std::enable_shared_from_this<RootWidget>(),
    _theme(new Theme()),
    _invalidated_rect(),
    _mouse_capture(nullptr),
    _mouse_capture_button(0),
    _focused(nullptr),
    _old_hit_chain(),
    _old_focus_chain(),
    _cairo_ctx(),
    _cairo_surface(),
    _pango_ctx(),
    _resized(false),
    _drag_controller(nullptr),
    _cursor(0, 0),
    _surface_dirty()
{
    /* TODO: add layer widgets */
    _invalidated_rect = NotARect;
}

RootWidget::~RootWidget()
{

}

void RootWidget::_capture(WidgetPtr capturee, unsigned int button)
{
    if (!(capturee->flags() & WidgetFlag::CAPTURE_MOUSE)) {
        return;
    }
    _mouse_capture = capturee;
    _mouse_capture_button = button;
}

WidgetPtr RootWidget::_find_key_event_target()
{
    if (_focused && (_focused->get_root().get() != this)) {
        _focused = nullptr;
    }
    return (_focused ? _focused : get_root());
}

WidgetPtr RootWidget::_find_mouse_event_target(
    int x, int y, HitChain *chain)
{
    if (_mouse_capture) {
        return _mouse_capture;
    }

    if (chain) {
        if (chain->size() == 0) {
            return WidgetPtr();
        } else {
            return chain->front();
        }
    }

    return hittest(Point(x, y));
}

void RootWidget::_focus(HitChain *chain)
{
    if (chain == nullptr) {
        return;
    }

    WidgetPtr target = nullptr;
    HitChain::iterator target_iter = chain->end();
    for (auto it = chain->begin(); it != chain->end(); it++)
    {
        target = *it;
        if (target->flags() & WidgetFlag::FOCUSABLE) {
            target_iter = it;
            break;
        }
    }
    if (target_iter == chain->end()) {
        return;
    }

    if (_focused) {
        _focused->state().reset(Focused);
    }
    _focused = target;
    target->state().set(Focused);

    target_iter++;
    HitSet new_set(target_iter, chain->end());

    auto diff = set_difference_iterator(new_set, _old_focus_chain);
    for (; diff != decltype(diff)(); ++diff) {
        const WidgetPtr &focused = *diff;
        focused->state().set(HasFocusedChild);
    }

    diff = set_difference_iterator(_old_focus_chain, new_set);
    for (; diff != decltype(diff)(); ++diff) {
        const WidgetPtr &unfocused = *diff;
        unfocused->state().reset(HasFocusedChild);
    }

    std::swap(_old_focus_chain, new_set);
}

void RootWidget::_update_hover_state(HitChain *chain)
{
    if (chain == nullptr) {
        std::unique_ptr<HitChain> tmp_chain(new HitChain());
        _update_hover_state(tmp_chain.get());
        return;
    }

    HitSet new_set(chain->begin(), chain->end());
    auto diff = set_difference_iterator(new_set, _old_hit_chain);
    for (; diff != decltype(diff)(); ++diff) {
        const WidgetPtr &hovered = *diff;
        hovered->state().set(Hovered);
    }

    diff = set_difference_iterator(_old_hit_chain, new_set);
    for (; diff != decltype(diff)(); ++diff) {
        const WidgetPtr &hovered = *diff;
        hovered->state().reset(Hovered);
    }

    std::swap(_old_hit_chain, new_set);
}

void RootWidget::do_align()
{
    /* TODO: align the layers */
}

RootPtr RootWidget::get_root()
{
    return this->std::enable_shared_from_this<RootWidget>::shared_from_this();
}

ThemePtr RootWidget::get_theme()
{
    return _theme;
}

void RootWidget::focus(const WidgetPtr &ref)
{
    HitChain chain;
    iter_upwards(ref, std::back_inserter(chain));
    _focus(&chain);
}

void RootWidget::invalidate_rect(const Rect &ref)
{
    _invalidated_rect.extend(ref);
}

void RootWidget::release_capture()
{
    _mouse_capture = nullptr;
    _mouse_capture_button = 0;
}

void RootWidget::start_drag(
    std::unique_ptr<DragControllerBase> &&controller)
{
    if (_drag_controller) {
        throw std::invalid_argument("Dragging is already in progress.");
    }
    _drag_controller = std::move(controller);
    release_capture();
}

void RootWidget::stop_drag()
{
    _drag_controller = nullptr;
    release_capture();
}

/* event dispatching */

void RootWidget::dispatch_caret_motion(
    CaretMotionDirection direction,
    CaretMotionStep step,
    bool select)
{
    WidgetPtr target = _find_key_event_target();
    bool handled = false;
    while (target && !handled) {
        handled = target->ev_caret_motion(direction, step, select);
        if (handled) {
            break;
        }
        target = target->get_parent();
    }
}

void RootWidget::dispatch_hide()
{

}

void RootWidget::dispatch_key_down(Key::Key key,
                                   KeyModifiers modifiers)
{
    if (_drag_controller) {
        _drag_controller->ev_key_down(key, modifiers);
        return;
    }

    /* TODO: dispatch caret motion (and other global bindings) */

    WidgetPtr target = _find_key_event_target();
    bool handled = false;
    while (target && !handled) {
        handled = target->ev_key_down(key, modifiers);
        if (handled) {
            break;
        }
        target = target->get_parent();
    }
}

void RootWidget::dispatch_key_up(Key::Key key,
                                 KeyModifiers modifiers)
{
    if (_drag_controller) {
        _drag_controller->ev_key_up(key, modifiers);
        return;
    }

    WidgetPtr target = _find_key_event_target();
    bool handled = false;
    while (target && !handled) {
        handled = target->ev_key_up(key, modifiers);
        if (handled) {
            break;
        }
        target = target->get_parent();
    }
}

void RootWidget::dispatch_mouse_click(
    int x, int y,
    MouseButton button,
    KeyModifiers modifiers,
    unsigned int nth)
{
    if (_drag_controller) {
        return;
    }

    WidgetPtr target = _find_mouse_event_target(x, y);
    bool handled = false;
    while (target && !handled) {
        handled = target->ev_mouse_click(x, y, button, modifiers, nth);
        if (handled) {
            break;
        }
        target = target->get_parent();
    }
}

void RootWidget::dispatch_mouse_down(
    int x, int y,
    MouseButton button,
    KeyModifiers modifiers)
{
    if (_drag_controller) {
        _drag_controller->ev_mouse_down(x, y, button, modifiers);
        return;
    }

    std::unique_ptr<HitChain> hitchain;
    if (!_mouse_capture) {
        hitchain = std::unique_ptr<HitChain>(new HitChain());
        hittest_with_chain(Point(x, y), *hitchain.get());
    }

    _focus(hitchain.get());

    WidgetPtr target;
    target = _find_mouse_event_target(x, y, hitchain.get());

    bool handled = false;
    while (target && !handled) {
        const Rect &abs_rect = target->get_absolute_rect();
        coord_int_t tx = x - abs_rect.get_left(),
                    ty = y - abs_rect.get_top();

        handled = target->ev_mouse_down(tx, ty, button, modifiers);
        if (handled) {
            // we must break here as we need the actual target for
            // capturing
            break;
        }
        target = target->get_parent();
    }

    if (handled && !_mouse_capture) {
        _capture(target, button);
    }
}

void RootWidget::dispatch_mouse_move(
    int x, int y,
    int dx, int dy,
    MouseButtons buttons,
    KeyModifiers modifiers)
{
    _cursor = std::move(Point(x, y));
    if (_drag_controller) {
        _drag_controller->ev_mouse_move(x, y, buttons, modifiers);
        return;
    }

    std::unique_ptr<HitChain> hitchain;
    if (!_mouse_capture) {
        hitchain = std::unique_ptr<HitChain>(new HitChain());
        hittest_with_chain(Point(x, y), *hitchain.get());
    }

    WidgetPtr target;
    target = _find_mouse_event_target(x, y, hitchain.get());

    bool handled = false;
    while (target && !handled) {
        const Rect &abs_rect = target->get_absolute_rect();
        coord_int_t tx = x - abs_rect.get_left(),
                    ty = y - abs_rect.get_top();

        handled = target->ev_mouse_move(tx, ty, dx, dy, buttons, modifiers);
        if (_mouse_capture) {
            // no hover update and no propagation upwards on captured
            // mouse!
            return;
        }
        if (handled) {
            break;
        }
        target = target->get_parent();
    }

    _update_hover_state(hitchain.get());
}

void RootWidget::dispatch_mouse_up(
    int x, int y,
    MouseButton button,
    KeyModifiers modifiers)
{
    if (_drag_controller) {
        _drag_controller->ev_mouse_up(x, y, button, modifiers);
        return;
    }

    WidgetPtr target;
    target = _find_mouse_event_target(x, y);

    bool handled = false;
    while (target && !handled) {
        const Rect &abs_rect = target->get_absolute_rect();
        coord_int_t tx = x - abs_rect.get_left(),
                    ty = y - abs_rect.get_top();

        handled = target->ev_mouse_up(tx, ty, button, modifiers);
        if (handled) {
            break;
        }
        target = target->get_parent();
    }
}

void RootWidget::dispatch_resize(
    unsigned int w,
    unsigned int h)
{

}

void RootWidget::dispatch_scroll(
    int x, int y,
    int scrollx, int scrolly)
{
    WidgetPtr target;
    target = _find_mouse_event_target(x, y);

    bool handled = false;
    while (target && !handled) {
        handled = target->ev_scroll(scrollx, scrolly);
        if (handled) {
            break;
        }
        target = target->get_parent();
    }
}

void RootWidget::dispatch_show()
{

}

void RootWidget::dispatch_text_input(const char *text)
{
    if (_drag_controller) {
        return;
    }

    WidgetPtr target = _find_key_event_target();
    if (target) {
        target->ev_text_input(text);
    }
}

void RootWidget::dispatch_wm_quit()
{

}

}
