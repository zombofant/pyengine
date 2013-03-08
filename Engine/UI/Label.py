# File name: Label.py
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

try:
    import Engine.CEngine.Pango as Pango
except (ImportError):
    Pango = None

import CSS.Minilanguage

class Label(object):
    def __init__(self, widget, text=""):
        self._cairo = None
        self._pango = None
        self._layout = None
        self._widget = widget
        self._text = text
        self.Width = None
        self.Height = None
        self._justify = False
        self.invalidate_context()
        self.invalidate_layout()

    def invalidate_context(self):
        self._context_invalidated = True

    def invalidate_layout(self):
        self._layout_invalidated = True

    def _update_context(self):
        if not self._context_invalidated:
            return
        assert self._widget
        self._cairo = self._widget._cairo
        self._pango = self._widget._pango
        self._layout = None
        self._context_invalidated = False
        self.invalidate_layout()

    def _update_layout(self):
        self._update_context()

        if not self._layout_invalidated:
            return

        if not self._layout:
            self._layout = Pango.Layout(self._pango)

        width = self._width if self._width == -1 else self._width * Pango.SCALE
        height = self._height if self._height == -1 else self._height * Pango.SCALE

        self._layout.set_width(width)
        self._layout.set_height(height)

        style = self._widget.ComputedStyle
        self._layout.set_alignment(style.TextAlign)
        self._layout.set_ellipsize(Pango.EllipsizeMode.NONE)
        self._layout.set_justify(False)
        self._layout.set_wrap(Pango.WrapMode.WORD)
        self._layout.set_font_description(style.FontDescription)

        self._layout.set_text(self._text, len(self._text))

        self._layout_invalidated = False

    def get_dimensions(self):
        self._update_layout()
        _, logical = self._layout.get_pixel_extents()
        return logical.width, logical.height

    def render(self, in_box):
        self._update_layout()
        ctx = self._cairo
        style = self._widget.ComputedStyle
        _, logical = self._layout.get_pixel_extents()
        lx = logical.x
        ly = logical.y
        lw = logical.width
        lh = logical.height

        # XXX: Pango docs say, we shall take into account the x and y
        # values of the extent. If we do, however, layout positioning
        # gets all wrong!! WTF?
        # <http://developer.gnome.org/pango/stable/pango-Layout-Objects.html#pango-layout-get-pixel-extents>
        x = in_box.Left # + lx
        y = in_box.Top + style.VerticalAlign(logical.height, in_box.Height) # + ly

        ctx.translate(x, y)
        style.TextColour.set_source(ctx)
        Pango.PangoCairo.show_layout(ctx, self._layout)
        ctx.translate(-x, -y)

    @property
    def Width(self):
        if self._width == -1:
            return None
        return self._width

    @Width.setter
    def Width(self, value):
        if value is None:
            self._width = -1
        else:
            self._width = int(value)
        self.invalidate_layout()

    @property
    def Height(self):
        if self._height == -1:
            return None
        return self._height

    @Height.setter
    def Height(self, value):
        if value is None:
            self._height = -1
        else:
            self._height = int(value)
        self.invalidate_layout()

    @property
    def Text(self):
        return self._text.decode("utf-8")

    @Text.setter
    def Text(self, value):
        self._text = unicode(value).encode("utf-8")
        self.invalidate_layout()

    @property
    def Widget(self):
        return self._widget
