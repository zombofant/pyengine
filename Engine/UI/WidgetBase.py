# File name: WidgetBase.py
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

__all__ = ["AbstractWidget", "ParentWidget", "Widget"]

import copy

from CSS.Rect import Rect
from CSS.Rules import Rule
from CSS.FaceBuffer import FaceBuffer
from CSS.ClassSet import ClassSet
from Style import Style, BaseStyle

from OpenGL.GL import GL_TRIANGLES, glEnable, GL_TEXTURE_2D
from Engine.GL.Texture import Texture2D

try:
    import Engine.CEngine.GL as CGL
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
        self._parent = None
        self._relativeRect = Rect(0, 0, onChange=self.onResize)
        self._absoluteRect = Rect(0, 0, onChange=self.onResize)
        self._styleRule = None
        self._invalidatedComputedStyle = True
        self._invalidatedAlignment = True
        self._styleClasses = ClassSet()
        self._rootWidget = None
        self._cairoContext = None
        self._pangoContext = None
        self._isHovered = False
        self._isActive = False
        self._isFocused = False
        self._computedStyle = Style()

    def _invalidateComputedStyle(self):
        self._invalidatedComputedStyle = True

    def _invalidateAlignment(self):
        self._invalidatedAlignment = True

    def _themeChanged(self):
        self._invalidateComputedStyle()
        self.ComputedStyle

    def invalidateContext(self):
        pass

    def realign(self):
        if self._invalidatedAlignment or self._invalidatedComputedStyle:
            # the below is neccessary for widgets which do no alignment
            # nor rendering -- otherwise we'll realign them on every
            # frame!
            self.ComputedStyle
            self.doAlign()
            self._invalidatedAlignment = False

    def doAlign(self):
        pass

    def render(self):
        self.ComputedStyle.inCairo(self.AbsoluteRect, self._cairoContext)

    def onKeyDown(self, symbol, modifiers):
        return False

    def onKeyUp(self, symbol, modifiers):
        return False

    def onMouseDown(self, x, y, button, modifiers):
        return False

    def onMouseClick(self, x, y, button, modifiers, nth):
        return False

    def onMouseMove(self, x, y, dx, dy, buttons, modifiers):
        return False

    def onMouseUp(self, x, y, button, modifiers):
        return False

    def onMouseEnter(self):
        pass

    def onMouseLeave(self):
        pass

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

    def invalidate(self):
        root = self.RootWidget
        if not root:
            return
        root.invalidateRect(self.AbsoluteRect)

    def getDimensions(self):
        myStyle = self.ComputedStyle
        borderBox = myStyle.Border.getBox()
        if myStyle.Width is not None:
            width = myStyle.Width + myStyle.Padding.Horizontal + \
                borderBox.Horizontal
        else:
            width = None
        if myStyle.Height is not None:
            height = myStyle.Height + myStyle.Padding.Vertical + \
                borderBox.Vertical
        else:
            height = None

        return width, height

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
    def ComputedStyle(self):
        if self._invalidatedComputedStyle:
            if self.Theme:
                style = copy.copy(self.Theme.getWidgetStyle(self))
            else:
                style = Style()
            style += self._styleRule
            if self.Parent:
                style.solveInheritance(self.Parent.ComputedStyle)
            else:
                style.solveInheritance(BaseStyle())
            diff = self._computedStyle.diff(style)
            if diff:
                self._computedStyle = style
                if Style.Layout in diff:
                    self._invalidateAlignment()
                    if self.Parent:
                        self.Parent._invalidateAlignment()
                if Style.Visual in diff:
                    # force redrawing of the current rect
                    self.invalidate()
            self._invalidatedComputedStyle = False
        return self._computedStyle

    @property
    def RelativeRect(self):
        return self._relativeRect

    @RelativeRect.setter
    def RelativeRect(self, value):
        if value != self._relativeRect:
            self._relativeRect.assign(value)
            self._invalidateAlignment()

    @property
    def AbsoluteRect(self):
        return self._absoluteRect

    @AbsoluteRect.setter
    def AbsoluteRect(self, value):
        if value != self._absoluteRect:
            # the old rect must be invalidated too
            self.invalidate()
            self._absoluteRect.assign(value)
            self._invalidateAlignment()

    @property
    def StyleClasses(self):
        return self._styleClasses

    @property
    def CSSState(self):
        return (self._isHovered, self._isActive, self._isFocused)

    @property
    def IsHovered(self):
        return self._isHovered

    @IsHovered.setter
    def IsHovered(self, value):
        value = bool(value)
        if value == self._isHovered:
            return
        self._isHovered = value
        self._invalidateComputedStyle()

    @property
    def IsFocused(self):
        return self._isFocused

    @IsFocused.setter
    def IsFocused(self, value):
        value = bool(value)
        if value == self._isFocused:
            return
        self._isFocused = value
        self._invalidateComputedStyle()

    @property
    def IsActive(self):
        return self._isActive

    @IsHovered.setter
    def IsActive(self, value):
        value = bool(value)
        if value == self._isActive:
            return
        self._isActive = value
        self._invalidateComputedStyle()

    @property
    def ClientRect(self):
        rect = copy.copy(self.AbsoluteRect)
        rect.shrink(self.ComputedStyle.Border.getBox())
        return rect

    @property
    def RootWidget(self):
        return self._rootWidget

    @property
    def Theme(self):
        if self._rootWidget:
            return self._rootWidget.Theme
        else:
            return None

class Widget(AbstractWidget):
    """
    Base class for non-parent widgets. Use this for any widget which will
    not contain other widgets.

    See *ParentWidget* for parents, *AbstractWidget* for root widgets.
    """

    def __init__(self, parent, **kwargs):
        super(Widget, self).__init__(**kwargs)
        self._parent = None
        self._flags = set()
        self._rootWidget = None
        self._cairoContext = None
        self._pangoContext = None
        if parent is not None:
            parent.add(self)

    def _requireParent(self):
        if self._parent is None:
            raise ValueError("This operation on {0} requires it to have a parent.".format(self))

    def _parentChanged(self):
        if self._parent is not None:
            self._rootWidget = self._parent.RootWidget
        else:
            self._rootWidget = None
        if self._rootWidget is not None:
            self._cairoContext = self._rootWidget._cairoContext
            self._pangoContext = self._rootWidget._pangoContext
        else:
            self._cairoContext = None
            self._pangoContext = None
        self.IsFocused = False
        self.IsHovered = False
        self.IsActive = False
        self.invalidateContext()
        self._themeChanged()

    def hitTestWithChain(self, p):
        return [self] if p in self.AbsoluteRect else False

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

    def _hitTestWithChain(self, p):
        for child in self:
            hit = child.hitTestWithChain(p)
            if hit is not False:
                return hit
        return False

    def _newChild(self, widget):
        pass

    def add(self, child):
        assert not (child in self._children and not child.Parent == self and not isinstance(child, RootWidget))
        self._checkPotentialChild(child)
        self._children.append(child)
        child._parent = self
        child._parentChanged()
        self._newChild(child)
        self._invalidateAlignment()

    def index(self, child):
        return self._children.index(child)

    def remove(self, child):
        self._children.remove(child)
        child._parent = None
        child._parentChanged()
        self._invalidateAlignment()

    def treeDepthFirst(self):
        yield self
        for child in self:
            if isinstance(child, WidgetContainer):
                for node in child.treeDepthFirst():
                    yield node
            else:
                yield child

    def updateRenderingContext(self):
        for widget in self.treeDepthFirst():
            widget._cairoContext = widget._rootWidget._cairoContext
            widget._pangoContext = widget._rootWidget._pangoContext
            widget.invalidateContext()


class ParentWidget(Widget, WidgetContainer):
    """
    Base class for widgets which contain other widgets. This derives from
    Widget and WidgetContainer, so all of the benefits apply here.
    """

    def __init__(self, parent, **kwargs):
        super(ParentWidget, self).__init__(parent)

    def _newChild(self, widget):
        pass

    def _parentChanged(self):
        super(ParentWidget, self)._parentChanged()
        for child in self:
            child._parentChanged()

    def add(self, widget):
        super(ParentWidget, self).add(widget)
        self._newChild(widget)

    def bringToFront(self, key):
        child = self._children[key]
        del self._children[key]
        self._children.append(child)

    def hitTestWithChain(self, p):
        self.realign()
        if not p in self.AbsoluteRect:
            return False
        chain = self._hitTestWithChain(p)
        if chain is not False:
            chain.append(self)
            return chain
        else:
            return [self]

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
            if child.Visible and child.AbsoluteRect.Area > 0:
                child.render()

    def sendToBack(self, key):
        child = self._children[key]
        del self._children[key]
        self._children.insert(0, child)

    def update(self, timeDelta):
        for child in self:
            child.update(timeDelta)
