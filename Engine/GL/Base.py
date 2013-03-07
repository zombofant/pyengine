# File name: Base.py
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

try:
    import Engine.CEngine.GL as CGL
    Struct = CGL.Struct
    Class = CGL.Class
except ImportError:
    class CGL(object):        
        Struct = object
        Class = object
try:
    from OpenGL.GL import *
    from OpenGL.GLU import *
    from OpenGL.GL.framebufferobjects import *
    from OpenGL.GL.shaders import *
except ImportError:
    pass

class Struct(CGL.Struct):
    def __init__(self, **kwargs):
        super(Struct, self).__init__(**kwargs)
    
    @classmethod
    def unbind(cls):
        cls._bind_call(cls._bind_class, 0)

class Object(Struct, CGL.Class):
    def __init__(self, **kwargs):
        super(Object, self).__init__(**kwargs)
        self.id = 0

class BindableObject(Object):
    def bind(self):
        self._bind_call(self._bind_class, self.id)
    

def check_error():
    error = glGetError()
    if error != GL_NO_ERROR:
        raise Exception(gluErrorString(error))
