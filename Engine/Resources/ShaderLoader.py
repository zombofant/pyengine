# File name: ShaderLoader.py
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

import re

from Base import ResourceLoader
from Manager import ResourceManager
from ShaderParser import ShaderParser
import TextLoader

try:
    import Engine.GL.ShaderLibrary as ShaderLibrary
except ImportError:
    pass

class ShaderLoader(ResourceLoader):
    
    def __init__(self, **kwargs):
        try:
            super(ShaderLoader, self).__init__(
                [ShaderLibrary.ShaderLibrary],
                ['shader'],
                relativePathPrefix="/data/shader",
                **kwargs)
        except NameError:
            self._loaderNotAvailable()

    def load(self, filelike, targetClass, encoding="utf8"):
        variables, vpSource, fpSource = ShaderParser((line.decode(encoding) for line in filelike)).parse()
        return ShaderLibrary.ShaderLibrary(variables, vpSource, fpSource)

ResourceManager().registerResourceLoader(ShaderLoader)
