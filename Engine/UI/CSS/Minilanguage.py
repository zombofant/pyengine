# File name: Minilanguage.py
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

from Fill import Transparent, Colour, Gradient, Image, Stretch, Repeat
from Box import BaseBox
from Rules import Rule
from Rect import Rect
from Selectors import *
from Selectors import AttributeClass, AttributeExists, AttributeValue
from Literals import *

class StylesheetNamespace(object):
    image = Image
    gradient = Gradient
    step = Gradient.Step
    rgba = Colour.rgba
    hsva = Colour.hsva
    hsla = Colour.hsla
    stretch = Stretch
    repeat = Repeat
    rect = Rect
    url = URLLiteral
    transparent = Transparent
    solid = "solid"

    _tokenBlacklist = ["evaluateCall", "get"]

    __singleton = None

    def __new__(cls, *args, **kwargs):
        if cls.__singleton is None:
            return super(type(StylesheetNamespace), cls).__new__(cls, *args, **kwargs)
        else:
            return cls.__singleton

    def __init__(self):
        if not hasattr(self, "_initialized"):
            super(StylesheetNamespace, self).__init__()
            self._initialized = True

    def evaluateCall(self, call, *args):
        call = self.get(call)
        return call(*args)

    def get(self, token):
        token.lower()
        if token.startswith("_") or token in self._tokenBlacklist or not hasattr(self, token):
            raise ValueError("Token {0!r} not defined in css".format(token))
        return getattr(self, token)

class ElementNames(dict):
    __singleton = None
    
    def __new__(cls, *args, **kwargs):
        if cls.__singleton is None:
            cls.__singleton = dict.__new__(cls, *args, **kwargs)
            return cls.__singleton
        else:
            return cls.__singleton

    def __init__(self):
        if not hasattr(self, "_initialized"):
            super(ElementNames, self).__init__()
            self._initialized = True

    def __getitem__(self, key):
        return super(ElementNames, self).__getitem__(key.lower())

    def __setitem__(self, key, value):
        return super(ElementNames, self).__setitem__(key.lower(), value)

    def __delitem__(self, key):
        return super(ElementNames, self).__delitem__(key.lower())

    def update(self, otherdict):
        for key, value in otherdict.iteritems():
            self[key] = value

    def registerWidgetClass(self, widgetClass):
        self[widgetClass.__name__] = widgetClass

elementNames = ElementNames()
namespace = StylesheetNamespace()
    
