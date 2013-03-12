# File name: ListWidget.py
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

__all__ = ["AbstractList", "List", "ListItem", "TextListItem"]

import copy

import CSS.Minilanguage

from WidgetBase import AbstractWidget, ParentWidget, Widget
from Label import Label
from LabelWidget import LabelledWidget
from ScrollWidget import ScrollBar, ScrollMode

import CSS.Rect as Rect

class ListItem(AbstractWidget):
    def on_mouse_click(self, x, y, button, modifiers, nth):
        self.Parent.select_item(self)

class TextListItem(LabelledWidget, ListItem):
    def __init__(self, parent, text, **kwargs):
        super(TextListItem, self).__init__(parent, **kwargs)
        self._label.Text = text

class AbstractList(ParentWidget):
    def __init__(self, parent, **kwargs):
        super(AbstractList, self).__init__(parent, **kwargs)

        self._vbar = ScrollBar(self, ScrollMode.VERTICAL,
                               on_scroll=self._scroll_vertical)
        self._hbar = ScrollBar(self, ScrollMode.HORIZONTAL,
                               on_scroll=self._scroll_horizontal)

        self._items = []
        self._selected_item = None
        self._item_index = -1
        self._clip_rect = Rect.Rect(0, 0, 0, 0)

    def _calc_inner_dimensions(self):
        max_width, total_height = 0, 0
        min_height = None
        for widget in self._items:
            widget_style = widget.ComputedStyle
            w, h = widget.get_dimensions()
            if w is None or h is None:
                continue
            max_width = max(w + widget_style.Margin.Horizontal, max_width)
            total_height += h + widget_style.Margin.Vertical
            if min_height is None:
                min_height = h
            else:
                min_height = min(min_height, h)

        return max_width, total_height, min_height

    def do_align(self):
        style = self.ComputedStyle
        rect = copy.copy(self.AbsoluteRect)
        rect.shrink(style.Border.get_box())
        rect.shrink(style.Padding)

        hbar_height = self._hbar.ComputedStyle.Height
        vbar_width = self._vbar.ComputedStyle.Width

        self._hbar.AbsoluteRect = Rect.Rect(
            rect.Left,
            rect.Bottom - hbar_height,
            rect.Right - vbar_width,
            rect.Bottom)
        self._vbar.AbsoluteRect = Rect.Rect(
            rect.Right - vbar_width,
            rect.Top,
            rect.Right,
            rect.Bottom - hbar_height)

        inner_w, inner_h, min_height = self._calc_inner_dimensions()
        if min_height is not None:
            self._vbar.Step = min_height / 2
            self._vbar.Page = min_height

        if inner_w > rect.Width - vbar_width:
            self._hbar.Visible = True
        else:
            self._hbar.Visible = False

        if inner_h > rect.Height - hbar_height:
            self._vbar.Visible = True
        else:
            self._vbar.Visible = False

        if self._vbar.Visible:
            rect.Right -= self._vbar.AbsoluteRect.Width
        if self._hbar.Visible:
            rect.Bottom -= self._hbar.AbsoluteRect.Height

        inner_w = max(rect.Width, inner_w)

        if self._hbar.Visible:
            self._hbar.Range = (0, inner_w - rect.Width)
            if not self._vbar.Visible:
                self._hbar.AbsoluteRect.Right = rect.Right

        if self._vbar.Visible:
            self._vbar.Range = (0, inner_h - rect.Height)
            if not self._hbar.Visible:
                self._vbar.AbsoluteRect.Bottom = rect.Bottom

        y = rect.Top - self._vbar.Position
        for item in self._items:
            item_style = item.ComputedStyle
            item_margin = item_style.Margin
            _, item_height = item.get_dimensions()

            if item_height is None:
                continue

            y += item_margin.Top

            item.AbsoluteRect = Rect.Rect(
                rect.Left + item_margin.Left,
                y,
                rect.Right - item_margin.Right,
                y + item_height)

            y += item_height + item_margin.Bottom

        self._clip_rect = rect

    def _scroll_vertical(self, old, new):
        self._invalidate_alignment()
        self.invalidate()
        self.RootWidget.rehover()

    def _scroll_horizontal(self, old, new):
        self._invalidate_alignment()
        self.invalidate()
        self.RootWidget.rehover()

    def add(self, child):
        super(AbstractList, self).add(child)
        if not isinstance(child, ScrollBar):
            self._items.append(child)

    def remove(self, child):
        super(AbstractList, self).remove(child)
        if not isinstance(child, ScrollBar):
            self._items.remove(child)

    def select_item(self, item):
        if item == self._selected_item:
            return

        if self._selected_item:
            self._selected_item.IsActive = False

        if item is None:
            self._selected_item = None
            self._item_index = -1
            return

        try:
            self._item_index = self._items.index(item)
        except ValueError:
            self.select_item(None)
            return

        self._selected_item = item
        item.IsActive = True

    def render(self):
        AbstractWidget.render(self)
        ctx = self._cairo
        ctx.save()
        try:
            ctx.rectangle(*self._clip_rect.XYWH)
            ctx.clip()
            for item in self._items:
                item.render()
        finally:
            ctx.restore()

        if self._vbar.Visible:
            self._vbar.render()
        if self._hbar.Visible:
            self._hbar.render()

    def on_scroll(self, scrollX, scrollY):
        self._vbar.scroll_by(-scrollY * self._vbar.Step)
        self._hbar.scroll_by(scrollX * self._hbar.Step)

class List(AbstractList):
    pass

CSS.Minilanguage.ElementNames().register_widget_class(List)
CSS.Minilanguage.ElementNames().register_widget_class(ListItem)
