# File name: ScreenWidget.py
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

__all__ = ["ScreenWidget"]

import copy

import CSS.Minilanguage

from WidgetBase import ParentWidget

class ScreenWidget(ParentWidget):
    """
    Represents an operating system screen or window.

    This is used by the RootWidget *Application* to manage windows.
    """

    def __init__(self, parent, window, **kwargs):
        super(ScreenWidget, self).__init__(parent, **kwargs)
        self._window = window

    def doAlign(self):
        for child in self:
            rect = copy.copy(self.AbsoluteRect)
            childStyle = child.ComputedStyle
            rect.Left += childStyle.Margin.Left
            rect.Right -= childStyle.Margin.Right
            rect.Top += childStyle.Margin.Top
            rect.Bottom -= childStyle.Margin.Bottom
            child.AbsoluteRect = rect

    @property
    def Window(self):
        return self._window

CSS.Minilanguage.ElementNames().registerWidgetClass(ScreenWidget, "Screen")
