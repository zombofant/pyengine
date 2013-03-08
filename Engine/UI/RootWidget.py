# File name: RootWidget.py
# This file is part of: pyengine
#
# LICENSE
#
# The contents of this file are subject to the Mozilla Public License
# Version 1.1 (the "License"); you may not use this file except in
# compliance with the License. You may obtain a copy of the License at
# http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
# the License for the specific language governing rights and limitations
# under the License.
#
# Alternatively, the contents of this file may be used under the terms
# of the GNU General Public license (the  "GPL License"), in which case
# the provisions of GPL License are applicable instead of those above.
#
# FEEDBACK & QUESTIONS
#
# For feedback and questions about pyengine please e-mail one of the
# authors named in the AUTHORS file.
########################################################################
from __future__ import unicode_literals, print_function, division
from our_future import *

__all__ = ["RootWidget"]

import cairo
import copy

import Engine.CEngine.Pango as Pango

import CSS.Minilanguage

from Engine.CEngine import key, motion
from WidgetBase import AbstractWidget, WidgetContainer
from LayerWidget import LayerWidget, DesktopLayer, WindowLayer, PopupLayer
from Flags import *

from Engine.GL import make_pot

class RootWidget(AbstractWidget, WidgetContainer):
    """
    This widget implements dispatching of events and can work as a parent
    for other widgets. It also handles focus and mouse capturing. For this
    it respects the *Focusable* flag, which must be set by a widget which
    wants to recieve the focus.
    """

    keymap = {
        (key.Left, 0): lambda self: self.dispatch_caret_motion(
            motion.Left, motion.StepChar),
        (key.Right, 0): lambda self: self.dispatch_caret_motion(
            motion.Right, motion.StepChar),
        (key.Home, 0): lambda self: self.dispatch_caret_motion(
            motion.Begin, motion.StepChar),
        (key.End, 0): lambda self: self.dispatch_caret_motion(
            motion.End, motion.StepChar)
        }

    def __init__(self, **kwargs):
        self._theme = None
        self._invalidated_rects = []  # needed during initialization
        super(RootWidget, self).__init__(**kwargs)
        self._rootwidget = self
        self._mouse_capture = None
        self._mouse_capture_button = 0
        self._focused = None
        self._flags = frozenset()
        self._child_classes = LayerWidget
        self._popuplayer = PopupLayer(self)
        self._windowlayer = WindowLayer(self)
        self._desktoplayer = DesktopLayer(self)
        self._old_hit_chain = frozenset()
        self._cairo_surface = None
        self._cairo = None
        self._pango = None
        self._resized = False
        self._invalidated_rects = []  # discard init values, they're incorrect
                                     # anyways
        self.surface_dirty = False

    def _find_key_event_target(self):
        if self._focused and self._focused.RootWidget is not self:
            self._focused = None
        return self._focused or self

    def _map_mouse_event(self, x, y, hitchain=None):
        target = self._mouse_capture or \
            ((hitchain[0] if hitchain is not False else None)
             if hitchain is not None else self._hit_test((x, y)))
        if target is None:
            return None, x, y
        return (target, x - target.AbsoluteRect.Left, y - target.AbsoluteRect.Top)

    def _update_hover_state(self, hitchain=None, p=None):
        if hitchain is None:
            hitchain = self._hit_test_with_chain(p)
        if hitchain is False:
            hitchain = []
        hitchain = frozenset(hitchain)
        old_hit_chain = self._old_hit_chain
        if hitchain != old_hit_chain:
            for non_hovered in old_hit_chain - hitchain:
                non_hovered.IsHovered = False
                non_hovered.on_mouse_leave()

            for hovered in hitchain - old_hit_chain:
                hovered.IsHovered = True
                hovered.on_mouse_enter()

            self._old_hit_chain = hitchain

    def _focus_and_capture(self, hitchain, button):
        target = None
        for candidate in hitchain:
            if Focusable in candidate._flags:
                target = candidate
                break
        else:
            return

        if self._focused is not None:
            self._focused._is_focused = False
            self._focused._invalidate_computed_style()
        self._focused = target
        target.IsFocused = True

        self._mouse_capture = target
        self._mouse_capture_button = button

    def _recreate_cairo_context(self, width, height):
        self._cairo_surface = cairo.ImageSurface(
            cairo.FORMAT_ARGB32,
            width, height)
        self._cairo = cairo.Context(self._cairo_surface)
        self._pango = Pango.PangoCairo.create_context(self._cairo)
        self.update_rendering_context()

    def _require_cairo_context(self):
        myrect = self.AbsoluteRect
        w, h = myrect.Width, myrect.Height

        if not (self._cairo_surface and
                w == self._cairo_surface.get_width() and
                h == self._cairo_surface.get_height()):
            self._recreate_cairo_context(w, h)

    def invalidate_rect(self, rect):
        self._invalidated_rects.append(copy.copy(rect))

    def do_align(self):
        assert len(self) == 3
        myrect = self.AbsoluteRect
        self._desktoplayer.AbsoluteRect = myrect
        self._windowlayer.AbsoluteRect = myrect
        self._popuplayer.AbsoluteRect = myrect

        self._require_cairo_context()
        self._invalidated_rects = []
        self._resized = True

    def dispatch_key_down(self, key, modifiers):
        target = self._find_key_event_target()

        try:
            handler = self.keymap[(key, modifiers)]
        except KeyError:
            pass
        else:
            if handler(self):
                return

        handled = target.on_key_down(key, modifiers)
        parent = target.Parent
        while parent and not handled:
            handled = parent.on_key_down(key, modifiers)
            parent = parent.Parent

    def dispatch_key_up(self, *args):
        target = self._find_key_event_target()
        handled = target.on_key_up(*args)
        parent = target.Parent
        while parent and not handled:
            handled = parent.on_key_up(*args)
            parent = parent.Parent

    def dispatch_mouse_down(self, x, y, button, modifiers):
        if self._mouse_capture is None:
            hitchain = self._hit_test_with_chain((x, y))
        else:
            hitchain = None
        target, x, y = self._map_mouse_event(x, y, hitchain)
        if target:
            target.on_mouse_down(x, y, button, modifiers)
        if self._mouse_capture is None:
            self._focus_and_capture(hitchain, button)

    def dispatch_mouse_click(self, x, y, button, modifiers, nth):
        target, cx, cy = self._map_mouse_event(x, y)
        if target:
            target.on_mouse_click(cx, cy, button, modifiers, nth)

    def dispatch_mouse_move(self, x, y, dx, dy, button, modifiers):
        if self._mouse_capture is None:
            hitchain = self._hit_test_with_chain((x, y))
        else:
            hitchain = None
        target, x, y = self._map_mouse_event(x, y, hitchain)
        if target:
            target.on_mouse_move(x, y, dx, dy, button, modifiers)

        if self._mouse_capture is None:
            self._update_hover_state(hitchain)

    def dispatch_mouse_up(self, x, y, button, modifiers):
        target, cx, cy = self._map_mouse_event(x, y)
        if target:
            target.on_mouse_up(cx, cy, button, modifiers)
        if target is self._mouse_capture and button & self._mouse_capture_button:
            self._mouse_capture = None
            self._mouse_capture_button = 0
            self._update_hover_state(p=(x,y))

    def dispatch_scroll(self, x, y, scrollX, scrollY):
        target, x, y = self._map_mouse_event(x, y)
        if target:
            target.on_scroll(scrollX, scrollY)

    def dispatch_text_input(self, text):
        target = self._find_key_event_target()
        target.on_text_input(text)

    def dispatch_caret_motion(self, direction, step):
        target = self._find_key_event_target()
        handled = target.on_caret_motion(direction, step)
        parent = target.Parent
        while not handled and parent:
            handled = parent.on_caret_motion(direction, step)
            parent = parent.Parent

    def dispatch_caret_motion_select(self, direction, step):
        target = self._find_key_event_target()
        handled = target.on_caret_motion_select(direction, step)
        parent = target.Parent
        while not handled and parent:
            handled = parent.on_caret_motion_select(direction, step)
            parent = parent.Parent
        return handled

    def realign(self):
        super(RootWidget, self).realign()
        for child in self:
            child.realign()

    def clear_cairo_surface(self):
        ctx = self._cairo
        ctx.set_source_rgba(0., 0., 0., 0.)
        ctx.set_operator(cairo.OPERATOR_SOURCE)
        ctx.paint()
        ctx.set_operator(cairo.OPERATOR_OVER)
        ctx.set_line_cap(cairo.LINE_CAP_SQUARE)

    def _setup_clipping(self):
        ctx = self._cairo
        for rect in self._invalidated_rects:
            ctx.rectangle(*rect.XYWH)
        ctx.clip()

    def render(self):
        self.realign()
        self.surface_dirty = False
        if not (self._invalidated_rects or self._resized):
            # no dirty regions, nothing to do \o/
            return
        if not self._resized:
            # set up clipping regions
            self._setup_clipping()
        self.clear_cairo_surface()
        self._desktoplayer.render()
        self._windowlayer.render()
        self._popuplayer.render()
        self._invalidated_rects = []
        self._resized = False
        self._cairo.reset_clip()
        self.surface_dirty = True

    def update(self, timedelta):
        for child in self:
            child.update(timedelta)

    @property
    def WindowLayer(self):
        return self._windowlayer

    @property
    def DesktopLayer(self):
        return self._desktoplayer

    @property
    def PopupLayer(self):
        return self._popuplayer

    @property
    def Parent(self):
        return None

    @property
    def RootWidget(self):
        return self

    @property
    def Theme(self):
        return self._theme

    @Theme.setter
    def Theme(self, theme):
        if theme == self._theme:
            return
        self._theme = theme
        for widget in self.tree_depth_first():
            widget._theme_changed()

CSS.Minilanguage.ElementNames().register_widget_class(RootWidget, "Root")
