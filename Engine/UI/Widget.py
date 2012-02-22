# File name: Widget.py
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
import BoxModel

class Widget(object):
    Margin = BoxModel.Margin
    Padding = BoxModel.Padding
    Border = BoxModel.Border
    _parent = None
    _childClasses = Widget
    _flags = set()
    _children = []
    
    def __init__(self, **kwargs):
        super(Widget, self).__init__(**kwargs)

    @property
    def Parent(self):
        return self._parent

    @Parent.setter
    def Parent(self, value):
        if self._parent == value:
            return
        if self._parent is not None:
            self._parent.remove(self)
        self._parent = value
        if self._parent is not None:
            self._parent.add(self)

    def add(self, child):
        assert isinstance(child, Widget)
        
