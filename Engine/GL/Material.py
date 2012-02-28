# File name: Material.py
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
import sys

from StateManagement import StateObjectGroup, ActiveTexture
from Texture import Texture2D
from Engine.Resources.Manager import ResourceManager
from OpenGL.GL import GL_TEXTURE0, glEnable, glDisable, glBindTexture
import pyglet

class Material(object):
    """
    The Material class stores material data for 3D models
    """

    def __init__(self, **kwargs):
        super(Material, self).__init__()
        self._dataTypes = ['textures']
        self.setData(**kwargs)

    def setData(self, **args):
        for dtype in dict(**args):
            self.__setattr__(dtype, args[dtype])

    @property
    def stateGroup(self):
        if len(self._textures) == 0: return None
        texpath = '/data/textures/%s' % self._textures[0]
        texture = ResourceManager().require(texpath)
        return StateObjectGroup(ActiveTexture(texture, GL_TEXTURE0))

    @property
    def textures(self):
        return self._textures

    @textures.setter
    def textures(self, value):
        self._textures = list(value)

class TestGroup(pyglet.graphics.Group):
    def __init__(self, texture):
        super(TestGroup, self).__init__()
        assert isinstance(texture, Texture2D)
        self._texture = texture

    def set_state(self):
        glEnable(self._texture.target)
        glBindTexture(self._texture.target, self._texture.id)

    def unset_state(self):
        glDisable(self._texture.target)

