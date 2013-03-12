# encoding=utf-8
# File name: ScrollWidget.py
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

__all__ = ["ScrollBar", "ScrollMode"]

import copy
import math

try:
    import Engine.CEngine.Pango as Pango
except ImportError:
    Pango = None

import CSS.Minilanguage
from CSS.Rect import Rect

from WidgetBase import Widget, ParentWidget
from ButtonWidget import AbstractButton
from DragController import DragMoveWidget
import Flags

class ScrollMode(object):
    pass

ScrollMode.HORIZONTAL = ScrollMode()
ScrollMode.VERTICAL = ScrollMode()
ScrollMode.valid_modes = {ScrollMode.VERTICAL, ScrollMode.HORIZONTAL}

class DragScrollThumb(DragMoveWidget):
    def __init__(self,
                 root_widget,
                 mouse_button,
                 x, y,
                 scrollbar,
                 on_thumb_drag=None,
                 **kwargs):
        super(DragScrollThumb, self).__init__(
            root_widget,
            mouse_button,
            x, y,
            scrollbar._thumb,
            **kwargs)
        self.scrollbar = scrollbar
        self.thumb = self.widget
        self.old_position = scrollbar.Position

    def release(self):
        super(DragScrollThumb, self).release()

    def abort(self):
        self.scrollbar.Position = self.old_position
        super(DragScrollThumb, self).abort()

    def on_widget_move(self, x, y):
        self.scrollbar._thumb_drag_handler(x, y)

class ScrollButton(AbstractButton):
    pass

class ScrollThumb(Widget):
    def __init__(self, parent, **kwargs):
        super(ScrollThumb, self).__init__(parent, **kwargs)

class AbstractScrollBar(ParentWidget):
    def __init__(self, parent, mode, on_scroll=None, **kwargs):
        super(AbstractScrollBar, self).__init__(parent, **kwargs)
        self._up_button = ScrollButton(self,
                                       onclick=self._up_button_click)
        self._up_button.StyleClasses.add("up")
        self._down_button = ScrollButton(self,
                                         onclick=self._down_button_click)
        self._down_button.StyleClasses.add("down")
        self._thumb = ScrollThumb(self)
        self._min = 0
        self._max = 10
        self._step = 1
        self._page = 1
        self._on_scroll = on_scroll
        self._set_mode(mode)
        self._position = 0
        self._thumb_factor = 0
        self._thumb_drag_btn = 0
        self._flags = {Flags.CaptureMouse}
        self._thumb_bar_rect = Rect(0, 0, 0, 0)

    def _increase_position(self, delta):
        new_position = self._position + delta
        if new_position > self._max:
            new_position = self._max
        self.Position = new_position

    def _decrease_position(self, delta):
        new_position = self._position - delta
        if new_position < self._min:
            new_position = self._min
        self.Position = new_position

    def _up_button_click(self, sender):
        self._increase_position(self._step)

    def _down_button_click(self, sender):
        self._decrease_position(self._step)

    def _set_mode(self, mode):
        try:
            aligner, down_caption, up_caption, css_class, \
                thumb_dragged, thumb_rel_click = \
                {
                ScrollMode.HORIZONTAL: (
                    self._horiz_do_align, "←", "→", "horizontal",
                    self._horiz_thumb_dragged, self._horiz_thumb_rel_click),
                ScrollMode.VERTICAL: (
                    self._vert_do_align, "↑", "↓", "vertical",
                    self._vert_thumb_dragged, self._vert_thumb_rel_click)
                }[mode]
        except KeyError:
            raise ValueError("Invalid scroll mode: {}".format(mode))
        self._aligner = aligner
        self.StyleClasses.discard("horizontal")
        self.StyleClasses.discard("vertical")
        self.StyleClasses.add(css_class)
        self._up_button.Caption = up_caption
        self._down_button.Caption = down_caption
        self._thumb_drag_handler = thumb_dragged
        self._thumb_rel_click = thumb_rel_click

    def _calculate_thumb_size(self, space, min_size):
        steps = math.ceil((self._max - self._min + 1) / self._step)
        if steps > space - min_size:
            # crunch
            size = min_size
            if space - size <= 0:
                size = 4
        else:
            # expand
            size = space - steps

        if space - size <= 0:
            space = 0
        else:
            space -= size
        return size, steps, space

    def _horiz_thumb_dragged(self, x, y):
        # offset x so that it matches the left end of the thumb bar
        x -= self._thumb_bar_rect.Left
        new_position = int(round(x * self._thumb_factor))
        if new_position > self._max:
            new_position = self._max
        elif new_position < self._min:
            new_position = self._min
        self.Position = new_position

    def _horiz_thumb_rel_click(self, rx, ry):
        if rx < 0:
            self._decrease_position(self._page)
        if rx > self._thumb.AbsoluteRect.Width:
            self._increase_position(self._page)

    def _horiz_do_align(self):
        mystyle = self.ComputedStyle
        myrect = copy.copy(self.AbsoluteRect)
        myrect.shrink(mystyle.Padding)
        myrect.shrink(mystyle.Border.get_box())
        height = myrect.Height

        self._down_button.AbsoluteRect = Rect(
            myrect.Left,
            myrect.Top,
            myrect.Left + height,
            myrect.Bottom
            )

        self._up_button.AbsoluteRect = Rect(
            myrect.Right - height,
            myrect.Top,
            myrect.Right,
            myrect.Top + height
            )

        thumb_style = self._thumb.ComputedStyle

        space = myrect.Width - 2*height - thumb_style.Margin.Horizontal

        self._thumb_bar_rect = Rect(
            myrect.Left + height,
            myrect.Top,
            myrect.Right - height,
            myrect.Bottom
            )

        # FIXME: use min-width/min-height as soon as it's available here

        thumb_min_width = 10

        thumb_width, steps, space = \
            self._calculate_thumb_size(space, thumb_min_width)

        thumb_x = round(
            (self._position - self._min) / (self._max - self._min) * space
            ) + myrect.X + height + thumb_style.Margin.Left

        self._thumb_factor = steps / space

        self._thumb.AbsoluteRect = Rect(
            thumb_x,
            myrect.Y + thumb_style.Margin.Top,
            thumb_x + thumb_width,
            myrect.Bottom - thumb_style.Margin.Bottom
            )
        self._thumb.invalidate()

    def _vert_thumb_dragged(self, x, y):
        # offset y so that it matches the left end of the thumb bar
        y -= self._thumb_bar_rect.Top
        new_position = int(round(y * self._thumb_factor))
        if new_position > self._max:
            new_position = self._max
        elif new_position < self._min:
            new_position = self._min
        self.Position = new_position

    def _vert_thumb_rel_click(self, rx, ry):
        if ry < 0:
            self._decrease_position(self._page)
        if ry > self._thumb.AbsoluteRect.Height:
            self._increase_position(self._page)

    def _vert_do_align(self):
        mystyle = self.ComputedStyle
        myrect = copy.copy(self.AbsoluteRect)
        myrect.shrink(mystyle.Padding)
        myrect.shrink(mystyle.Border.get_box())
        width = myrect.Width

        self._down_button.AbsoluteRect = Rect(
            myrect.Left,
            myrect.Top,
            myrect.Right,
            myrect.Top + width
            )

        self._up_button.AbsoluteRect = Rect(
            myrect.Left,
            myrect.Bottom - width,
            myrect.Right,
            myrect.Bottom,
            )

        thumb_style = self._thumb.ComputedStyle

        space = myrect.Height - 2*width - thumb_style.Margin.Vertical

        self._thumb_bar_rect = Rect(
            myrect.Left,
            myrect.Top + width,
            myrect.Right,
            myrect.Bottom - width
            )

        # FIXME: use min-width/min-height as soon as it's available here

        thumb_min_height = 10

        thumb_height, steps, space = \
            self._calculate_thumb_size(space, thumb_min_height)

        thumb_y = round(
            (self._position - self._min) / (self._max - self._min) * space
            ) + myrect.Y + width + thumb_style.Margin.Top

        self._thumb_factor = steps / space

        self._thumb.AbsoluteRect = Rect(
            myrect.X + thumb_style.Margin.Left,
            thumb_y,
            myrect.Right - thumb_style.Margin.Right,
            thumb_y + thumb_height,
            )
        self._thumb.invalidate()

    def do_align(self):
        super(AbstractScrollBar, self).do_align()
        self._aligner()

    def set_position_silently(self, value):
        if not self._min <= value <= self._max:
            raise ValueError("Position ({}) must be in range {}..{}".format(
                    value, self._min, self._max
                    ))
        self._position = value
        self._invalidate_alignment()

    def on_mouse_down(self, x, y, button, modifiers):
        if button <= 3  :
            x += self.AbsoluteRect.Left
            y += self.AbsoluteRect.Top
            thumb_rect = self._thumb.AbsoluteRect
            if (x, y) in thumb_rect:
                self.RootWidget.start_drag(
                    DragScrollThumb,
                    button,
                    x, y,
                    self)
            return True

    def on_mouse_click(self, x, y, button, modifiers, nth):
        if button <= 3:
            x += self.AbsoluteRect.Left
            y += self.AbsoluteRect.Top
            if not (x, y) in self._thumb_bar_rect:
                return True
            thumb_rect = self._thumb.AbsoluteRect
            if not (x, y) in thumb_rect:
                x -= thumb_rect.Left
                y -= thumb_rect.Top
                self._thumb_rel_click(x, y)
            return True

    @property
    def Range(self):
        return self._min, self._max

    @Range.setter
    def Range(self, value):
        self.Min, self.Max = value

    @property
    def Min(self):
        return self._min

    @Min.setter
    def Min(self, value):
        if value > self._max:
            raise ValueError(
                "Min ({}) must be less than or equal to Max ({})".format(
                    value, self._max
                    )
                )
        self._min = value
        self._invalidate_alignment()

    @property
    def Max(self):
        return self._max

    @Max.setter
    def Max(self, value):
        if value < self._min:
            raise ValueError(
                "Max ({}) must be greater than or equal to Min ({})".format(
                    value, self._min
                    )
                )
        self._max = value
        self._invalidate_alignment()

    @property
    def Position(self):
        return self._position

    @Position.setter
    def Position(self, value):
        old_position = self._position
        if value == old_position:
            return
        self.set_position_silently(value)
        if self._on_scroll:
            self._on_scroll(old_position, value)

    @property
    def Page(self):
        return self._page

    @Page.setter
    def Page(self, value):
        if value < 0:
            raise ValueError("Page size must be non-negative.")
        self._page = value


class ScrollBar(AbstractScrollBar):
    pass

CSS.Minilanguage.ElementNames().register_widget_class(ScrollButton)
CSS.Minilanguage.ElementNames().register_widget_class(ScrollThumb)
CSS.Minilanguage.ElementNames().register_widget_class(ScrollBar)
