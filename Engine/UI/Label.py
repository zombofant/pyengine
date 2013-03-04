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

__all__ = ["LabelWidget"]

try:
    from OpenGL.GL import *
    import Engine.GL.Shader as Shader
except (ImportError, NameError):
    pass

try:
    import Engine.CEngine.Pango as Pango
except (ImportError):
    Pango = None

import Engine.Resources.Manager as Manager
import Engine.Resources.FontLoader

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
        self.invalidateContext()
        self.invalidateLayout()

    def invalidateContext(self):
        self._contextInvalidated = True

    def invalidateLayout(self):
        self._layoutInvalidated = True

    def _updateContext(self):
        if not self._contextInvalidated:
            return
        assert self._widget
        self._cairo = self._widget._cairoContext
        self._pango = self._widget._pangoContext
        self._layout = None
        self.invalidateLayout()

    def _updateLayout(self):
        self._updateContext()

        if not self._layoutInvalidated:
            return

        if not self._layout:
            self._layout = Pango.PangoLayout(self._pango)

        self._layout.Width = self._width
        self._layout.Height = self._height

        # TODO: read these values from the ComputedStyle
        self._layout.Alignment = Pango.Alignment.Left
        self._layout.Ellipsize = Pango.EllipsizeMode.NoEllipsis
        self._layout.Justify = False
        self._layout.Wrap = Pango.WrapMode.Word
        # TODO: pick font + size using ComputedStyle

        self._layout.Text = self._text

    def render(self, inBox):
        self._updateLayout()
        ctx = self._cairo
        lx, ly, lw, lh = self._layout.getLogicalExtents()

        x = inBox.Left + lx
        y = inBox.Top + ly

        ctx.translate(x, y)
        self._widget.ComputedStyle.TextColour.setSource(ctx)
        self._pango.showLayout(self._layout)
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
        self.invalidateLayout()

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
        self.invalidateLayout()

    @property
    def Text(self):
        return self._text

    @Text.setter
    def Text(self, value):
        self._text = unicode(value)

    @property
    def Widget(self):
        return self._widget
