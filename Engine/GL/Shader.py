# File name: Shader.py
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

import Engine.Utils as Utils

from Base import *

import ctypes as c

class Shader(BindableObject):
    def __init__(self, **kwargs):
        super(Shader, self).__init__(**kwargs)
        self.uniformmap = {}

    def _compile_shader(self, kind, source):
        shader = glCreateShader(kind)
        glShaderSource(shader, source)
        glCompileShader(shader)
        
        length = glGetShaderiv(shader, GL_INFO_LOG_LENGTH)
        if length > 1:
            log = glGetShaderInfoLog(shader)
            glDeleteShader(shader)
            raise ValueError("Failed to compile {0}.\nCompiler log: \n{2}\nSource: \n{1}".format(kind, Utils.line_numbering(source), log))
        return shader

    def _load_uniform(self, name):
        location = glGetUniformLocation(self.id, str(name))
        self.uniformmap[name] = location
        return location

    def load_shader(self, vs, fs):
        if self.id != 0:
            self.unload_shader()
        vsobj = None
        fsobj = None
        error = None
        program = glCreateProgram()
        try:
            vsobj = self._compile_shader(GL_VERTEX_SHADER, vs)
            fsobj = self._compile_shader(GL_FRAGMENT_SHADER, fs)
            
            glAttachShader(program, vsobj)
            glAttachShader(program, fsobj)
            
            glLinkProgram(program)
            
            glDeleteShader(vsobj)
            glDeleteShader(fsobj)

            if glGetProgramiv(program, GL_LINK_STATUS) != GL_TRUE:
                log = glGetProgramInfoLog(program)
                raise ValueError("Shader program did not link: {0}".format(log))

            glValidateProgram(program)
            if glGetProgramiv(program, GL_VALIDATE_STATUS) != GL_TRUE:
                raise ValueError("Shader program did not validate.")
            
            self.id = program
        except:
            if vsobj is not None:
                glDeleteShader(vsobj)
            if fsobj is not None:
                glDeleteShader(fsobj)
            glDeleteProgram(program)
            self.id = 0
            raise

    def load_from_file(self, vsname, fsname, uniforms):
        f = open(fsname, 'r')
        fs = f.read()
        f.close()
        f = open(vsname, 'r')
        vs = f.read()
        f.close()
        self.load_shader(vs, fs, uniforms)

    def unload_shader(self):
        glDeleteProgram(program)
        self.id = 0

    def __contains__(self, name):
        value = self.uniformmap.get(name, None)
        if value is None:
            value = self._load_uniform(name)
        return value >= 0

    def __getitem__(self, name):
        value = self.uniformmap.get(name, None)
        if value is None:
            value = self._load_uniform(name)
        if value < 0:
            raise KeyError("Unknown uniform in program object: {0}".format(name))
        return value

    def bind(self):
        if Shader.bound != self.id:
            glUseProgram(self.id)

    @staticmethod
    def unbind():
        Shader.bound = None
        glUseProgram(0)

Shader.bound = None
