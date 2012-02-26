# File name: Shader.py
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

class Shader(Object):
    def __init__(self, **kwargs):
        super(Shader, self).__init__(**kwargs)
        self.id = None
        
    def _compileShader(self, kind, source):
        shader = glCreateShader(kind)
        glShaderSource(shader, source)
        glCompileShader(shader)
        
        length = glGetShaderiv(shader, GL_INFO_LOG_LENGTH)
        if length > 1:
            log = glGetShaderInfoLog(shader)
            glDeleteShader(shader)
            raise Exception(log)
        return shader
            
    def _loadUniforms(self, uniforms):
        self.uniformMap = {}
        for name, type in uniforms.iteritems():
            location = glGetUniformLocation(self.id, str(name))
            if location < 0:
                raise Exception("Unknown uniform in program object: {0}".format(name))
            self.uniformMap[name] = (location, type)
        
    def loadShader(self, vs, fs, uniforms):
        if self.id is not None:
            self.unloadShader()
        vsObj = None
        fsObj = None
        error = None
        program = glCreateProgram()
        try:
            vsObj = self._compileShader(GL_VERTEX_SHADER, vs)
            fsObj = self._compileShader(GL_FRAGMENT_SHADER, fs)
            
            glAttachShader(program, vsObj)
            glAttachShader(program, fsObj)
            
            glLinkProgram(program)
            
            glDeleteShader(vsObj)
            glDeleteShader(fsObj)
            
            self.id = program
            self._loadUniforms(uniforms)
        except:
            if vsObj is not None:
                glDeleteShader(vsObj)
            if fsObj is not None:
                glDeleteShader(fsObj)
            glDeleteProgram(program)
            self.id = None
            raise
            
    def loadFromFile(self, vsName, fsName, uniforms):
        f = open(fsName, 'r')
        fs = f.read()
        f.close()
        f = open(vsName, 'r')
        vs = f.read()
        f.close()
        self.loadShader(vs, fs, uniforms)
            
    def unloadShader(self):
        glDeleteProgram(program)
        self.id = None
        
    def __setitem__(self, name, value):
        t = self.uniformMap[name]
        t[1](t[0], value)
        
    def bind(self):
        glUseProgram(self.id)
        
    @staticmethod
    def unbind():
        glUseProgram(0)

