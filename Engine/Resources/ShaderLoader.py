# File name: ShaderLoader.py
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

import re

from Base import ResourceLoader
from Manager import ResourceManager
import TextLoader

try:
    import Engine.GL.Shader as Shader
except ImportError:
    pass

class ShaderParser(object):
    _transition = re.compile("^\[([\w\s]+)\]\s*$", re.I)
    _transitionMap = {
        re.compile(r"^(vertex( shader)?|vs)$", re.I): 1,
        re.compile(r"^(fragment( shader)?|fs)$", re.I): 2
    }
    _preprocessor = re.compile(r"^\s*#(include)\s(.*)\s*$")
    _outsideComment = re.compile(r"^\s*#\s.*$")

    _includeFile = re.compile(r'^"([^\"]+)"$')
    
    def __init__(self, lineIterable):
        self.lineIterableStack = [iter(lineIterable)]
        self.state = 0 # 0; outside; 1: vertex shader; 2: fragment shader
        self.fp = None
        self.vp = None

    def _getLine(self):
        while len(self.lineIterableStack) > 0:
            try:
                return next(self.lineIterableStack[-1])
            except StopIteration:
                self.lineIterableStack.pop()
                pass
        else:
            return None

    def _include(self, argument):
        includeFile = self._includeFile.match(argument.strip())
        if not includeFile:
            raise ValueError("Invalid include command: {0}".format(argument))
        fileName = includeFile.group(1)
        iterable = ResourceManager().require(fileName, resourceType="txt").split("\n")
        self.lineIterableStack.append(iter(iterable))

    def _doTransition(self, line):
        transition = self._transition.match(line)
        if transition:
            target = transition.group(1).strip()
            for expr, state in self._transitionMap.iteritems():
                if expr.match(target):
                    self.state = state
                    return True
            else:
                raise ValueError("Invalid shader section: {0}".format(target))
        return False

    def _doPreprocessing(self, line):
        preprocessing = self._preprocessor.match(line)
        if preprocessing:
            command, argument = preprocessing.groups()[0:2]
            if command in self._preprocessorMap:
                self._preprocessorMap[command](self, argument)
                return True
            raise ValueError("Invalid preprocessor command: {0}".format(command))
        return False

    def _outsideLine(self, line):
        if self._doTransition(line):
            return
        if self._doPreprocessing(line):
            return
        if len(line.strip()) == 0:
            return
        raise ValueError("Garbage outside shader section: {0}".format(line))

    def _vertexShaderLine(self, line):
        if self._doTransition(line):
            return
        if self._doPreprocessing(line):
            return
        if self.vp is None:
            self.vp = line
            return
        self.vp += "\n" + line

    def _fragmentShaderLine(self, line):
        if self._doTransition(line):
            return
        if self._doPreprocessing(line):
            return
        if self.fp is None:
            self.fp = line
            return
        self.fp += "\n" + line

    def parse(self):
        line = self._getLine()
        while line is not None:
            line = line.rstrip()
            self._stateMap[self.state](self, line)
            line = self._getLine()
        return self.vp, self.fp
    
    _stateMap = {
        0: _outsideLine,
        1: _vertexShaderLine,
        2: _fragmentShaderLine
    }

    _preprocessorMap = {
        "include": _include
    }

class ShaderLoader(ResourceLoader):
    
    def __init__(self, **kwargs):
        try:
            super(ShaderLoader, self).__init__(
                [Shader.Shader],
                ['shader'],
                relativePathPrefix="/data/shader"
                **kwargs)
        except NameError:
            self._loaderNotAvailable()

    def load(self, filelike, targetClass, uniforms=None, encoding="utf8"):
        vpSource, fpSource = ShaderParser((line.decode(encoding) for line in filelike)).parse()
        shader = Shader.Shader()
        shader.loadShader(vpSource, fpSource, uniforms or {})
        return shader
