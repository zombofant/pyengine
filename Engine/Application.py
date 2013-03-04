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
from GL import Framebuffer, Texture2D, Renderbuffer, makePOT

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

    *displayMode* must be either :data:`None` or a
    :class:`~Engine.CEngine.Window.DisplayMode` instance which
    reflects the desired display mode.

    .. warning:: The display mode *must* be included in the return
        value of ``display.DisplayModes``, otherwise its use is
        invalid and undefined behaviour ensues!

    *eventLoop* must be either :data:`None` or a valid EventLoop,
    which usually is an instance of a subclass of
    :class:`~Engine.CEngine.Window.EventLoop`.
    """

    def __init__(self, display,
                 geometry=(800, 600),
                 fullscreen=False,
                 displayMode=None,
                 eventLoop=None,
                 **kwargs):
        super(Application, self).__init__(**kwargs)
        self.fullscreen = fullscreen
        self._primaryWidget = None
        self._screens = []
        self.SyncedFrameLength = 0.01
        self.SyncedSpeedFactor = 1.
        self._aggregatedTime = 0.

        if displayMode is None:
            modes = display.DisplayModes
            modes.sort(reverse=True)
            displayMode = modes[0]
        log.log(Severity.Information,
                "Creating context with mode: {0}".format(displayMode))
        self._window = display.createWindow(displayMode,
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
        self._window._sceneWidgets = []

        self._newScreen(self._window, 0, 0, *geometry)
        self.AbsoluteRect = Rect(0, 0, *geometry)

        self.realign()
        if eventLoop is None:
            eventLoop = CWindow.EventLoop(display, self)
        self._eventLoop = eventLoop

    def _getWidgetScreen(self, widget):
        p = widget
        while p is not None and p is not self:
            if isinstance(p, ScreenWidget):
                return p
            p = p._parent
        else:
            return None

    def _newScreen(self, window, x, y, width, height, primary=True):
        widget = ScreenWidget(self._desktopLayer, window)
        widget.AbsoluteRect = Rect(x, y, x+width, y+height)
        t = (window, widget)
        self._screens.append(t)
        if primary:
            assert self._primaryWidget is None
            self._primaryWidget = widget
        return t

    def addSceneWidget(self, widget):
        if not isinstance(widget, SceneWidget):
            raise ValueError("Scene widgets must derive from SceneWidget")
        screen = self._getWidgetScreen(widget)
        assert screen is not None
        self.realign()
        if screen.AbsoluteRect & widget.AbsoluteRect != widget.AbsoluteRect:
            raise ValueError("Scene widget leaves screen boundaries")
        window = screen.Window
        assert window is not None
        assert not (widget in window._sceneWidgets)
        window._sceneWidgets.append(widget)

    def hitTest(self, p):
        return self._hitTest(p) or self

    def run(self):
        self._eventLoop.run()

    def frameSynced(self):
        pass

    def frameUnsynced(self, deltaT):
        pass

    def handleKeyDown(self, key, modifiers):
        self.dispatchKeyDown(key, modifiers)

    def handleKeyUp(self, key, modifiers):
        self.dispatchKeyUp(key, modifiers)

    def handleMouseDown(self, x, y, button, modifiers):
        self.dispatchMouseDown(x, y, button, modifiers)

    def handleMouseMove(self, x, y, dx, dy, buttons, modifiers):
        self.dispatchMouseMove(x, y, dx, dy, buttons, modifiers)

    def handleMouseUp(self, x, y, button, modifiers):
        self.dispatchMouseUp(x, y, button, modifiers)

    def handleMouseScroll(self, x, y, scrollX, scrollY):
        self.dispatchScroll(x, y, scrollX, scrollY)

    def handleResize(self, width, height):
        self.AbsoluteRect = Rect(0, 0, width, height)
        glViewport(0, 0, width, height)
        # FIXME: this can be removed when rendering cairo is supported
        for widget in self.treeDepthFirst():
            widget.realign()

    def handleTextInput(self, text):
        self.dispatchTextInput(text)

    def handleHide(self):
        pass

    def handleShow(self):
        pass

    def handleWMQuit(self):
        self._eventLoop.terminate()

    def getCurrentFPS(self):
        return self._eventLoop.CurrentFPS
