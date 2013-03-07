# File name: Material.py
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
import sys

from StateManagement import StateObjectGroup, ActiveTexture
from Texture import Texture2D
from Engine.Resources.Manager import ResourceManager
from OpenGL.GL import GL_TEXTURE0

class Material(object):
    """
    The Material class stores material data for 3D models
    """

    def __init__(self, **kwargs):
        super(Material, self).__init__()
        self._datatypes = ['textures']
        self._stategroup = None
        self._updategroup = False
        self._clear()
        self.set_data(**kwargs)

    def __del__(self):
        for texture in self._textures:
            del texture
        if self._stategroup is not None:
            del self._stategroup

    def _clear(self):
        for dtype in self._datatypes:
            self.__setattr__(dtype, None)
        self._stategroup = None

    def _update_state_group(self):
        states = []
        # add textures
        if self._textures is not None:
            for j in range(0,len(self._textures)):
                texnum = int(GL_TEXTURE0) + j
                states.append(ActiveTexture(self._textures[j], texnum))
        if self._stategroup is not None:
            del self._stategroup
        self._stategroup = StateObjectGroup(*states)
        self._updategroup = False
 
    def set_data(self, **args):
        for dtype in dict(**args):
            self.__setattr__(dtype, args[dtype])

    @property
    def state_group(self):
        """
        Return a state group for rendering repesenting the OpenGL states
        and textures the material defines.
        """
        if self._updategroup or self._stategroup is None:
            self._update_state_group()
        return self._stategroup

    @property
    def textures(self):
        """
        Returns the list of textures this material contains.
        See setter declaration below for more information.
        """
        return self._textures

    @textures.setter
    def textures(self, value):
        """
        Set the list of textures this material contains.
        This should be a list of texture filenames.
        """
        texlist = []
        if value is not None:
            for filename in list(value):
                texlist.append(ResourceManager().require(filename))
        self._textures = texlist
        if len(self._textures) < 1:
            self._textures = None
        self._updategroup = True # force updating the render-state group

