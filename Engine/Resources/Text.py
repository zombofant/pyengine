# File name: Text.py
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

class TextLoader(ResourceLoader):
    """
    Implement a loader for text resources.
    """

    def __init__(self, **kwargs):
        super(TextLoader, self).__init__(**kwargs)
        self._supportedTargetClasses = [unicode, str]
        self._defaultTargetClass = unicode
        self._resourceTypes = ['txt']

    def load(self, fileLike, targetClass=unicode, encoding="utf8"):
        text = "\n" . join((line.decode(encoding) for line in fileLike))
        if targetClass is str:
            return str(text)
        else:
            return unicode(text)

# register an instance of TextLoader with the resource manager
from Manager import ResourceManager
ResourceManager().registerResourceLoader(TextLoader())

