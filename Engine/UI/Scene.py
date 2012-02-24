# File name: Scene.py
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

__all__ = ["SceneWidget"]

from Widget import Widget
from OpenGL.GL import *
from OpenGL.GLU import *

class SceneWidget(Widget):
    def __init__(self, parent, **kwargs):
        # FIXME: pass scene graph and camera to use ;)
        super(SceneWidget, self).__init__(parent, **kwargs)
        self.FOV = 60.0
        self.ZNear = 1.0
        self.ZFar = 100.0

    def _setupProjection(self):
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(self.FOV, self.Rect.Width / self.Rect.Height, self.ZNear, self.ZFar)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()

    def _resetProjection(self):
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        glMatrixMode(GL_MODELVIEW)

    def renderScene(self):
        pass
