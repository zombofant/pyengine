# File name: CSSLoader.py
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

from Base import ResourceLoader
from Manager import ResourceManager

from Engine.UI.CSS.Parser import Parser

class CSSLoader(ResourceLoader):
    """
    Implements a loader for CSS (cascading style sheet) files.
    """

    def __init__(self, **kwargs):
        super(CSSLoader, self).__init__(
            [list],
            ['css'],
            relativePathPrefix="/data/css",
            **kwargs)
        self._parser = Parser()

    def load(self, fileLike, targetClass=list):
        assert targetClass is list
        return self._parser.parse(fileLike)

# register an instance of TextLoader with the resource manager
ResourceManager().registerResourceLoader(CSSLoader)

