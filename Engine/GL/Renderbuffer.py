# File name: Renderbuffer.py
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
from Base import *

class RenderbufferBase(GLObject):
    def __init__(self, width=None, height=None, **kwargs):
        assert width is not None
        assert height is not None
        super(RenderbufferBase, self).__init__(**kwargs)
        self._dimensions = (width, height)
        
    def _getValid(self):
        return False
        
    @property
    def Dimensions(self):
        return self._dimensions
        
    @property
    def Valid(self):
        return self._getValid()
        
    def attach(self, target):
        pass
        
class Renderbuffer(RenderbufferBase):
    def __init__(self, width=None, height=None, format=None, **kwargs):
        super(Renderbuffer, self).__init__(width=width, height=height, **kwargs)
        self.format = format
        self.id = glGenRenderbuffers(1)
        glBindRenderbuffer(GL_RENDERBUFFER, self.id)
        glRenderbufferStorage(GL_RENDERBUFFER, format, width, height)
        glBindRenderbuffer(GL_RENDERBUFFER, 0)
        
    def __del__(self):
        glDeleteRenderbuffers(self.id)
    
    def attach(self, target):
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, target, GL_RENDERBUFFER, self.id)
        
    def _getValid(self):
        return True

