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
from UI.Root import RootWidget
from UI.Screen import Screen

__docformat__ = "restructuredtext"

"""
Application and Window base classes.
This primarily provides for handling of multiple-head setups
"""

class Application(RootWidget):
    def __init__(self, geometry=(800, 600), fullscreen=False, **kwargs):
        super(Application, self).__init__(**kwargs)
        self.fullscreen = fullscreen
        self.windows = []
        self._childClasses = Screen

        if fullscreen:
            self._constructFullscreen()
        else:
            self._newScreen((0, 0), geometry)

    def _constructFullscreen(self):
        """
        Detect screens and create screen widgets for each. Also create
        the ui coordinate map.
        """
        display = pyglet.window.get_platform().get_default_display()
        screens = display.get_screens()
        default = display.get_default_screen()
        
        # convert the screen list in a list we can use
        screens = [[i, screen, screen.x, screen.y, screen.width, screen.height, screen is default] for i, screen in enumerate(screens)]
        minX = min((screen[2] for screen in screens))
        minY = min((screen[3] for screen in screens))
        if minX != 0 or minY != 0:
            for screen in screens:
                screen[2] -= minX
                screen[3] -= minY
        
        for i, screen, x, y, w, h, primary in screens:
            self._newScreen((x, y), (w, h), True, primary, screen)

    def _newScreen(self, ui_logical, geometry, fullscreen=False, primary=True, screen=None):        
        window = self.makeWin(ui_logical, None if fullscreen else geometry, screen)
        widget = Screen(self)
        widget.Left, widget.Top = ui_logical
        widget.Width, widget.Height = geometry
        self.windows.append((window, widget))

    def makeWin(self, ui_logical, geometry=None, screen=None):
        """
        Factory method to create Window objects, should return the
        Window subclass used in the application.
        
        :Parameters:
            `ui_logical` : (int, int)
                UI logical coordinates. These may differ from the window 
                coordinates.
            `geometry` : (int, int) or *None*
                This must be ``(width, height)`` for a non-fullscreen
                window or None for a fullscreen window.
            `screen` : pyglet.window.Screen or *None*
                This parameter is mandatory if no geometry is given to
                determine the screen to create the window on. Otherwise
                it internally defaults to the default screen.
        """
        if geometry is None:
            if screen is None:
                raise ValueError("screen must be given if a fullscreen window is to be created.")
            return Window(self, ui_logical, fullscreen=True, screen=screen)
        else:
            return Window(self, ui_logical, width=geometry[0], height=geometry[1], screen=screen)

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
            **kwargs):
        
        super(Window, self).__init__(**kwargs)

        self._application = application

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
