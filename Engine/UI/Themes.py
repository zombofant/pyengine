# File name: Themes.py
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

from Widget import Widget

class Theme(object):
    def __init__(self, **kwargs):
        super(Theme, self).__init__(**kwargs)

class WidgetStyle(object):
    def __init__(self, **kwargs):
        super(WidgetStyle, self).__init__(**kwargs)
        self._border = Border()
        self._padding = Padding()
        self._margin = Margin()
        self._background = Background()

    def __iadd__(self, other):
        if not isinstance(other, Rule):
            return NotImplemented
        if hasattr(other, "Border"):
            self._border.assign(other.Border)
        if hasattr(other, "Padding"):
            self._padding.assign(other.Padding)
        if hasattr(other, "Margin"):
            self._margin.assign(other.Margin)
        if hasattr(other, "Background"):
            self._background.assign(other.Background)
        return self
        
    @property
    def Border(self):
        return self._border

    @Border.setter
    def Border(self, value):
        self._border.assign(value)

    @property
    def Padding(self):
        return self._padding

    @Padding.setter
    def Padding(self, value):
        self._padding.assign(value)

    @property
    def Margin(self):
        return self._margin

    @Margin.setter
    def Margin(self, value):
        self._margin.assign(value)

    @property
    def Background(self):
        return self._background

    @Background.setter
    def Background(self, value):
        self._background.assign(value)

    
