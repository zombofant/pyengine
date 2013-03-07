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
import CSS.Rect as Rect

from WidgetBase import ParentWidget

class ScreenWidget(ParentWidget):
    """
    Represents an operating system screen or window.

    This is used by the RootWidget *Application* to manage windows.
    """

    def __init__(self, parent, window, **kwargs):
        super(ScreenWidget, self).__init__(parent, **kwargs)
        self._window = window

    def do_align(self):
        for child in self:
            rect = copy.copy(self.AbsoluteRect)
            rect.shrink(self.ComputedStyle.Padding)

            childstyle = child.ComputedStyle
            margin = copy.copy(childstyle.Margin)
            margin.deautoify(
                Rect.Rect(
                    0, 0,
                    child.ComputedStyle.Width or rect.Width,
                    child.ComputedStyle.Height or rect.Height
                    ),
                rect
                )

            rect.Left += margin.Left
            rect.Right -= margin.Right
            rect.Top += margin.Top
            rect.Bottom -= margin.Bottom
            child.AbsoluteRect = rect

    @property
    def Window(self):
        return self._window

CSS.Minilanguage.ElementNames().register_widget_class(ScreenWidget, "Screen")
