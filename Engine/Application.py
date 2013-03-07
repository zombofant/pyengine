# File name: Application.py
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

try:
    import Engine.CEngine
    import Engine.CEngine.Window as CWindow
    import Engine.CEngine.GL as CGL
except ImportError:
    # XXX: make documentation work
    class CWindow(object):
        class EventSink(object):
            pass

from Engine.CEngine.Log import server, Severity
log = server

import time
from OpenGL.GL import *
from OpenGL.GL.framebufferobjects import *

from UI import Rect, RootWidget, ScreenWidget, SceneWidget
from GL import Framebuffer, Texture2D, Renderbuffer, make_pot

__docformat__ = "restructuredtext"

class Application(RootWidget, CWindow.EventSink):
    """
    This is the core class for any PyEngine application. It manages
    the display and handles fullscreen / windowed / multi-head setup.

    *display* must be a :class:`CWindow.Display` instance, whose
    subclass will be platform-specific. *geometry* must be a 2-tuple
    giving the width and height of the desired viewport. *fullscreen*
    is a boolean indicating whether a fullscreen context shall be
    created.

    .. note:: Using *fullscreen* will make PyEngine ignore the
        geometry option. It will span over all available output
        devices instead. This behaviour will be extended in the
        future, allowing the user to pick which monitors to span over.

    *display_mode* must be either :data:`None` or a
    :class:`~Engine.CEngine.Window.DisplayMode` instance which
    reflects the desired display mode.

    .. warning:: The display mode *must* be included in the return
        value of ``display.DisplayModes``, otherwise its use is
        invalid and undefined behaviour ensues!

    *event_loop* must be either :data:`None` or a valid EventLoop,
    which usually is an instance of a subclass of
    :class:`~Engine.CEngine.Window.EventLoop`.
    """

    def __init__(self, display,
                 geometry=(800, 600),
                 fullscreen=False,
                 display_mode=None,
                 event_loop=None,
                 **kwargs):
        super(Application, self).__init__(**kwargs)
        self.fullscreen = fullscreen
        self._primary_widget = None
        self._screens = []
        self.SyncedFrameLength = 0.01
        self.SyncedSpeedFactor = 1.
        self._aggregated_time = 0.

        if display_mode is None:
            modes = display.DisplayModes
            modes.sort(reverse=True)
            display_mode = modes[0]
        log.log(Severity.Information,
                "Creating context with mode: {0}".format(display_mode))
        self._window = display.createWindow(display_mode,
                                            geometry[0], geometry[1],
                                            fullscreen)

        if fullscreen:
            self._window.setFullscreen(display.Screens[0].index,
                                       display.Screens[0].index,
                                       display.Screens[0].index,
                                       display.Screens[0].index)
        else:
            self._window.setWindowed(display.Screens[0].index, *geometry)

        self._window.switchTo()
        self._window.setTitle("PythonicEngine application")
        self._window.initializeGLEW()
        glClearColor(0., 0., 0., 1.)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        self._window.flip()
        self._window._scene_widgets = []

        self.main_window, self.main_screen = self._new_screen(self._window, 0, 0, *geometry)
        self.AbsoluteRect = Rect(0, 0, *geometry)

        self.realign()
        if event_loop is None:
            event_loop = CWindow.EventLoop(display, self)
        self._event_loop = event_loop

    def _get_widget_screen(self, widget):
        p = widget
        while p is not None and p is not self:
            if isinstance(p, ScreenWidget):
                return p
            p = p._parent
        else:
            return None

    def _new_screen(self, window, x, y, width, height, primary=True):
        widget = ScreenWidget(self._desktoplayer, window)
        widget.AbsoluteRect = Rect(x, y, x+width, y+height)
        t = (window, widget)
        self._screens.append(t)
        if primary:
            assert self._primary_widget is None
            self._primary_widget = widget
        return t

    def add_scene_widget(self, widget):
        if not isinstance(widget, SceneWidget):
            raise ValueError("Scene widgets must derive from SceneWidget")
        screen = self._get_widget_screen(widget)
        assert screen is not None
        self.realign()
        if screen.AbsoluteRect & widget.AbsoluteRect != widget.AbsoluteRect:
            raise ValueError("Scene widget leaves screen boundaries")
        window = screen.Window
        assert window is not None
        assert not (widget in window._scene_widgets)
        window._scene_widgets.append(widget)

    def hit_test(self, p):
        return self._hit_test(p) or self

    def run(self):
        self._event_loop.run()

    def frameSynced(self):
        pass

    def frameUnsynced(self, deltaT):
        pass

    def handleKeyDown(self, key, modifiers):
        self.dispatch_key_down(key, modifiers)

    def handleKeyUp(self, key, modifiers):
        self.dispatch_key_up(key, modifiers)

    def handleMouseDown(self, x, y, button, modifiers):
        self.dispatch_mouse_down(x, y, button, modifiers)

    def handleMouseClick(self, x, y, button, modifiers, nth):
        self.dispatch_mouse_click(x, y, button, modifiers, nth)

    def handleMouseMove(self, x, y, dx, dy, buttons, modifiers):
        self.dispatch_mouse_move(x, y, dx, dy, buttons, modifiers)

    def handleMouseUp(self, x, y, button, modifiers):
        self.dispatch_mouse_up(x, y, button, modifiers)

    def handleMouseScroll(self, x, y, scrollX, scrollY):
        self.dispatch_scroll(x, y, scrollX, scrollY)

    def handleResize(self, width, height):
        self.AbsoluteRect = Rect(0, 0, width, height)
        glViewport(0, 0, width, height)
        # FIXME: this can be removed when rendering cairo is supported
        for widget in self.tree_depth_first():
            widget.realign()

    def handleTextInput(self, text):
        self.dispatch_text_input(text)

    def handleHide(self):
        pass

    def handleShow(self):
        pass

    def handleWMQuit(self):
        self._event_loop.terminate()

    def get_current_fps(self):
        return self._event_loop.CurrentFPS
