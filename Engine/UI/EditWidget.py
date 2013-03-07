# File name: EditWidget.py
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

__all__ = ["Edit"]

import copy

try:
    import Engine.CEngine.Pango as Pango
except ImportError:
    Pango = None

from Engine.CEngine.Window import motion

import CSS.Minilanguage
from CSS.Rect import Rect

from WidgetBase import Widget
import Flags

class AbstractEdit(Widget):
    def __init__(self, parent, **kwargs):
        self._layout = None
        self._layout_invalidated = True
        self._text = b""
        super(AbstractEdit, self).__init__(parent, **kwargs)
        self._cursor_index = 0
        self._cursor_rect = Rect(0, 0, 0, 0)
        self._flags = {Flags.Focusable}

    def _invalidate_computed_style(self):
        super(AbstractEdit, self)._invalidate_computed_style()
        self._layout_invalidated = True

    def invalidate_context(self):
        self._layout = None
        self._layout_invalidated = True

    def _update_layout(self):
        if not self._layout_invalidated and self._layout:
            return

        if not self._layout:
            self._layout = Pango.Layout(self._pango)

        self._layout.set_width(-1)
        self._layout.set_height(-1)

        style = self.ComputedStyle
        self._layout.set_alignment(Pango.Alignment.LEFT)
        self._layout.set_ellipsize(Pango.EllipsizeMode.NONE)
        self._layout.set_justify(False)
        self._layout.set_wrap(Pango.WrapMode.WORD)
        self._layout.set_font_description(style.FontDescription)

        self._layout.set_text(self._text, len(self._text))

        self._layout_invalidated = False

        self._update_cursor_rect()

    def render(self):
        super(AbstractEdit, self).render()
        self._update_layout()

        ctx = self._cairo
        style = self.ComputedStyle

        rect = copy.copy(self.AbsoluteRect)
        rect.shrink(style.Padding)
        rect.shrink(style.Border.get_box())

        ctx.translate(rect.X, rect.Y)
        style.TextColour.set_source(ctx)
        Pango.PangoCairo.show_layout(ctx, self._layout)
        ctx.translate(-rect.X, -rect.Y)

        if self._is_focused:
            cursor_rect = copy.copy(self._cursor_rect)
            cursor_rect.transpose(rect.X, rect.Y)
            ctx.rectangle(*cursor_rect.XYWH)
            ctx.fill()

    def _update_cursor_rect(self):
        strong_pos, _ = self._layout.get_cursor_pos(self._cursor_index)
        self._cursor_rect = Rect(strong_pos.x / Pango.SCALE, strong_pos.y / Pango.SCALE)
        self._cursor_rect.Width = 2
        self._cursor_rect.Height = strong_pos.height / Pango.SCALE

    def _cursor_index_to_pango(self, cur_index):
        cur_trailing = 0
        if cur_index >= len(self._text):
            cur_index = len(self._text)
            cur_trailing = 0
        return cur_index, cur_trailing

    def _pango_to_cursor_index(self, index, trailing):
        if trailing != 0:
            iterator = self._layout.get_iter()
            while iterator.get_index() != index:
                if not iterator.next_char():
                    break
            else:
                for i in xrange(trailing):
                    iterator.next_char()
                index = iterator.get_index()
        if index >= len(self._text):
            index = len(self._text)
        elif index < 0:
            index = 0
        return index

    def _move_cursor(self, by_characters):
        if by_characters == 0:
            return

        direction = 1
        if by_characters < 0:
            by_characters = -by_characters
            direction = -1

        cur_index, cur_trailing = self._cursor_index_to_pango(self._cursor_index)

        print(cur_index, cur_trailing)

        for i in xrange(by_characters):
            cur_index, cur_trailing = self._layout.move_cursor_visually(
                True,
                cur_index,
                cur_trailing,
                direction)
            if cur_index < 0:
                break
            elif cur_index > len(self._text):
                break

        print(cur_index, cur_trailing)

        self._cursor_index = self._pango_to_cursor_index(cur_index, cur_trailing)
        self._update_cursor_rect()
        self.invalidate()

    def _insert_character(self, text):
        self._update_layout()

        self._text = self._text[:self._cursor_index] + \
            text + self._text[self._cursor_index:]
        self._layout.set_text(self._text, len(self._text))

        self._cursor_index += len(text)
        self._update_cursor_rect()

    def _delete_section(self, start, end):
        self._text = self._text[:start] + self._text[end:]
        self._layout.set_text(self._text, len(self._text))
        self._update_cursor_rect()

    def _backspace(self):
        if len(self._text) == 0:
            return
        old_index = self._cursor_index
        if old_index == 0:
            return

        self._move_cursor(-1)
        new_index = self._cursor_index
        self._delete_section(new_index, old_index)

    def _delete(self):
        if len(self._text) == 0:
            return
        old_index = self._cursor_index
        if old_index >= len(self._text):
            return

        self._move_cursor(1)
        new_index = self._cursor_index
        self._cursor_index = old_index
        self._delete_section(old_index, new_index)

    def on_text_input(self, text):
        self._update_layout()
        if text == b"\x08":  # backspace
            self._backspace()
        elif text == b"\x7f":  # delete
            self._delete()
        else:
            self._insert_character(text)
        self.invalidate()
        return True

    def on_caret_motion(self, direction, step):
        if step == motion.StepChar:
            self._update_layout()
            if direction == motion.Left:
                self._move_cursor(-1)
                return True
            elif direction == motion.Right:
                self._move_cursor(1)
                return True
            elif direction == motion.End:
                self._cursor_index = len(self._text)
                self._update_cursor_rect()
                self.invalidate()
                return True
            elif direction == motion.Begin:
                self._cursor_index = 0
                self._update_cursor_rect()
                self.invalidate()
                return True

    def on_mouse_down(self, x, y, button, modifiers):
        self._update_layout()
        style = self.ComputedStyle
        x -= style.Padding.Left + style.Border.Left.Width
        y -= style.Padding.Top + style.Border.Top.Width
        x *= Pango.SCALE
        y *= Pango.SCALE
        exact, index, trailing = self._layout.xy_to_index(int(x), int(y))
        print(index, trailing)
        self._cursor_index = self._pango_to_cursor_index(index, trailing)
        self._update_cursor_rect()
        self.invalidate()
        return True

class Edit(AbstractEdit):
    pass

CSS.Minilanguage.ElementNames().register_widget_class(Edit)
