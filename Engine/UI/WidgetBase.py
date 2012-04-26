# File name: WidgetBase.py
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

__all__ = ["AbstractWidget", "ParentWidget", "Widget"]

from CSS.Rect import Rect
from CSS.Rules import Rule
from CSS.FaceBuffer import FaceBuffer
from CSS.ClassSet import ClassSet
from Style import Style

try:
    import pyglet
    from OpenGL.GL import GL_TRIANGLES, glEnable, GL_TEXTURE_2D
    from Engine.GL.Texture import Texture2D
except ImportError:
    pass

class AbstractWidget(object):
    """
    Abstract base class for widgets. Do not derive from this if you are not
    creating a root widget. For root widgets, derive from *AbstractWidget*
    and *WidgetContainer*.

    See *Widget* and *ParentWidget* for other widgets.
    """

    def __init__(self, **kwargs):
        super(AbstractWidget, self).__init__(**kwargs)
        self.Visible = True
        self.Enabled = True
        self._relativeRect = Rect(0, 0)
        self._relativeRect._onChange = self._relMetricsChanged
        self._absoluteRect = Rect(0, 0)
        self._absoluteRect._onChange = self._absMetricsChanged
        self._styleRule = None
        self._themeStyle = Style()
        self._invalidateComputedStyle()
        self._styleClasses = ClassSet()
        self._rootWidget = None
        
    def _absMetricsChanged(self):
        self._invalidateAlignment()

    def _invalidateAlignment(self):
        self._invalidatedAlignment = True

    def _invalidateComputedStyle(self):
        self._invalidatedComputedStyle = True
        self._invalidateGeometry()
        self._invalidateAlignment()

    def _invalidateGeometry(self):
        self._invalidatedGeometry = True
        self._geometry = None

    def _relMetricsChanged(self):
        self._invalidateAlignment()

    def realign(self):
        if self._invalidatedAlignment:
            self.doAlign()
            self._invalidatedAlignment = False

    def doAlign(self):
        pass

    def render(self):
        if self._invalidatedGeometry:
            self.realign()
            faceBuffer = FaceBuffer()
            self.ComputedStyle.geometryForRect(self.AbsoluteRect, faceBuffer)
            self._geometry = dict(
                ((tex, pyglet.graphics.vertex_list((len(geometry[0][1]))//2, *geometry)) for tex, geometry in faceBuffer.getGeometry().iteritems())
            )
            del faceBuffer
            self._invalidateGeometry = False
        for tex, vertexList in self._geometry.iteritems():
            if tex is not None:
                self._rootWidget._shader.bind(texturing=True, upsideDown=False).id
                tex.bind()
            else:
                self._rootWidget._shader.bind(texturing=False, upsideDown=False).id
            vertexList.draw(GL_TRIANGLES)
            

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
        self._invalidateAlignment()

    def onScroll(self, scrollX, scrollY):
        return False

    def onTextInput(self, text):
        return False

    def onCaretMotion(self, motion):
        return False

    def onCaretMotionSelect(self, motion):
        return False

    @property
    def StyleRule(self):
        return self._styleRule

    @StyleRule.setter
    def StyleRule(self, value):
        if value is not None and not isinstance(value, Rule):
            raise TypeError("Widget StyleRules must be CSS Rules")
        self._styleRule = value
        self._invalidateComputedStyle()

    @property
    def ThemeStyle(self):
        return self._themeStyle

    @ThemeStyle.setter
    def ThemeStyle(self, value):
        if self._themeStyle == value:
            return
        if not isinstance(value, Style):
            raise TypeError("ThemeStyle must be a Style instance. Got {0} {1}".format(type(value), value))
        self._themeStyle = value
        self._invalidateComputedStyle()

    @property
    def ComputedStyle(self):
        if self._invalidatedComputedStyle:
            self._computedStyle = self._themeStyle + self._styleRule
            self._invalidatedComputedStyle = False
        return self._computedStyle

    @property
    def RelativeRect(self):
        return self._relativeRect

    @RelativeRect.setter
    def RelativeRect(self, value):
        self._relativeRect.assign(value)

    @property
    def AbsoluteRect(self):
        return self._absoluteRect

    @AbsoluteRect.setter
    def AbsoluteRect(self, value):
        self._absoluteRect.assign(value)

    @property
    def StyleClasses(self):
        return self._styleClasses

class Widget(AbstractWidget):
    """
    Base class for non-parent widgets. Use this for any widget which will
    not contain other widgets.

    See *ParentWidget* for parents, *AbstractWidget* for root widgets.
    """
    
    def __init__(self, parent, **kwargs):
        if not isinstance(parent, WidgetContainer):
            raise ValueError("Widget parent must be an instance of WidgetContainer (e.g. ParentWidget).")
        super(Widget, self).__init__(**kwargs)
        self._parent = None
        self._flags = set()
        parent.add(self)
        self._rootWidget = parent._rootWidget

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

    @property
    def Flags(self):
        return self._flags


class WidgetContainer(object):    
    """
    Abstraction of a container which can contain widgets. It is list based
    (thus ordered) and implements a basic typecheck.

    Do not derive from this if you are not implementing a root widget. For
    root widgets, see *AbstractWidget*, for normal widgets which may contain
    other widgets see *ParentWidget*.
    """
    
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
        return None

    def _newChild(self, widget):
        pass

    def add(self, child):
        assert not (child in self._children and not child.Parent == self and not isinstance(child, RootWidget))
        self._checkPotentialChild(child)
        self._children.append(child)
        child._parent = self
        child._parentChanged()
        self._newChild(child)

    def index(self, child):
        return self._children.index(child)

    def treeDepthFirst(self):
        yield self
        for child in self:
            if isinstance(child, WidgetContainer):
                for node in child.treeDepthFirst():
                    yield node
            else:
                yield child


class ParentWidget(Widget, WidgetContainer):
    """
    Base class for widgets which contain other widgets. This derives from
    Widget and WidgetContainer, so all of the benefits apply here.
    """
    
    def __init__(self, parent, **kwargs):
        super(ParentWidget, self).__init__(parent)

    def _newChild(self, widget):
        self._invalidateAlignment()

    def _parentChanged(self):
        super(ParentWidget, self)._parentChanged()
        for child in self:
            child._parentChanged()

    def add(self, widget):
        super(ParentWidget, self).add(widget)
        self._invalidateAlignment()

    def bringToFront(self, key):
        child = self._children[key]
        del self._children[key]
        self._children.append(child)

    def getRootWidget(self):
        return self._rootWidget

    def hitTest(self, p):
        self.realign()
        if not p in self.AbsoluteRect:
            return None
        return self._hitTest(p) or self

    def realign(self):
        super(ParentWidget, self).realign()
        for child in self:
            child.realign()

    def render(self):
        super(ParentWidget, self).render()
        for child in self:
            if child.Visible:
                child.render()

    def sendToBack(self, key):
        child = self._children[key]
        del self._children[key]
        self._children.insert(0, child)

    def update(self, timeDelta):
        for child in self:
            child.update(timeDelta)
