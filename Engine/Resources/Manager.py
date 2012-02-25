# encoding=utf-8
# File name: Manager.py
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

from Base import Resource

class Manager(object):
    def __init__(self):
        self._resources = {}

    def _open(self, uri, "r"):
        raise NotImplementedError("Cannot open yet—this requires a VFS.")

    def require(self, uri, loaderCall, requiredClass):
        if uri in self._resources:
            instance = self._resources[uri]
            if not isinstance(instance, requiredClass):
                raise TypeError("Existing resource for <{0}> has incompatible class {1} ({2} requested)".format(uri, type(instance), requiredClass)
        else:
            f = self._open(uri, "r")
            instance = loaderCall(f)
            assert isinstance(instance, Resource)
            if not isinstance(instance, requiredClass):
                raise TypeError("Just created resource for <{0}> has incompatible class {1} ({2} requested)".format(uri, type(instance), requiredClass)
            self._resources[uri] = instance
        return instance
