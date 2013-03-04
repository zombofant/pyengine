# File name: Constants.py
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

class VerticalAlign(object):
    @staticmethod
    def Top(object_h, outer_h):
        return 0

    @staticmethod
    def Bottom(object_h, outer_h):
        return outer_h - object_h

    @staticmethod
    def Middle(object_h, outer_h):
        return outer_h / 2 - object_h / 2


class auto(object):
    class __metaclass__(type):
        def __str__(self):
            return str(unicode(self))

        def __unicode__(self):
            return "auto"

        def __repr__(self):
            return "<css auto>"

    def __str__(self):
        return str(unicode(self.__class__))

    def __repr__(self):
        return repr(self.__class__)

class inherit(object):
    class __metaclass__(type):
        def __str__(self):
            return str(unicode(self))

        def __unicode__(self):
            return "inherit"

        def __repr__(self):
            return "<css inherit>"

    def __str__(self):
        return str(unicode(self.__class__))

    def __repr__(self):
        return repr(self.__class__)

_Auto = auto
del auto
_Inherit = inherit
del inherit

Auto = _Auto()
Inherit = _Inherit()
