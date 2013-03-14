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
import itertools

import CSS.Minilanguage

from WidgetBase import AbstractWidget, ParentWidget, Widget
from Label import Label
from LabelWidget import LabelledWidget
from ScrollWidget import ScrollBar, ScrollMode

import Flags

import CSS.Rect as Rect

class ListItem(AbstractWidget):
    def on_mouse_click(self, x, y, button, modifiers, nth):
        self.Parent.select_item(self)

class TextListItem(LabelledWidget, ListItem):
    def __init__(self, parent, text, **kwargs):
        super(TextListItem, self).__init__(parent, **kwargs)
        self._label.Text = text

class AbstractList(ParentWidget):
    def __init__(self, parent, on_dbl_click=None, **kwargs):
        super(AbstractList, self).__init__(parent, **kwargs)

        self._vbar = ScrollBar(self, ScrollMode.VERTICAL,
                               on_scroll=self._scroll_vertical)
        self._hbar = ScrollBar(self, ScrollMode.HORIZONTAL,
                               on_scroll=self._scroll_horizontal)

        self._items = []
        self._selected_item = None
        self._item_index = -1
        self._clip_rect = Rect.Rect(0, 0, 0, 0)
        self._flags = {Flags.Focusable}
        self._on_dbl_click = on_dbl_click
        self._min_item = 0
        self._max_item = 0

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

        min_index = len(self)
        max_index = -1

        y = rect.Top - self._vbar.Position
        for i, item in enumerate(self._items):
            item_style = item.ComputedStyle
            item_margin = item_style.Margin
            _, item_height = item.get_dimensions()

            if item_height is None:
                continue

            if y <= rect.Bottom:
                max_index = max(max_index, i)

            y += item_margin.Top

            item.AbsoluteRect = Rect.Rect(
                rect.Left + item_margin.Left,
                y,
                rect.Right - item_margin.Right,
                y + item_height)

            y += item_height + item_margin.Bottom

            if y >= rect.Top:
                min_index = min(min_index, i)

        self._min_index = min_index
        self._max_index = max_index + 1
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

    def clear(self):
        for item in list(self._items):
            self.remove(item)
        self._vbar.Position = 0
        self._hbar.Position = 0
        self.invalidate()

    def select_item(self, item):
        self.SelectedItem = item

    def _hit_test(self, p):
        for child in [self._vbar, self._hbar] + self._items[self._min_index:self._max_index]:
            if not child.Visible:
                continue
            hit = child.hit_test(p)
            if hit is not None:
                return hit
        return None

    def _hit_test_with_chain(self, p):
        for child in [self._vbar, self._hbar] + self._items[self._min_index:self._max_index]:
            if not child.Visible:
                continue
            hit = child.hit_test_with_chain(p)
            if hit is not False:
                return hit
        return False

    def render(self):
        AbstractWidget.render(self)
        ctx = self._cairo
        ctx.save()
        try:
            ctx.rectangle(*self._clip_rect.XYWH)
            ctx.clip()
            for item in self._items[self._min_index:self._max_index]:
                item.render()
        finally:
            ctx.restore()

        if self._vbar.Visible:
            self._vbar.render()
        if self._hbar.Visible:
            self._hbar.render()

    def on_scroll(self, scrollX, scrollY):
        if self._vbar.Visible:
            self._vbar.scroll_by(-scrollY * self._vbar.Step)
        if self._hbar.Visible:
            self._hbar.scroll_by(scrollX * self._hbar.Step)

    def on_mouse_click(self, x, y, button, modifiers, nth):
        print(nth)
        print(self._on_dbl_click)
        if nth == 2 and self._on_dbl_click:
            self._on_dbl_click(self)
            return True

    @property
    def SelectedItem(self):
        return self._selected_item

    @SelectedItem.setter
    def SelectedItem(self, item):
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

    @property
    def ItemIndex(self):
        return self._item_index

    @ItemIndex.setter
    def ItemIndex(self, value):
        value = int(value)
        if value < 0:
            self.SelectedItem = None
        elif value < len(self._items):
            self.SelectedItem = self._items[value]
        else:
            raise ValueError("ItemIndex out of bounds.")

class List(AbstractList):
    pass

CSS.Minilanguage.ElementNames().register_widget_class(List)
CSS.Minilanguage.ElementNames().register_widget_class(ListItem)
