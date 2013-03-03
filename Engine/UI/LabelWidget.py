# File name: LabelWidget.py
# This file is part of: pyuni
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
# For feedback and questions about pyuni please e-mail one of the
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

from WidgetBase import Widget

class LabelWidget(Widget):
    def __init__(self, parent, text="", **kwargs):
        assert Pango
        # FIXME: Font rendering
        # font = Manager.ResourceManager().require('/data/fonts/Cantarell-Regular.otf', fontFamily="Cantarell", size=10)
        # self._text = pyglet.text.Label(text, font_name="Cantarell", font_size=10, bold=True, italic=False, anchor_y='top')
        super(LabelWidget, self).__init__(parent, **kwargs)
        self._layout = Pango.PangoLayout(self._pangoContext)
        self._text = None
        self.Text = text

    def doAlign(self):
        # self._text.x = self.AbsoluteRect.Left
        # self._text.y = self._rootWidget.AbsoluteRect.Height - self.AbsoluteRect.Top
        self._layout.Width = self.AbsoluteRect.Width
        # self._layout.Height = self.AbsoluteRect.Height

    def render(self):
        # Shader.unbind()
        # FIXME/pyglet
        # self._text.draw()
        ctx = self._cairoContext
        ctx.translate(self.AbsoluteRect.Left, self.AbsoluteRect.Top)
        self._pangoContext.showLayout(self._layout)
        ctx.translate(-self.AbsoluteRect.Left, -self.AbsoluteRect.Top)
        pass

    @property
    def Text(self):
        return self._text

    @Text.setter
    def Text(self, value):
        if self._text == value:
            return
        self._text = value
        self._layout.Text = self._text

CSS.Minilanguage.ElementNames().registerWidgetClass(LabelWidget)
