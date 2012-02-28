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
from OpenGL.GL import GL_TEXTURE0

class Material(object):
    """
    The Material class stores material data for 3D models
    """

    def __init__(self, **kwargs):
        super(Material, self).__init__()
        self._dataTypes = ['textures']
        self._stateGroup = None
        self._updateGroup = False
        self._clear()
        self.setData(**kwargs)

    def __del__(self):
        for texture in self._textures:
            del texture
        if self._stateGroup is not None:
            del self._stateGroup

    def _clear(self):
        for dtype in self._dataTypes:
            self.__setattr__(dtype, None)
        self._stateGroup = None

    def _updateStateGroup(self):
        states = []
        # add textures
        if self._textures is not None:
            for j in range(0,len(self._textures)):
                texNum = int(GL_TEXTURE0) + j
                states.append(ActiveTexture(self._textures[j], texNum))
        if self._stateGroup is not None:
            del self._stateGroup
        self._stateGroup = StateObjectGroup(*states)
        self._updateGroup = False
 
    def setData(self, **args):
        for dtype in dict(**args):
            self.__setattr__(dtype, args[dtype])

    @property
    def stateGroup(self):
        """
        Return a state group for rendering repesenting the OpenGL states
        and textures the material defines.
        """
        if self._updateGroup or self._stateGroup is None:
            self._updateStateGroup()
        return self._stateGroup

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
        texList = []
        if value is not None:
            for filename in list(value):
                texList.append(ResourceManager().require(filename))
        self._textures = texList
        if len(self._textures) < 1:
            self._textures = None
        self._updateGroup = True # force updating the render-state group

