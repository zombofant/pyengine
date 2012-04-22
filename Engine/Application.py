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

try:
    import CUni
    import CUni.Window
    import CUni.GL as CGL
except ImportError:
    # XXX: make documentation work
    class CUni(object):
        class Window(object):
            class EventSink(object):
                pass

import time
from OpenGL.GL import *
from OpenGL.GL.framebufferobjects import *

from UI import Rect, RootWidget, ScreenWidget, SceneWidget
from GL import Framebuffer, Texture2D, Renderbuffer, makePOT

__docformat__ = "restructuredtext"

"""
Application and Window base classes.
This primarily provides for handling of multiple-head setups
"""

class Application(RootWidget, CUni.Window.EventSink):
    def __init__(self, display, geometry=(800, 600), fullscreen=False, **kwargs):
        super(Application, self).__init__(**kwargs)
        self.fullscreen = fullscreen
        self._primaryWidget = None
        self._screens = []
        self.SyncedFrameLength = 0.01
        self.SyncedSpeedFactor = 1.
        self._aggregatedTime = 0.

        self._geometryBuffer = CGL.GeometryBuffer(CGL.VertexFormat("v:2;t0:2;c:4"), GL_DYNAMIC_DRAW)
        self.updateGeometryBuffer()

        self._render = super(Application, self).render

        if fullscreen:
            raise NotImplementedError("FIXME: C++ interface does not support fullscreen yet.")
        else:
            modes = display.DisplayModes
            modes.sort(reverse=True)
            mode = modes[0]
            print("Creating context with mode: {0}".format(mode))
            self._window = display.createWindow(mode, geometry[0], geometry[1], fullscreen)
            self._window.switchTo()
            self._window.initializeGLEW()
            #self._window.switchTo()
            self._newScreen(self._window, 0, 0, *geometry)

        self.realign()
        self._eventLoop = CUni.Window.EventLoop(display, self)

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

    def update(self, timeDelta):
        """
        This method gets called by pyglet on every frame iteration. It
        dispatches the passed time to *updateSynced* and
        *updateUnsynced*.
        """
        syncedTime = self._aggregatedTime + timeDelta * self.SyncedSpeedFactor
        frameLength = self.SyncedFrameLength
        while syncedTime >= frameLength:
            syncedTime -= frameLength
            self.updateSynced()
        self._aggregatedTime = syncedTime
        self.updateUnsynced(timeDelta)
        super(Application, self).update(timeDelta)
        if self.updateRender is not None:
            self.updateRender()

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

    def render(self):
        self._render()

    def _renderWithoutFBO(self):
        """
        Renders the UI. Note that after initalization, this function
        will have been replaced with *_renderWithFBO* if useFramebuffer
        has been passed to the constructor.
        """
        super(Application, self).render()

    def _renderWithFBO(self):
        self._fbo.bind()
        x, y, w, h = self.AbsoluteRect.XYWH
        glViewport(x, y, w, h)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        glOrtho(0, w, h, 0, -10.0, 10.0)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        # FIXME: Redraw only parts of the UI which need to
        # glEnable(GL_SCISSOR_TEST)
        # glScissor(
        glClearColor(0.0, 0.0, 1.0, 0.0)
        glClear(GL_COLOR_BUFFER_BIT)
        glDisable(GL_DEPTH_TEST)
        glEnable(GL_BLEND)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
        super(Application, self).render()
        glDisable(GL_BLEND)
        glEnable(GL_DEPTH_TEST)
        # glDisable(GL_SCISSOR_TEST)
        Framebuffer.unbind()

    def renderWindow(self, window):
        glClearColor(0.0, 0.0, 0.0, 1.0)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

        wx, wy, ww, wh = window.widget.AbsoluteRect.XYWH

        for sceneWidget in window._sceneWidgets:
            x, y, w, h = sceneWidget.AbsoluteRect.XYWH
            xlog, ylog = window.UILogicalCoords
            x -= xlog
            y -= ylog
            glViewport(x, y, w, h)
            sceneWidget.renderScene()

        glViewport(0, 0, ww, wh)
        self._setUIOffset(wx, wy)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        glOrtho(0, ww, wh, 0, -10., 10.)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        glClear(GL_DEPTH_BUFFER_BIT)
        glEnable(GL_BLEND)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
        self.render()
        glDisable(GL_BLEND)

    def _renderWindowWithFBO(self, window):
        glClearColor(0.0, 0.0, 0.0, 1.0)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

        for sceneWidget in window._sceneWidgets:
            x, y, w, h = sceneWidget.AbsoluteRect.XYWH
            xlog, ylog = window.UILogicalCoords
            x -= xlog
            y -= ylog
            glViewport(x, y, w, h)
            sceneWidget.renderScene()

        glViewport(0, 0, window.width, window.height)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        glClear(GL_DEPTH_BUFFER_BIT)
        glEnable(GL_BLEND)
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA)
        self._uiTexture.bind()
        glEnable(GL_TEXTURE_2D)
        window.FBOVertexList.draw(GL_TRIANGLE_STRIP)
        glDisable(GL_TEXTURE_2D)
        Texture2D.unbind()
        glDisable(GL_BLEND)

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
        self.dispatchMouseScroll(x, y, scrollX, scrollY)

    def handleResize(self, width, height):
        pass

    def handleTextInput(self, text):
        self.dispatchTextInput(text)

    def handleWMQuit(self):
        self._eventLoop.terminate()
