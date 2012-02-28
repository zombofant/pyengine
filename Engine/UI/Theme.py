# File name: Themes.py
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

class SelectorTuple(tuple):
    def __eq__(self, other):
        if not isinstance(self, SelectorTuple):
            return NotImplemented
        if len(other) != len(self):
            return False
        for myRule, otherRule in zip(self, other):
            if myRule is not otherRule:
                return False
        return True


class Style(object):
    def __init__(self, *rules, **kwargs):
        super(Style, self).__init__(rules)
        for rule in self._mergedRules:
            self._addRule(rule)

    def _addRule(self, rule):
        for key, value in rule._properties:
            self._applyProperty(key, value)
        
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
        setattr(self.Border, edge, value)

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


class Theme(object):
    def __init__(self, **kwargs):
        super(Theme, self).__init__(**kwargs)
        self._rules = []
        self._i = 0

    def _sort(self):
        self._rules.sort(cmp=lambda x, y: cmp(x[0], y[0]) or cmp(x[1], y[1]))

    def addRules(self, rules, resort=True):
        for rule in rules:
            for i, selector in zip(itertools.count(self._i), rule._selectors):
                self._rules.append((selector.specifity(), i, rule))
            self._i = i
        if resort:
            self._sort()
