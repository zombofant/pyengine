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
from UI import Rect, RootWidget, ScreenWidget, SceneWidget
from OpenGL.GL import *
from OpenGL.GL.framebufferobjects import *
from GL import Framebuffer, Texture2D, Renderbuffer, makePOT

__docformat__ = "restructuredtext"

"""
Application and Window base classes.
This primarily provides for handling of multiple-head setups
"""

class Application(RootWidget):
    def __init__(self, geometry=(800, 600), fullscreen=False, useFramebuffer=True, **kwargs):
        super(Application, self).__init__(**kwargs)
        self.fullscreen = fullscreen
        self.windows = []
        self._childClasses = ScreenWidget
        self._primaryWidget = None
        self.SyncedFrameLength = 0.01
        self.SyncedSpeedFactor = 1.
        self._aggregatedTime = 0.

        if fullscreen:
            self._constructFullscreen()
        else:
            self._constructWindowed(geometry)

        if useFramebuffer:
            self._buildFramebuffer()
            self.render = self._renderWithFBO
            self.renderWindow = self._renderWindowWithFBO
        self.realign()

    def _constructWindowed(self, geometry):
        window, widget = self._newScreen((0, 0), geometry)
        self.AbsoluteRect.Width = window.width
        self.AbsoluteRect.Height = window.height

    def _constructFullscreen(self):
        """
        Detect screens and create screen widgets for each.
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
        totalWidth = 0
        totalHeight = 0
        
        for i, screen, x, y, w, h, primary in screens:
            self._newScreen((x, y), (w, h), True, primary, screen)
            totalWidth = max(totalWidth, x + w)
            totalHeight = max(totalHeight, y + h)

        self.AbsoluteRect.Width = totalWidth
        self.AbsoluteRect.Height = totalHeight

    def _newScreen(self, ui_logical, geometry, fullscreen=False, primary=True, screen=None):        
        window = self.makeWin(ui_logical, None if fullscreen else geometry, screen)
        widget = ScreenWidget(self, window)
        widget.AbsoluteRect = Rect(ui_logical[0], ui_logical[1], ui_logical[0]+geometry[0], ui_logical[1]+geometry[1])
        t = (window, widget)
        self.windows.append(t)
        if primary:
            assert self._primaryWidget is None
            self._primaryWidget = widget
        return t

    def _buildFramebuffer(self):
        w = makePOT(self.AbsoluteRect.Width)
        h = makePOT(self.AbsoluteRect.Height)
        fbo = Framebuffer(w, h)
        uiTex = Texture2D(w, h, GL_RGBA8)
        fbo[GL_COLOR_ATTACHMENT0] = uiTex
        try:
            fbo.validate()
        except Exception:
            # we may have a problem with non-quadratic textures
            w = max(w, h)
            h = max(w, h)
            fbo = Framebuffer(w, h)
            uiTex = Texture2D(w, h, GL_RGBA8)
            fbo[GL_COLOR_ATTACHMENT0] = uiTex
            fbo.validate()
        self._fbo = fbo
        self._uiTexture = uiTex
        domain = pyglet.graphics.vertexdomain.create_domain(b"v2f/static", b"t2f/static")
        vertices = [
            -1.0, 1.0,
            1.0, 1.0,
            -1.0, -1.0,
            1.0, -1.0
        ]
        vertexLists = []
        for window, widget in self.windows:
            vl = domain.create(4)
            xmin, xmax = widget.AbsoluteRect.Left / w, widget.AbsoluteRect.Right / h
            ymin, ymax = widget.AbsoluteRect.Top / h, widget.AbsoluteRect.Bottom / h
            texCoords = [
                xmin, ymax,
                xmax, ymax,
                xmin, ymin,
                xmax, ymin
            ]
            vl.vertices = vertices
            vl.tex_coords = texCoords
            window.FBOVertexList = vl

    def _getWidgetScreen(self, widget):
        p = widget
        while p is not None and p is not self:
            if isinstance(p, ScreenWidget):
                return p
            p = p._parent
        else:
            return None

    def addSceneWidget(self, widget):
        if not isinstance(widget, SceneWidget):
            raise ValueError("Scene widgets must derive from SceneWidget")
        screen = self._getWidgetScreen(widget)
        assert screen is not None
        if screen.AbsoluteRect & widget.AbsoluteRect != widget.AbsoluteRect:
            raise ValueError("Scene widget leaves screen boundaries")
        window = screen.Window
        assert window is not None
        assert not (widget in window._sceneWidgets)
        window._sceneWidgets.append(widget)

    def hitTest(self, p):
        return self._hitTest(p) or self

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
        pyglet.clock.schedule(self.update)
        pyglet.app.run()

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
        self.render()

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
        """
        Renders the UI. Note that after initalization, this function
        will have been replaced with *_renderWithFBO* if useFramebuffer
        has been passed to the constructor.
        """
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        # FIXME: Rendering without FBO support
        raise NotImplementedError("Cannot render without FBO support currently.")

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
        glClearColor(0.0, 0.0, 1.0, 0.1)
        glClear(GL_COLOR_BUFFER_BIT)
        glDisable(GL_DEPTH_TEST)
        super(Application, self).render()
        glEnable(GL_DEPTH_TEST)
        # glDisable(GL_SCISSOR_TEST)
        Framebuffer.unbind()

    def renderWindow(self, window):
        raise NotImplementedError("Cannot render without FBO support currently.")

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
        glClear(GL_DEPTH_BUFFER_BIT)
        glEnable(GL_BLEND)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
        self._uiTexture.bind()
        glEnable(GL_TEXTURE_2D)
        window.FBOVertexList.draw(GL_TRIANGLE_STRIP)
        glDisable(GL_TEXTURE_2D)
        Texture2D.unbind()
        glDisable(GL_BLEND)

    def _fold_coords(self, win, x, y):
        """
        Fold window coordinates *x*, *y* from window *win* to logical
        UI coordinates
        """
        lx, ly = win.UILogicalCoords
        return lx+x, ly+y

    def _on_close(self, win):
        pyglet.app.exit()

    def _on_draw(self, win):
        self.renderWindow(win)

    def _on_key_press(self, win, symbol, modifiers):
        self.dispatchKeyDown(symbol, modifiers)

    def _on_key_release(self, win, symbol, modifiers):
        self.dispatchKeyUp(symbol, modifiers)

    def _on_mouse_drag(self, win, x, y, dx, dy, buttons, modifiers):
        lx, ly = self._fold_coords(win, x, y)
        self.dispatchMouseMove(lx, ly, dx, dy, buttons, modifiers)

    def _on_mouse_motion(self, win, x, y, dx, dy):
        lx, ly = self._fold_coords(win, x, y)

        # the last two arguments are modifier bitmaps, as we don't get
        # any just pass none of them
        self.dispatchMouseMove(lx, ly, dx, dy, 0, 0)


    def _on_mouse_press(self, win, x, y, button, modifiers):
        lx, ly = self._fold_coords(win, x, y)
        self.dispatchMouseDown(lx, ly, button, modifiers)

    def _on_mouse_release(self, win, x, y, button, modifiers):
        lx, ly = self._fold_coords(win, x, y)
        self.dispatchMouseUp(lx, ly, button, modifiers)

    def _on_mouse_scroll(self, win, x, y, scroll_x, scroll_y):
        lx, ly = self._fold_coords(win, x, y)
        self.dispatchScroll(lx, ly, scroll_x, scroll_y)

    def _on_resize(self, win, width, height):
        pass

    def _on_text(self, win, text):
        self.dispatchTextInput(text)

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
        self._sceneWidgets = []

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
        self._application._on_key_release(self, symbol, modifiers)

    def on_mouse_drag(self, x, y, dx, dy, buttons, modifiers):
        self._application._on_mouse_drag(self, x, y, dx, dy, buttons, modifiers)

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
