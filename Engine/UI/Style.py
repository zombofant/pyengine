# File name: Style.py
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

import itertools
import iterutils
import copy

import CSS.Literals as Literals
from CSS.Box import Padding, Margin, BaseBox
from CSS.Border import Border, BorderEdge
from CSS.Fill import Fill, Colour, Transparent, Image
from CSS.Rules import Rule
from CSS.Rect import Rect

class Style(object):
    def __init__(self, *rules, **kwargs):
        self._background = kwargs.pop("background") if "background" in kwargs else Transparent
        self._padding = Padding()
        self._margin = Margin()
        self._border = Border()
        if "padding" in kwargs:
            self.Padding = kwargs.pop("padding")
        if "margin" in kwargs:
            self.Margin = kwargs.pop("margin")
        if "border" in kwargs:
            self.Border = kwargs.pop("border")
        super(Style, self).__init__(**kwargs)
        for rule in rules:
            self._addRule(rule)

    def __deepcopy__(self, memo):
        # FIXME: implement pickle interface
        return Style(
            background=copy.deepcopy(self.Background, memo),
            padding=copy.deepcopy(self.Padding, memo),
            margin=copy.deepcopy(self.Margin, memo),
            border=copy.deepcopy(self.Border, memo)
        )

    def _addRule(self, rule):
        for key, value in rule._properties:
            self._applyProperty(key, value)

    def __add__(self, rules):
        if isinstance(rules, Rule):
            new = copy.deepcopy(self)
            new._addRule(rules)
        else:
            new = copy.deepcopy(self)
            for rule in rules:
                new._addRule(rules)
        return new

    def __eq__(self, other):
        if not isinstance(other, Style):
            return NotImplemented
        return (
            self._background == other._background and
            self._padding == other._padding and
            self._margin == other._margin and
            self._border == other._border
        )

    def __repr__(self):
        return """<Style
    Padding={0!r},
    Margin={1!r},
    Background={2!r},
    Border={3!r}>""".format(self._padding, self._margin, self._background, self._border)
        
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
        if len(value) == 1:
            self.Background.RepeatX = value
            self.Background.RepeatY = value
        elif len(value) == 2:
            self.Background.RepeatX, self.Background.RepeatY = value
        else:
            raise ValueError("Unsupported amount of axis: {0}".format(len(value)))

    def _setBackgroundImage(self, value):
        self.Background = value

    def _setBackgroundColour(self, value):
        self.Background = value

    def _setBorderEdge(self, edge, value):
        setattr(self.Border, edge, BorderEdge(value[0], value[2]))

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
        if value is not None and not isinstance(value, Fill):
            raise TypeError("Background must be a Fill instance. Got {0} {1}".format(type(value), value))
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
        "border-left": lambda self, value: self._setBorderEdge("Left", value),
        "border-right": lambda self, value: self._setBorderEdge("Right", value),
        "border-top": lambda self, value: self._setBorderEdge("Top", value),
        "border-bottom": lambda self, value: self._setBorderEdge("Bottom", value),
        "padding-left": lambda self, value: self._setBaseBoxEdge("Padding", "Left", value),
        "padding-right": lambda self, value: self._setBaseBoxEdge("Padding", "Right", value),
        "padding-top": lambda self, value: self._setBaseBoxEdge("Padding", "Top", value),
        "padding-bottom": lambda self, value: self._setBaseBoxEdge("Padding", "Bottom", value),
        "margin-left": lambda self, value: self._setBaseBoxEdge("Margin", "Left", value),
        "margin-right": lambda self, value: self._setBaseBoxEdge("Margin", "Right", value),
        "margin-top": lambda self, value: self._setBaseBoxEdge("Margin", "Top", value),
        "margin-bottom": lambda self, value: self._setBaseBoxEdge("Margin", "Bottom", value),
    }

    def geometryForRect(self, rect, faceBuffer):
        """
        Takes a widget rect and creates ui geometry for it using
        the geometryForRect calls of the respective filler objects.

        Returns a Rect instance representing the client rect for the
        passed rect, that is the rect shrinked by any Border and Padding
        defined in this style.

        Does not take into account margin.
        """
        clientRect = copy.copy(rect)
        clientRect.shrink(self.Border.geometryForRect(rect, faceBuffer))
        clientRect.shrink(self.Padding)
        return clientRect
        
