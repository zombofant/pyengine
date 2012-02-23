# File name: Application.py
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
import pyglet
import time
import UI.Root

"""
Application and Window base classes.
This primarily provides for handling of multiple-head setups
"""

class Application(UI.Root.Root):

    def __init__(self, geometry=(800, 600), fullscreen=False, **kwargs):
        super(Application, self).__init__(**kwargs)

        self.fullscreen = fullscreen
        self.windows = []

        if fullscreen:
            self._ConstructFullscreen()
        else:
            win = self.makeWin(geometry, (0,0))
            self.windows.append(win)

    def _ConstructFullscreen(self):
        pass

    def makeWin(self, geometry, ui_logical):
        """
        Factory method to create Window objects, should return the
        Window subclass used in the application.
        """
        return Window

    def run(self):
        pyglet.clock.schedule(self.updateUnsynced)
        pyglet.app.run()

    def updateUnsynced(self, timeDelta):
        """
        This method is called once in every game loop iteration. So an
        arbitary amount of time may have passed, which is given by the
        *timeDelta* argument.
        """

    def updateSynced(self):
        """
        This method gets called every *SyncedFrameLength* seconds. It
        may be called several times in sequence if the previous frame
        took too long, but each call still represents the guaranteed
        amount of time specified.
        """

    def _fold_coords(self, win, x, y):
        """
        Fold window coordinates *x*, *y* from window *win* to logical
        UI coordinates
        """
        lx, ly = win.UILogicalCoords
        return lx+x, ly+y

    def _on_close(self, win):
        self._application._on_close(self)

    def _on_draw(self, win):
        pass

    def _on_key_press(self, win, symbol, modifiers):
        self.dispatchKeyDown(symbol, modifiers)

    def _on_key_release(self, win, symbol, modifiers):
        self.dispatchKeyUp(symbol, modifiers)

    def _on_mouse_drag(self, win, x, y, dx, dy, buttons, modifiers):
        lx, ly = self._fold_coords(x, y)
        self.dispatchMouseMotion(lx, ly, dx, dy, buttons, modifiers)

    def _on_mouse_motion(self, win, x, y, dx, dy):
        lx, ly = self._fold_coords(x, y)

        # the last two arguments are modifier bitmaps, as we don't get
        # any just pass none of them
        self.dispatchMouseMotion(lx, ly, dx, dy, 0, 0)


    def _on_mouse_press(self, win, x, y, button, modifiers):
        lx, ly = self._fold_coords(x, y)
        self.dispatchMouseDown(lx, ly, button, modifiers)

    def _on_mouse_release(self, win, x, y, button, modifiers):
        lx, ly = self._fold_coords(x, y)
        self.dispatchMouseUp(lx, ly, button, modifiers)

    def _on_mouse_scroll(self, win, x, y, scroll_x, scroll_y):
        lx, ly = self._fold_coords(x, y)
        self.dispatchScroll(lx, ly, scroll_x, scroll_y)

    def _on_resize(self, win, width, height):
        pass

    def _on_text(self, win, text):
        self.dispatchText(text)

    def _on_text_motion(self, win, motion):
        self.dispatchCaretMotion(motion)

    def _on_text_motion_select(self, win, motion):
        self.dispatchCaretMotionSelect(motion)

class Window(pyglet.window.Window):

    def __init__(self, application, ui_logical,
            initialGeometry=None,
            initialTitle=None,
            **kwargs):
        
        w, h = (int(x) for x in initialGeometry or (800, 600))
        if w < 0 or h < 0:
            self._raiseDimensionsTooSmall(w, h)

        if initialTitle is None:
            initialTitle = type(self).__name__

        super(Window, self).__init__(**kwargs)

        self._application = application

        self._title = initialTitle
        self._terminated = True
        self._syncedFrameLength = 0.01
        self._ui_logical_coords = ui_logical

    @property
    def UILogicalCoords(self):
        return self._ui_logical_coords

    def _raiseDimensionsTooSmall(self, w, h):
        raise ValueError("Width and height must be positive (got w={0}, h={1})".format(w, h))

    def on_close(self):
        self._application._on_close(self)

    def on_context_lost(self):
        raise RuntimeError()

    def on_context_state_lost(self):
        raise RuntimeError()

    def on_draw(self):
        self._application._on_draw(self)

    def on_key_press(self, symbol, modifiers):
        self._application._on_key_press(self, symbol, modifiers)

    def on_key_release(self, symbol, modifiers):
        self._application._on_key_release(self)

    def on_mouse_drag(self, x, y, dx, dy, buttons, modifiers):
        self._application._on_mouse_motion(self, x, y, dx, dy, buttons, modifiers)

    def on_mouse_motion(self, x, y, dx, dy):
        self._application._on_mouse_motion(self, x, y, dx, dy)

    def on_mouse_press(self, x, y, button, modifiers):
        self._application._on_mouse_press(self, x, y, button, modifiers)

    def on_mouse_release(self, x, y, button, modifiers):
        self._application._on_mouse_release(self, x, y, button, modifiers)

    def on_mouse_scroll(self, x, y, scroll_x, scroll_y):
        self._application._on_mouse_scroll(self, x, y, scroll_x, scroll_y)

    def on_resize(self, width, height):
        self._application._on_resize(self, width, height)

    def on_text(self, text):
        self._application._on_text(self, text)

    def on_text_motion(self, motion):
        self._application._on_text_motion(self, motion)

    def on_text_motion_select(self, motion):
        self._application._on_text_motion_select(self, motion)
