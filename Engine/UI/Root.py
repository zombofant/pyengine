from __future__ import unicode_literals, print_function, division
from our_future import *

import pyglet
import pyglet.window.mouse as mouse
from Widget import Widget
from Screen import Screen
from Flags import *

class Root(Widget):
    def __init__(self, **kwargs):
        super(Widget, self).__init__(**kwargs)
        self._mouseCapture = None
        self._mouseCaptureButton = 0
        self._focused = None
        # self._childClasses = Screen
        self._activeButtonMask = mouse.LEFT | mouse.MIDDLE | mouse.RIGHT
    
    def _findKeyEventTarget(self):
        return self._focused or self

    def _mapMouseEvent(self, x, y):
        target = self._mouseCapture or self.hitTest(x, y)
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
        if target is not self._mouseCapture and button & self._activeButtonMask:
            if Focusable in target.Flags:
                self._focused = target
            self._mouseCapture = target

    def dispatchMouseMove(self, x, y, dx, dy, button, modifiers):
        target, x, y = self._mapMouseEvent(x, y)
        target.onMouseMove(x, y, dx, dy, button, modifiers)

    def dispatchMouseUp(self, x, y, button, modifiers):
        target, x, y = self._mapMouseEvent(x, y)
        target.onMouseUp(x, y, dx, dy, button, modifiers)
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
        
    
