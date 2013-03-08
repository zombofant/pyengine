# File name: IconWidget.py
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

from CSS.Rect import Rect
from CSS.Fill import Image

class Icon(object):
    def __init__(self, icon=None, **kwargs):
        super(Icon, self).__init__(**kwargs)
        self._rect = Rect(0, 0, 0, 0)
        self._icon = None
        self.Icon = icon

    def get_dimensions(self):
        if not self._icon:
            return 0, 0

        return self._icon.Dimensions

    def render(self, ctx):
        if self._icon:
            self._icon.in_cairo(ctx, self._rect)

    @property
    def Icon(self):
        return self._icon

    @Icon.setter
    def Icon(self, value):
        if isinstance(value, basestring):
            self.Icon = Image(value)
            return
        if self._icon == value:
            return
        self._icon = value
        self._rect.Width, self._rect.Height = self.get_dimensions()

    @property
    def X(self):
        return self._rect.X

    @X.setter
    def X(self, value):
        self._rect.X = value

    @property
    def Y(self):
        return self._rect.Y

    @Y.setter
    def Y(self, value):
        self._rect.Y = value
