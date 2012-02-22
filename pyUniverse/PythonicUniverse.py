# File name: PythonicUniverse.py
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
from Engine.Application import Window
from OpenGL.GL import *
import math

class PythonicUniverse(Window):
    def __init__(self, **kwargs):
        super(PythonicUniverse, self).__init__(**kwargs)
        self._initGL()
        self._initScrap()

    def _initGL(self):
        # always have your clear color set to something other than
        # black. If you ever debugged drawing problems with supposedly
        # missing vertices which were just black, you know what I'm
        # talking about...
        glClearColor(0.1, 0.1, 0.1, 1.0)
        glViewport(0, 0, self._width, self._height)
        self._setupOrtho(self._width, self._height)

    def _initScrap(self):
        self._quadPos = (0, 0)
        self._quadRot = 0.

    def _setupOrtho(self, width, height):
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        glOrtho(0, width, height, 0, -10, 10)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()

    def updateUnsynced(self, timeDelta):
        self._quadRot += timeDelta * math.pi
        if self._quadRot > 2.*math.pi:
            self._quadRot -= int(self._quadRot/math.pi)*math.pi

    def render(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

        xoffs = 5. * math.cos(self._quadRot)
        yoffs = 5. * math.sin(self._quadRot)

        x0, y0 = self._quadPos

        v1, v2, v3, v4 = (
            (x0 - xoffs, y0 - yoffs),
            (x0 - xoffs, y0 + yoffs),
            (x0 + xoffs, y0 + yoffs),
            (x0 + xoffs, y0 - yoffs),
        )
        glBegin(GL_QUADS)
        glVertex2f(*v1)
        glVertex2f(*v2)
        glVertex2f(*v3)
        glVertex2f(*v4)
        glEnd()

        self.window.Display()

    def onResized(self, event):
        super(PythonicUniverse, self).onResized(event)
        glViewport(0, 0, self._width, self._height)
        self._setupOrtho(self._width, self._height)

    def onMouseMoved(self, event):
        self._quadPos = (event.MouseMove.X, event.MouseMove.Y)

