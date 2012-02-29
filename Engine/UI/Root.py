# File name: Root.py
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
from Widget import AbstractWidget, WidgetContainer
from Flags import *

"""
This widget implements dispatching of events and can work as a parent
for other widgets. It also handles focus and mouse capturing. For this
it respects the *Focusable* flag, which must be set by a widget which
wants to recieve the focus.
"""
class RootWidget(AbstractWidget, WidgetContainer):
    def __init__(self, **kwargs):
        super(RootWidget, self).__init__(**kwargs)
        self._mouseCapture = None
        self._mouseCaptureButton = 0
        self._focused = None
        self._flags = frozenset()
        self.ActiveButtonMask = mouse.LEFT | mouse.MIDDLE | mouse.RIGHT
    
    def _findKeyEventTarget(self):
        return self._focused or self

    def _mapMouseEvent(self, x, y):
        target = self._mouseCapture or self.hitTest((x, y))
        return (target, x - target.AbsoluteRect.Left, y - target.AbsoluteRect.Top)
    
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
        target, x, y = self._mapMouseEvent(x, y)
        target.onMouseDown(x, y, button, modifiers)
        if target is not self._mouseCapture and button & self.ActiveButtonMask:
            if Focusable in target._flags:
                self._focused = target
            self._mouseCapture = target

    def dispatchMouseMove(self, x, y, dx, dy, button, modifiers):
        target, x, y = self._mapMouseEvent(x, y)
        target.onMouseMove(x, y, dx, dy, button, modifiers)

    def dispatchMouseUp(self, x, y, button, modifiers):
        target, x, y = self._mapMouseEvent(x, y)
        target.onMouseUp(x, y, button, modifiers)
        if target is self._mouseCapture and button & self._mouseCaptureButton:
            self._mouseCapture = None
            self._mouseCaptureButton = 0

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

    def update(self, timeDelta):
        for child in self:
            child.update(timeDelta)
