# File name: RootWidget.py
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

__all__ = ["RootWidget"]

from Engine.pygletHeadless import mouse
from WidgetBase import AbstractWidget, WidgetContainer
from LayerWidget import LayerWidget, DesktopLayer, WindowLayer, PopupLayer
from Flags import *

class RootWidget(AbstractWidget, WidgetContainer):
    """
    This widget implements dispatching of events and can work as a parent
    for other widgets. It also handles focus and mouse capturing. For this
    it respects the *Focusable* flag, which must be set by a widget which
    wants to recieve the focus.
    """

    def __init__(self, **kwargs):
        super(RootWidget, self).__init__(**kwargs)
        self._rootWidget = self
        self._mouseCapture = None
        self._mouseCaptureButton = 0
        self._focused = None
        self._flags = frozenset()
        self._childClasses = LayerWidget
        self._popupLayer = PopupLayer(self)
        self._windowLayer = WindowLayer(self)
        self._desktopLayer = DesktopLayer(self)
        self._oldHitChain = frozenset()
        self.ActiveButtonMask = mouse.LEFT | mouse.MIDDLE | mouse.RIGHT

    def _findKeyEventTarget(self):
        return self._focused or self

    def _mapMouseEvent(self, x, y, hitChain=None):
        target = self._mouseCapture or \
            ((hitChain[0] if hitChain is not False else None)
             if hitChain is not None else self._hitTest((x, y)))
        return (target, x - target.AbsoluteRect.Left, y - target.AbsoluteRect.Top)

    def _updateHoverState(self, hitChain=None, p=None):
        if hitChain is None:
            hitChain = self._hitTestWithChain(p)
        hitChain = frozenset(hitChain)
        oldHitChain = self._oldHitChain
        if hitChain != oldHitChain:
            for non_hovered in oldHitChain - hitChain:
                non_hovered._isHovered = False
                non_hovered._invalidateComputedStyle()
            for hovered in hitChain - oldHitChain:
                hovered._isHovered = True
                hovered._invalidateComputedStyle()
            self._oldHitChain = hitChain

    def _focusAndCapture(self, hitChain, button):
        target = None
        for candidate in hitChain:
            if Focusable in candidate._flags:
                target = candidate
                break
        else:
            return

        if self._focused is not None:
            self._focused._isFocused = False
            self._focused._invalidateComputedStyle()
        self._focused = target
        target._isFocused = True
        target._invalidateComputedStyle()

        self._mouseCapture = target
        self._mouseCaptureButton = button

    def doAlign(self):
        assert len(self) == 3
        myRect = self.AbsoluteRect
        self._desktopLayer.AbsoluteRect = myRect
        self._windowLayer.AbsoluteRect = myRect
        self._popupLayer.AbsoluteRect = myRect

    def dispatchKeyDown(self, *args):
        target = self._findKeyEventTarget()
        handled = target.onKeyDown(*args)
        if not handled and target is not self:
            self.onKeyDown(*args)

    def dispatchKeyUp(self, *args):
        target = self._findKeyEventTarget()
        handled = target.onKeyUp(*args)
        if not handled and target is not self:
            self.onKeyUp(*args)

    def dispatchMouseDown(self, x, y, button, modifiers):
        if self._mouseCapture is None:
            hitChain = self._hitTestWithChain((x, y))
        else:
            hitChain = None
        target, x, y = self._mapMouseEvent(x, y, hitChain)
        target.onMouseDown(x, y, button, modifiers)
        if self._mouseCapture is None and button & self.ActiveButtonMask:
            self._focusAndCapture(hitChain, button)

    def dispatchMouseMove(self, x, y, dx, dy, button, modifiers):
        if self._mouseCapture is None:
            hitChain = self._hitTestWithChain((x, y))
        else:
            hitChain = None
        target, x, y = self._mapMouseEvent(x, y, hitChain)
        target.onMouseMove(x, y, dx, dy, button, modifiers)

        if self._mouseCapture is None:
            self._updateHoverState(hitChain)

    def dispatchMouseUp(self, x, y, button, modifiers):
        target, x, y = self._mapMouseEvent(x, y)
        target.onMouseUp(x, y, button, modifiers)
        if target is self._mouseCapture and button & self._mouseCaptureButton:
            self._mouseCapture = None
            self._mouseCaptureButton = 0
            self._updateHoverState(p=(x,y))

    def dispatchScroll(self, x, y, scrollX, scrollY):
        target, x, y = self._mapMouseEvent(x, y)
        target.onScroll(scrollX, scrollY)

    def dispatchTextInput(self, text):
        target = self._findKeyEventTarget()
        target.onTextInput(text)

    def dispatchCaretMotion(self, motion):
        target = self._findKeyEventTarget()
        target.onCaretMotion(motion)

    def dispatchCaretMotionSelect(self, motion):
        target = self._findKeyEventTarget()
        target.onCaretMotionSelect(motion)

    def getRootWidget(self):
        return self

    def realign(self):
        super(RootWidget, self).realign()
        for child in self:
            child.realign()

    def render(self):
        self._desktopLayer.render()
        self._windowLayer.render()
        self._popupLayer.render()

    def update(self, timeDelta):
        for child in self:
            child.update(timeDelta)
