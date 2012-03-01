# File name: MaterialLoader.py
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

from Base import ResourceLoader
from Manager import ResourceManager

try:
    from Engine.GL.RenderModel import RenderModel
    from Engine.GL.Material import Material
except ImportError:
    pass

class MaterialLoader(ResourceLoader):
    """
    The MaterialLoader loads model material data.
    It loads exactly one material from the given fileLike and returns
    a Material instance constructed from the data.
    """

    def __init__(self, **kwargs):
        try:
            super(MaterialLoader, self).__init__(
                [Material],
                ['mtl'],
                relativePathPrefix='/data/materials',
                **kwargs)
        except NameError:
            self._loaderNotAvailable()

    def load(self, fileLike, targetClass=None):
        """
        The mtl loader.
        Load the material with name materialName from 
        """
        # FIXME: only parses face texture at the moment
        inMaterial = False
        textures = []
        for line in fileLike:
            if len(line) < 1 :
                if inMaterial: break
                continue
            if line[0] == '#' : continue
            parts = line.strip().split(' ')
            if parts[0] == 'newmtl':
                inMaterial = True
            if parts[0] == 'map_Kd':
                textures.append(parts[1])
        return Material(textures=textures)

ResourceManager().registerResourceLoader(MaterialLoader)
