# File name: Pango.py
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
import itertools

from gi.repository import Pango
from gi.repository import PangoCairo

def import_namespace(from_gi_module):
    globals_dict = globals()
    names = dir(from_gi_module)
    for name in names:
        try:
            globals_dict[name] = getattr(from_gi_module, name)
        except AttributeError:
            pass

import_namespace(PangoCairo)
import_namespace(Pango)

# _EllipsizeMode = EllipsizeMode
# _WrapMode = WrapMode
# _Alignment = Alignment
# _Weight = Weight

# class GObjectEnum(type):
#     def __new__(mcls, name, bases, dct):
#         enum = dct.pop("__enum__")
#         iterator = dct.pop("__index_iter__", itertools.count(0))
#         for index in iterator:
#             try:
#                 value = enum(index)
#             except ValueError:
#                 break
#             v, k = mcls.parseEnum(enum(index))
#             dct[k] = v

#         return type.__new__(mcls, name, bases, dct)

# class EllipsizeMode(object):
#     __metaclass__ = GObjectEnum
#     __enum__ = _EllipsizeMode


# class WrapMode(object):
#     __metaclass__ = GObjectEnum
#     __enum__ = _WrapMode


# class Alignment(object):
#     __metaclass__ = GObjectEnum
#     __enum__ = _Alignment


# class Weight(object):
#     __metaclass__ = GObjectEnum
#     __enum__ = _Weight
#     __index_iter__ = [100, 200, 300, 380,
