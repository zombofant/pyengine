# File name: Utils.py
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

import Constants

_INHERITABLE = "css_inheritable_flag"
_RECURSE = "css_inheritance_recurse"

class css_inheritable(property):
    pass

def css_inheritance_recurse(subclass):
    class _css_inheritance_recurse(css_inheritable):
        recurse_into = subclass
    return _css_inheritance_recurse

class Inheritables(type):
    @classmethod
    def find_inheritables(mcls, dct):
        for candidate in dct.values():
            if isinstance(candidate, css_inheritable):
                try:
                    subcls = candidate.recurse_into
                except AttributeError:
                    yield (candidate,)
                else:
                    for v in mcls.find_inheritables(subcls.__dict__):
                        yield (candidate,) + v


    @staticmethod
    def getNestedProperty(self, path, at=None):
        curr = at or self
        for node in path:
            curr = node.__get__(curr, curr.__class__)
        return curr

    @staticmethod
    def setNestedProperty(self, path, value):
        curr = self
        for node in path[:-1]:
            curr = node.__get__(curr, curr.__class__)
        path[-1].__set__(curr, value)

    @staticmethod
    def solveInheritance(self, parent):
        for prop in self.__inheritables__:
            value = self.getNestedProperty(prop)
            if value is Constants.Inherit:
                self.setNestedProperty(prop, self.getNestedProperty(prop, parent))


    def __new__(mcls, name, bases, dct):
        inheritables = list(mcls.find_inheritables(dct))
        dct["__inheritables__"] = inheritables
        dct["solveInheritance"] = mcls.solveInheritance
        dct["getNestedProperty"] = mcls.getNestedProperty
        dct["setNestedProperty"] = mcls.setNestedProperty
        return type.__new__(mcls, name, bases, dct)
