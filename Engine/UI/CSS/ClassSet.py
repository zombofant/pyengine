# File name: ClassSet.py
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

class ClassSet(object):
    def __init__(self):
        self._classes = set()

    def __iadd__(self, value):
        try:
            self.add(value)
        except TypeError:
            return NotImplemented
        return self

    def __isub__(self, value):
        try:
            self.discard(value)
        except TypeError:
            return NotImplemented
        return self

    def add(self, value):
        if isinstance(value, unicode):
            self._classes.add(value)
        else:
            try:
                iterator = iter(value)
            except TypeError:
                raise TypeError("ClassSet can only contain unicode items. Got {0} {1}".format(type(value), value))
            for cls in iterator:
                assert isinstance(cls, unicode)
                self._classes.add(cls)

    def discard(self, value):
        if isinstance(value, unicode):
            self._classes.discard(value)
        else:
            try:
                iterator = iter(value)
            except TypeError:
                raise TypeError("ClassSet can only contain unicode items. Got {0} {1}".format(type(value), value))
            for cls in iterator:
                assert isinstance(cls, unicode)
                self._classes.discard(cls)

    def __contains__(self, value):
        return value in self._classes

    def __iter__(self):
        return iter(self._classes)

    def __len__(self):
        return len(self._classes)
