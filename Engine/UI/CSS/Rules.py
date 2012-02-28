# File name: Rules.py
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

from Properties import Background, BaseBox, Padding, Margin, Border
from Values import RGBA
import Literals

class Rule(object):
    def __init__(self, selectors, properties,
            background=None,
            border=None,
            padding=None,
            margin=None,
            **kwargs):
        self._border = Border()
        self._padding = Padding()
        self._margin = Margin()
        super(Rule, self).__init__(**kwargs)
        self._selectors = selectors
        self.Background = background
        self.Border = border or Border()
        self.Padding = padding or Padding()
        self.Margin = margin or Margin()
        for key, value in properties:
            self._applyProperty(key, value)

    def __eq__(self, other):
        if not isinstance(other, Rule):
            return NotImplemented
        return (self._selectors == other._selectors and
                self.Background == other.Background and
                self.Padding == other.Padding and
                self.Border == other.Border and
                self.Margin == other.Margin)

    def __unicode__(self):
        return """{0} {{
    {1}
}}""".format(
            "\n, ".join(map(unicode, self._selectors)),
            ""
        )

    def __repr__(self):
        return "<{0} selectors={1}>".format(self.__class__.__name__, len(self._selectors))

    def _applyProperty(self, key, value):
        if key in self._literalSetters:
            if not isinstance(value, tuple):
                value = (value, )
            literalClass, propName = self._literalSetters[key]
            setattr(self, propName, literalClass(*value))
            return
        if key in self._propertySetters:
            self._propertySetters[key](self, value)
            return
        raise KeyError("{0} is not a valid stylesheet property".format(key))

    def _setBackgroundRepeat(self, value):
        pass

    def _setBackgroundImage(self, value):
        pass

    def _setBackgroundColour(self, value):
        pass

    def _setBorderEdge(self, edge, value):
        pass

    def _setBaseBoxEdge(self, box, edge, value):
        box = getattr(self, box)
        if len(value) != 1:
            raise ValueError("Setting a box edge requires exactly one number.")
        setattr(box, edge, value[0])

    @property
    def Background(self):
        return self._background

    @Background.setter
    def Background(self, value):
        if value is not None and not isinstance(value, Background):
            raise TypeError("Background must be a Background instance or None. Got {0} {1}".format(type(value), value))
        self._background = value

    @property
    def Padding(self):
        return self._padding

    @Padding.setter
    def Padding(self, value):
        if not isinstance(value, BaseBox):
            raise TypeError("Padding must be a BaseBox instance. Got {0} {1}".format(type(value), value))
        self._padding.assign(value)

    @property
    def Margin(self):
        return self._margin

    @Margin.setter
    def Margin(self, value):
        if not isinstance(value, BaseBox):
            raise TypeError("Margin must be a BaseBox instance. Got {0} {1}".format(type(value), value))
        self._margin.assign(value)

    @property
    def Border(self):
        return self._border

    @Border.setter
    def Border(self, value):
        if not isinstance(value, Border):
            raise TypeError("Border must be a Border instance. Got {0} {1}".format(type(value), value))
        self._border.assign(value)

    _literalSetters = {
        "background": (Literals.BackgroundLiteral, "Background"),
        "padding": (Literals.BoxLiteral, "Padding"),
        "margin": (Literals.BoxLiteral, "Margin"),
        "border": (Literals.BorderLiteral, "Border"),
    }

    _propertySetters = {
        "background-image": lambda self, value: self._setBackgroundImage(value),
        "background-repeat": lambda self, value: self._setBackgroundRepeat(value),
        "background-color": lambda self, value: self._setBackgroundColour(value),
        "border-left": lambda self, value: self._setBorderEdge("Left", edge),
        "border-right": lambda self, value: self._setBorderEdge("Right", edge),
        "border-top": lambda self, value: self._setBorderEdge("Top", edge),
        "border-bottom": lambda self, value: self._setBorderEdge("Bottom", edge),
        "padding-left": lambda self, value: self._setBaseBoxEdge("Padding", "Left", value),
        "padding-right": lambda self, value: self._setBaseBoxEdge("Padding", "Right", value),
        "padding-top": lambda self, value: self._setBaseBoxEdge("Padding", "Top", value),
        "padding-bottom": lambda self, value: self._setBaseBoxEdge("Padding", "Bottom", value),
        "margin-left": lambda self, value: self._setBaseBoxEdge("Margin", "Left", value),
        "margin-right": lambda self, value: self._setBaseBoxEdge("Margin", "Right", value),
        "margin-top": lambda self, value: self._setBaseBoxEdge("Margin", "Top", value),
        "margin-bottom": lambda self, value: self._setBaseBoxEdge("Margin", "Bottom", value),
    }
