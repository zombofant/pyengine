/**********************************************************************
File name: RootWidget.hpp
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
#ifndef _PYE_UI_ROOTWIDGET_H
#define _PYE_UI_ROOTWIDGET_H

#include <set>
#include <list>

#include "CEngine/WindowInterface/EventSink.hpp"

#include "CEngine/UI/CSS/Shapes.hpp"
#include "CEngine/UI/CSS/Theme.hpp"

#include "WidgetBase.hpp"
#include "DragController.hpp"

namespace PyEngine {

class RootWidget: public ParentWidget,
                  public EventSink
{
public:
    RootWidget();
    virtual ~RootWidget();

private:
    ThemePtr _theme;
    Rect _invalidated_rect;
    WidgetPtr _mouse_capture;
    int _mouse_capture_button;
    WidgetPtr _focused;
    HitSet _old_hit_chain;
    HitSet _old_focus_chain;
    cairo_t *_cairo_ctx;
    cairo_surface_t *_cairo_surface;
    PangoContext *_pango_ctx;
    bool _resized;
    std::unique_ptr<DragControllerBase> _drag_controller;
    Point _cursor;
    bool _surface_dirty;

protected:
    void _capture(WidgetPtr capturee, unsigned int button);
    WidgetPtr _find_key_event_target();
    WidgetPtr _find_mouse_event_target(int x, int y,
                                       HitChain *chain = nullptr);
    void _focus(HitChain *chain);
    void _update_hover_state(HitChain *chain);

public:
    virtual void do_align() override;
    RootPtr get_root() override;
    ThemePtr get_theme() override;
    bool is_element(const std::string &name) const override;
    void set_theme(const ThemePtr &theme);

public:
    void focus(const WidgetPtr &widget);
    void invalidate_rect(const Rect &ref);
    void release_capture();
    void start_drag(std::unique_ptr<DragControllerBase> &&controller);
    void stop_drag();

public:
    void dispatch_caret_motion(CaretMotionDirection direction,
                               CaretMotionStep step,
                               bool select);
    void dispatch_hide();
    void dispatch_key_down(Key::Key key,
                           KeyModifiers modifiers) override;
    void dispatch_key_up(Key::Key key,
                         KeyModifiers modifiers) override;
    void dispatch_mouse_click(int x, int y,
                              MouseButton button,
                              KeyModifiers modifiers,
                              unsigned int nth) override;
    void dispatch_mouse_down(int x, int y,
                             MouseButton button,
                             KeyModifiers modifiers) override;
    void dispatch_mouse_move(int x, int y,
                             int dx, int dy,
                             MouseButtons buttons,
                             KeyModifiers modifiers) override;
    void dispatch_mouse_up(int x, int y,
                           MouseButton button,
                           KeyModifiers modifiers) override;
    void dispatch_resize(unsigned int w, unsigned int h);
    void dispatch_scroll(int x, int y,
                         int scrollx, int scrolly) override;
    void dispatch_show();
    void dispatch_text_input(const char* text);
    void dispatch_wm_quit();
    void frame_synced();
    void frame_unsynced(TimeFloat deltaT);

};

}

#endif
