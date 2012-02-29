# File name: Screen.py
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

__all__ = ["ScreenWidget"]

from Widget import ParentWidget

class ScreenWidget(ParentWidget):
    """
    Represents an operating system screen or window.

    This is used by the RootWidget *Application* to manage windows.
    """
    
    def __init__(self, parent, window, **kwargs):
        super(ScreenWidget, self).__init__(parent, **kwargs)
        self._window = window

    def align(self):
        for child in self:
            child.AbsoluteRect.assign(self.Rect)

    @property
    def Window(self):
        return self._window
