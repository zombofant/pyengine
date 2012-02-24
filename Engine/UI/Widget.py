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
from Rect import Rect

"""
Abstract base class for widgets. Do not derive from this if you are not
creating a root widget. For root widgets, derive from *AbstractWidget*
and *WidgetContainer*.

See *Widget* and *ParentWidget* for other widgets.
"""
class AbstractWidget(object):
    def __init__(self, **kwargs):
        super(AbstractWidget, self).__init__(**kwargs)
        self.Visible = True
        self.Enabled = True
        self.RelativeRect = Rect(0, 0)
        self.RelativeRect._onChange = self._relMetricsChanged
        self.Rect = Rect(0, 0)
        self.Rect._onChange = self._absMetricsChanged
        
    def _absMetricsChanged(self):
        self.onResize()

    def _relMetricsChanged(self):
        pass

    def align(self):
        pass

    def onKeyDown(self, symbol, modifiers):
        return False

    def onKeyUp(self, symbol, modifiers):
        return False

    def onMouseDown(self, x, y, buttons, modifiers):
        return False

    def onMouseMove(self, x, y, dx, dy, buttons, modifiers):
        return False

    def onMouseUp(self, x, y, buttons, modifiers):
        return False

    def onResize(self):
        self.align()

    def onScroll(self, scrollX, scrollY):
        return False

    def onTextInput(self, symbol, modifiers):
        return False

"""
Base class for non-parent widgets. Use this for any widget which will
not contain other widgets.

See *ParentWidget* for parents, *AbstractWidget* for root widgets.
"""
class Widget(AbstractWidget):
    def __init__(self, parent, **kwargs):
        if not isinstance(parent, WidgetContainer):
            raise ValueError("Widget parent must be an instance of WidgetContainer (e.g. ParentWidget).")
        super(Widget, self).__init__(**kwargs)
        self.Margin = BoxModel.Margin()
        self.Padding = BoxModel.Padding()
        self.Border = BoxModel.Border()
        self._parent = None
        self._flags = set()
        parent.add(self)

    def _requireParent(self):
        if self._parent is None:
            raise ValueError("This operation on {0} requires it to have a parent.".format(self))
        
    def _parentChanged(self):
        assert self._parent is None or isinstance(self._parent, WidgetContainer)
        if self._parent is not None:
            self._rootWidget = self._parent.getRootWidget()
        else:
            self._rootWidget = None

    def hitTest(self, p):
        return self if p in self.AbsoluteRect else None

    def clientToAbsolute(self, p):
        return (p[0] + self.AbsoluteRect.X, p[1] + self.AbsoluteRect.Y)

    def clientToParent(self, p):
        return (p[0] + self._left, p[1] + self._top)

    def parentToClient(self, p):
        return (p[0] - self._left, p[1] - self._top)

    def update(self, deltaT):
        pass

    @property
    def Parent(self):
        return self._parent

"""
Abstraction of a container which can contain widgets. It is list based
(thus ordered) and implements a basic typecheck.

Do not derive from this if you are not implementing a root widget. For
root widgets, see *AbstractWidget*, for normal widgets which may contain
other widgets see *ParentWidget*.
"""
class WidgetContainer(object):
    def __init__(self, **kwargs):
        super(WidgetContainer, self).__init__(**kwargs)
        self._childClasses = Widget
        self._children = []

    def __contains__(self, child):
        return child in self._children

    def __delitem__(self, key):
        l = self._children[key]
        if isinstance(key, slice):
            for child in l:
                child._parent = None
                child._parentChanged()
        else:
            l._parent = None
            l._parentChanged()
        del self._children[key]

    def __getitem__(self, key):
        return self._children.__getitem__(key)

    def __iter__(self):
        return iter(self._children)

    def __len__(self):
        return len(self._children)

    def __reversed__(self):
        return reversed(self._children)

    def _checkPotentialChild(self, child):
        if not isinstance(child, self._childClasses):
            raise TypeError("Got {0}, but {1} only supports {2} as children.".format(type(child), self, self._childClasses))
        if child.Parent is not None:
            raise ValueError("A widget cannot be added multiple times (neither to the same nor to different parents).")

    def _hitTest(self, p):
        for child in self:
            hit = child.hitTest(p)
            if hit is not None:
                return hit
        return hit

    def add(self, child):
        assert not (child in self._children and not child.Parent == self and not isinstance(child, RootWidget))
        self._checkPotentialChild(child)
        self._children.append(child)
        child._parent = self
        child._parentChanged()

    def index(self, child):
        return self._children.index(child)

"""
Base class for widgets which contain other widgets. This derives from
Widget and WidgetContainer, so all of the benefits apply here.
"""
class ParentWidget(Widget, WidgetContainer):
    def __init__(self, parent, **kwargs):
        super(ParentWidget, self).__init__(parent)

    def _parentChanged(self):
        super(ParentWidget, self)._parentChanged()
        for child in self:
            child._parentChanged()

    def bringToFront(self, key):
        child = self._children[key]
        del self._children[key]
        self._children.append(child)

    def getRootWidget(self):
        return self._rootWidget

    def hitTest(self, p):
        if not p in self.Rect:
            return None
        return self._hitTest(p) or self

    def sendToBack(self, key):
        child = self._children[key]
        del self._children[key]
        self._children.insert(0, child)
