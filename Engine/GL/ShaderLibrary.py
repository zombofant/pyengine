# File name: ShaderLibrary.py
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

from Shader import Shader

class ShaderLibrary(object):
    def __init__(self, variables, vertex_program_tree, fragment_program_tree, **kwargs):
        super(ShaderLibrary, self).__init__(**kwargs)
        self._variables = variables
        self._vertex_program_tree = vertex_program_tree
        self._fragment_program_tree = fragment_program_tree
        self._cache = {}

    def _build_shader(self, variables):
        shader = Shader()
        sentinel = dict(variables)
        shader.load_shader(self._vertex_program_tree(sentinel), self._fragment_program_tree(sentinel))
        return shader

    def require_shader(self, variables):
        cachetoken = tuple(variables.iteritems())
        shader = self._cache.get(cachetoken, None)
        if shader is not None:
            return shader
        shader = self._build_shader(variables)
        self._cache[cachetoken] = shader
        return shader

    def cache_shaders(self, variable_dicts):
        for d in variable_dicts:
            actual_vars = dict(self._variables)
            actual_vars.update(d)
            self.require_shader(actual_vars)

    def bind(self, **variables):
        self._variables.update(variables)
        shader = self.require_shader(self._variables)
        shader.bind()
        return shader
        
