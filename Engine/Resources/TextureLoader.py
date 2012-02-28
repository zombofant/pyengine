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
except ImportError:
    pass

class TextureLoader(ResourceLoader):
    """
    Implement a loader for text resources.
    """

    def __init__(self, **kwargs):
        super(TextureLoader, self).__init__(**kwargs)
        self._supportedTargetClasses = [Texture2D]
        self._defaultTargetClass = Texture2D
        self._resourceTypes = ['png', 'jpg']

    def load(self, fileLike, targetClass=Texture2D):
        img = image.load(fileLike.name, fileLike)
        data = img.get_image_data()
        tex = Texture2D(width=data.width,
            height=data.height, format=4,
            data=data.get_data(data.format, data.pitch))
        return tex

# register an instance of TextLoader with the resource manager
ResourceManager().registerResourceLoader(TextureLoader())

