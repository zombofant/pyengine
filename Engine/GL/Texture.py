# File name: Texture.py
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

from Base import *
from Renderbuffer import RenderbufferBase
import numpy as np

try:
    import CUni.Resources as CResources
except ImportError:
    class CResources(object):
        Image = type(None)

class TextureBase(BindableObject):
    _bindCall = glBindTexture
    
    def __init__(self, format=None, **kwargs):
        assert format is not None
        super(TextureBase, self).__init__(**kwargs)
        self.format = format
        self.id = glGenTextures(1)

    def _getDataFormatType(self, dataTuple):
        return dataTuple if dataTuple is not None else (GL_LUMINANCE, GL_UNSIGNED_BYTE, None)
        
    def __del__(self):
        glDeleteTextures(np.array((self.id,), dtype=np.uint32))
        
    def __setitem__(self, key, value):
        if type(value) == int:
            glTexParameteri(self._bindClass, key, value)
        else:
            glTexParameterf(self._bindClass, key, value)
        
class Texture1D(TextureBase):
    _bindClass = GL_TEXTURE_1D
    
    def __init__(self, width=None, format=None, data=None, **kwargs):
        super(Texture1D, self).__init__(format=format, **kwargs)
        dataFormat, dataType, data = self._getDataFormatType(data)
        self._dimensions = (width, )
        self.bind()
        glTexImage1D(GL_TEXTURE_1D, 0, format, self._dimensions[0], 0, dataFormat, dataType, data)
        self[GL_TEXTURE_MAG_FILTER] = GL_NEAREST
        self[GL_TEXTURE_MIN_FILTER] = GL_NEAREST
        self[GL_TEXTURE_WRAP_S] = GL_CLAMP_TO_EDGE
        Texture1D.unbind()
    
    @property
    def Dimensions(self):
        return self._dimensions

class Texture2D(TextureBase, RenderbufferBase):
    _bindClass = GL_TEXTURE_2D
    
    def __init__(self, width=None, height=None, format=None, data=None, **kwargs):
        super(Texture2D, self).__init__(width=width, height=height, format=format, **kwargs)
        self.bind()
        if isinstance(data, CResources.Image):
            data.texImage2D(GL_TEXTURE_2D, 0, format)
        else:
            dataFormat, dataType, data = self._getDataFormatType(data)
            glTexImage2D(GL_TEXTURE_2D, 0, format, self._dimensions[0], self._dimensions[1], 0, dataFormat, dataType, data)
        self[GL_TEXTURE_MAG_FILTER] = GL_NEAREST
        self[GL_TEXTURE_MIN_FILTER] = GL_NEAREST
        self[GL_TEXTURE_WRAP_S] = GL_CLAMP_TO_EDGE
        self[GL_TEXTURE_WRAP_T] = GL_CLAMP_TO_EDGE
        Texture2D.unbind()
        
    def attach(self, target):
        glFramebufferTexture2D(GL_FRAMEBUFFER, target, GL_TEXTURE_2D, self.id, 0)
        
    def _getValid(self):
        return True

