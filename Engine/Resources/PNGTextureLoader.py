# File name: PNGTextureLoader.py
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

__globalError = None
try:
    from Engine.GL.Texture import Texture2D
    from OpenGL.GL import GL_RGBA, GL_UNSIGNED_BYTE

    import CUni
    import CUni.Resources as CResources
except ImportError as err:
    __globalError = err

import cairo
from CairoWrapper import CairoSurface

class PNGTextureLoader(ResourceLoader):
    """
    Implement a loader for png images as OpenGL textures.
    """

    def __init__(self, **kwargs):
        if globals()["__globalError"]:
            self._loaderNotAvailable(globals()["__globalError"])
        targetClasses = []
        targetClasses.append(CairoSurface)
        try:
            targetClasses.append(Texture2D)
        except NameError as err:
            self._loaderNotAvailable(unicode(err))
        try:
            CUni
        except NameError as err:
            self._loaderNotAvailable(unicode(err))
        
        super(PNGTextureLoader, self).__init__(
            targetClasses,
            ['png'],
            relativePathPrefix="/data/textures",
            **kwargs)
        
 
    def load(self, fileLike, targetClass=None):
        """
        Loads the given fileLike as PNG image. This is done using
        :cpp:func:`PNGImage` from CUni.
        """
        image = CResources.PNGImage(CUni.Stream(fileLike))
        if image is None or not image.IsValid:
            raise ValueError("Not valid PNG data.")
        
        if issubclass(targetClass, CairoSurface):
            return CairoSurface(image.cairoSurface())
        elif issubclass(targetClass, Texture2D):            
            texture = Texture2D(
                width=image.Width,
                height=image.Height,
                format=GL_RGBA,
                data=image)
            del image
            return texture
        else:
            assert targetClass in self._supportedTargetClasses

# register an instance of TextLoader with the resource manager
ResourceManager().registerResourceLoader(PNGTextureLoader)

