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

from Widget import Widget
from CSS import Border, Padding, Margin, Background

class Selector(object):
    def __init__(self, chained=None, **kwargs):
        super(Selector, self).__init__(**kwargs)
        self._chained = chained
    
    def __contains__(self, other):
        if not isinstance(other, Widget):
            raise TypeError("Selector can only test against Widgets. Got {0} {1}".format(type(other), other))
        if self._chained is not None:
            other = other in self._chained
        return self._testWidget(other)

class ParentSelector(Selector):
    def __init__(self, parentClass, **kwargs):
        super(ParentSelector, self).__init__(**kwargs)
        self._parentClass = parentClass

class ChildOf(ParentSelector):
    def _testWidget(self, widget):
        p = widget.Parent
        while p is not None:
            if isinstance(p, self._parentClass):
                return p
        else:
            return None

class DirectChildOf(ParentSelector):
    def _testWidget(self, widget):
        p = widget.Parent
        if isinstance(p, self._parentClass):
            return p
        else:
            return None

class Is(Selector):
    def __init__(self, testClass, **kwargs):
        super(Is, self).__init__(**kwargs)
        self._testClass = testClass
    
    def _testWidget(self, widget):
        if isinstance(widget, self._testClass):
            return widget
        else:
            return None

class HasCSSClass(Selector):
    def __init__(self, cssClass, **kwargs):
        super(HasCSSClass, self).__init__(**kwargs)
        self._cssClass = cssClass

    def _testWidget(self, widget):
        if self._cssClass in widget.CSSClasses:
            return widget
        else:
            return None

class Theme(object):
    def __init__(self, **kwargs):
        super(Theme, self).__init__(**kwargs)

class WidgetStyle(object):
    def __init__(self, **kwargs):
        super(WidgetStyle, self).__init__(**kwargs)
        self._border = Border()
        self._padding = Padding()
        self._margin = Margin()
        self._background = Background()

    @property
    def Border(self):
        return self._border

    @Border.setter
    def Border(self, value):
        self._border.assign(value)

    @property
    def Padding(self):
        return self._padding

    @Padding.setter
    def Padding(self, value):
        self._padding.assign(value)

    @property
    def Margin(self):
        return self._margin

    @Margin.setter
    def Margin(self, value):
        self._margin.assign(value)

    @property
    def Background(self):
        return self._background

    @Background.setter
    def Background(self, value):
        self._background.assign(value)

    
