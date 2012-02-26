# File name: Framebuffer.py
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
from Renderbuffer import RenderbufferBase
import numpy as np

class Framebuffer(Object):
    def __init__(self, width=None, height=None, **kwargs):
        assert width is not None
        assert height is not None
        super(Framebuffer, self).__init__(**kwargs)
        self.id = glGenFramebuffers(1)
        self._dimensions = (width, height)
        self._changed = False
        self._attachments = {
            GL_DEPTH_ATTACHMENT: None,
            GL_STENCIL_ATTACHMENT: None,
        }
        for i in range(GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT0+8):
            self._attachments[i] = None
            
    def __del__(self):
        glDeleteFramebuffers(np.array((self.id,)))
        super(Framebuffer, self).__del__()
    
    def __setitem__(self, key, value):
        if not isinstance(value, RenderbufferBase):
            raise TypeError("Needs {0}, got {1}".format(RenderbufferBase, type(value)))
        if not key in self._attachments:
            raise KeyError("Invalid attachment id: 0x{0:x}".format(key))
        if not value.Valid:
            raise ValueError("Attachment is not valid (yet?)")
        if value is not self._attachments[key]:
            if self._dimensions != value.Dimensions:
                raise ValueError("Incompatible dimensions. Have: {0}, got: {1}".format(self._dimensions, value.Dimensions))
            self._attachments[key] = value
            self._changed = True
    
    def __getitem__(self, key):
        return self._attachments[key]
        
    @property
    def Dimensions(self):
        return self._dimensions
    
    @Dimensions.setter
    def Dimensions(self, value):
        if value == self._dimensions:
            return
        newValue = tuple(value)
        if len(newValue) != 2 or type(newValue[0]) != int or type(newValue[1]) != int:
            raise TypeError("Need two value tuple, got {0} {1}".format(type(value), value))
        for obj in self._attachments.itervalues():
            if obj is not None:
                raise Exception("Cannot change framebuffer dimensions while attachments are present")
        self._dimensions = newValue
        
    def reattach(self):
        for slot, obj in self._attachments.iteritems():
            if obj is not None:
                obj.attach(slot)
        errno = glCheckFramebufferStatus(GL_FRAMEBUFFER)
        error = {
            GL_FRAMEBUFFER_COMPLETE: None,
            GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: "Incomplete attachment",
            GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: "Missing attachment",
            GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS: "Dimension mismatch",
            GL_FRAMEBUFFER_INCOMPLETE_FORMATS: "Format mismatch",
            GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: "Draw buffer error",
            GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: "Read buffer error",
            GL_FRAMEBUFFER_UNSUPPORTED: "Unsupported framebuffer setup"
        }[errno]
        if error is not None:
            raise Exception(error)
        self._changed = False
        
    def validate(self):
        if not self._changed:
            return
        glBindFramebuffer(GL_FRAMEBUFFER, self.id)
        self.reattach()
        Framebuffer.unbind()
    
    def bind(self):
        self.validate()
        glBindFramebuffer(GL_FRAMEBUFFER, self.id)
        
    def setupViewport(self):
        glViewport(0, 0, self._dimensions[0], self._dimensions[1])
        
    @staticmethod
    def unbind():
        glBindFramebuffer(GL_FRAMEBUFFER, 0)
