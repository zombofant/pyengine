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
    def __init__(self, **kwargs):
        super(Widget, self).__init__(**kwargs)
        self.Margin = BoxModel.Margin()
        self.Padding = BoxModel.Padding()
        self.Border = BoxModel.Border()
        self._parent = None
        self._childClasses = Widget
        self._flags = set()
        self._children = []

    @property
    def Parent(self):
        return self._parent

    def _checkPotentialChild(self, child):
        if child.Parent is not None:
            raise ValueError("A widget cannot be added multiple times (neither to the same nor to different parents).")
        if not isinstance(child, self._childClasses):
            raise TypeError("Got {0}, but {1} only supports {2} as children.".format(type(child), self, self._childClasses))

    def _requireParent(self):
        if self._parent is None:
            raise ValueError("This operation on {0} requires it to have a parent.".format(self))

    def add(self, child):
        assert isinstance(child, Widget)
        assert not (child in self._children and not child.Parent == self)
        self._checkPotentialChild(child)
        self._children.append(child)
        child._parent = self

    def __contains__(self, child):
        return child in self

    def __len__(self):
        return len(self._children)

    def __iter__(self):
        return iter(self._children)

    def __reversed__(self):
        return reversed(self._children)

    def __getitem__(self, key):
        return self._children.__getitem__(key)

    def __delitem__(self, key):
        l = self._children[key]
        if isinstance(key, slice):
            for child in l:
                child._parent = None
        else:
            l._parent = None
        del self._children[key]

    def index(self, child):
        return self._children.index(child)

    def bringToFront(self, key):
        child = self._children[key]
        del self._children[key]
        self._children.append(child)

    def sendToBack(self, key):
        child = self._children[key]
        del self._children[key]
        self._children.insert(0, child)
    
