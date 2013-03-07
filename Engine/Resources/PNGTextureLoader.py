# File name: PNGTextureLoader.py
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

from Base import ResourceLoader
from Manager import ResourceManager

__global_error = None
try:
    from Engine.GL.Texture import Texture2D
    from OpenGL.GL import GL_RGBA, GL_UNSIGNED_BYTE

    import Engine.CEngine as CEngine
    import Engine.CEngine.Resources as CResources
except ImportError as err:
    __global_error = err

import cairo
from CairoWrapper import CairoSurface

class PNGTextureLoader(ResourceLoader):
    """
    Implement a loader for png images as OpenGL textures.
    """

    def __init__(self, **kwargs):
        if globals()["__global_error"]:
            self._loader_not_available(globals()["__global_error"])
        target_classes = []
        try:
            target_classes.append(Texture2D)
        except NameError as err:
            self._loader_not_available(unicode(err))
        target_classes.append(CairoSurface)
        try:
            CEngine
        except NameError as err:
            self._loader_not_available(unicode(err))
        
        super(PNGTextureLoader, self).__init__(
            target_classes,
            ['png'],
            relative_path_prefix="/data/textures",
            **kwargs)
        
 
    def load(self, filelike, targetclass=None):
        """
        Loads the given filelike as PNG image. This is done using
        :cpp:func:`PNGImage` from CEngine
        """
        image = CResources.PNGImage(CEngine.Stream(filelike))
        if image is None or not image.IsValid:
            raise ValueError("Not valid PNG data.")
        
        if issubclass(targetclass, CairoSurface):
            return CairoSurface(image.cairoSurface())
        elif issubclass(targetclass, Texture2D):            
            texture = Texture2D(
                width=image.Width,
                height=image.Height,
                format=GL_RGBA,
                data=image)
            del image
            return texture
        else:
            assert targetclass in self._supported_target_classes

# register an instance of TextLoader with the resource manager
ResourceManager().register_resource_loader(PNGTextureLoader)

