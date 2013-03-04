# File name: LabelWidget.py
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

import copy

import Engine.Resources.Manager as Manager
import Engine.Resources.FontLoader

import CSS.Minilanguage

from WidgetBase import Widget
import Label

class LabelledWidget(Widget):
    def __init__(self, parent, **kwargs):
        self._label = Label.Label(self)
        super(LabelledWidget, self).__init__(parent, **kwargs)

    def _parentChanged(self):
        super(LabelledWidget, self)._parentChanged()
        self._label.invalidateContext()

    def _invalidateComputedStyle(self):
        super(LabelledWidget, self)._invalidateComputedStyle()
        self._label.invalidateLayout()

    def doAlign(self):
        self._label.Width = self.AbsoluteRect.Width
        self._label.Height = self.AbsoluteRect.Height

    def render(self):
        super(LabelledWidget, self).render()
        label_rect = copy.copy(self.AbsoluteRect)
        label_rect.shrink(self.ComputedStyle.Padding)
        self._label.render(label_rect)

class LabelWidget(LabelledWidget):
    @property
    def Text(self):
        return self._text

    @Text.setter
    def Text(self, value):
        self._label.Text = value

CSS.Minilanguage.ElementNames().registerWidgetClass(LabelWidget)
