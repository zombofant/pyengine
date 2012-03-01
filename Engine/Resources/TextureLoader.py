# File name: TextureLoader.py
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
    from pyglet import image
    from Engine.GL.Texture import Texture2D
    from OpenGL.GL import GL_RGBA, GL_UNSIGNED_BYTE
except ImportError:
    pass

class TextureLoader(ResourceLoader):
    """
    Implement a loader for texture resources.
    """

    def __init__(self, **kwargs):
        try:
            super(TextureLoader, self).__init__(
                [Texture2D],
                ['png', 'jpg'],
                relativePathPrefix="/data/textures"
                **kwargs)
        except NameError:
            self._loaderNotAvailable()
        
 
    def load(self, fileLike, targetClass=None):
        """
        Load the texture.
        We simply use the pyglet image loading functionality at the moment.
        """
        data = image.load(fileLike.name, fileLike).get_image_data()
        texture = Texture2D(
            width=data.width,
            height=data.height,
            format=GL_RGBA,
            data=(GL_RGBA, GL_UNSIGNED_BYTE, data.get_data('RGBA', data.pitch)))
        del data
        return texture

# register an instance of TextLoader with the resource manager
ResourceManager().registerResourceLoader(TextureLoader)

