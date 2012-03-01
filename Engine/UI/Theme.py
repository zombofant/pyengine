# File name: Theme.py
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

from Style import Style
from WidgetBase import WidgetContainer

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

    def __ne__(self, other):
        r = self.__eq__(other)
        if r is NotImplemented:
            return r
        else:
            return not r


class Theme(object):
    def __init__(self, **kwargs):
        super(Theme, self).__init__(**kwargs)
        self._rules = []
        self._i = 0
        self._cache = {}
        self._perWidgetCache = {}

    def _sort(self):
        self._rules.sort(cmp=lambda x, y: cmp(x[0], y[0]) or cmp(x[1], y[1]))

    def addRules(self, rules, resort=True):
        for rule in rules:
            for i, selector in zip(itertools.count(self._i), rule._selectors):
                self._rules.append((selector.specifity(), i, selector, rule))
            self._i = i
        if resort:
            self._sort()

    def getWidgetStyle(self, widget):
        matchingRules = [(selector, rule) for specifity, i, selector, rule in self._rules if selector.testWidget(widget) is not None]
        if len(matchingRules) == 0:
            return Style()
        selectors, rules = zip(*matchingRules)
        selectors = SelectorTuple(selectors)
        if selectors in self._cache:
            style = self._cache[selectors]
        else:
            style = Style(*rules)
            self._cache[selectors] = style
        return style

    def applyStyles(self, rootWidget):
        for widget in rootWidget.treeDepthFirst():
            widget.ThemeStyle = self.getWidgetStyle(widget)
