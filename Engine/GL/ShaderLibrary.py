# File name: ShaderLibrary.py
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

from Shader import Shader

class ShaderLibrary(object):
    def __init__(self, variables, vertexProgramTree, fragmentProgramTree, **kwargs):
        super(ShaderLibrary, self).__init__(**kwargs)
        self._variables = variables
        self._vertexProgramTree = vertexProgramTree
        self._fragmentProgramTree = fragmentProgramTree
        self._cache = {}

    def _buildShader(self, variables):
        shader = Shader()
        shader.loadShader(self._vertexProgramTree(variables), self._fragmentProgramTree(variables))
        return shader

    def requireShader(self, variables):
        cacheToken = tuple(variables.iteritems())
        if cacheToken in self._cache:
            shader = self._cache[cacheToken]
        else:
            shader = self._buildShader(variables)
            self._cache[cacheToken] = shader
        return shader

    def cacheShaders(self, variableDicts):
        for d in variableDicts:
            actualVars = dict(self._variables)
            actualVars.update(d)
            self.requireShader(actualVars)

    def bind(self, **variables):
        self._variables.update(variables)
        shader = self.requireShader(self._variables)
        shader.bind()
        return shader
        
