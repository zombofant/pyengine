# File name: Selectors.py
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

__all__ = ["ChildOf", "DirectChildOf", "Is", "HasCSSClass", "Or",
    "HasAttributes"]

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

    def __unicode__(self):
        return "{0} {1}".format(self._parentClass, self._chained)


class DirectChildOf(ParentSelector):
    def _testWidget(self, widget):
        p = widget.Parent
        if isinstance(p, self._parentClass):
            return p
        else:
            return None

    def __unicode__(self):
        return "{0} > {1}".format(self._parentClass, self._chained)


class Is(Selector):
    def __init__(self, testClass, **kwargs):
        super(Is, self).__init__(**kwargs)
        self._testClass = testClass
    
    def _testWidget(self, widget):
        if isinstance(widget, self._testClass):
            return widget
        else:
            return None

    def __unicode__(self):
        return "{0}".format(self._parentClass)


class Or(Selector):
    def __init__(self, *selectors, **kwargs):
        if "chained" in kwargs:
            selectors.append(kwargs["chained"])
            del kwargs["chained"]
        super(Or, self).__init__(**kwargs)
        self._selectors = list(selectors)

    def _testWidget(self, widget):
        for selector in self._selectors:
            result = selector._testWidget(widget)
            if result is not None:
                return result
        return None

    def addSelector(self, selector):
        self._selectors.append(selector)

    def __unicode__(self):
        return ", ".join(self._selectors)


class HasCSSClass(Selector):
    def __init__(self, cssClass, **kwargs):
        super(HasCSSClass, self).__init__(**kwargs)
        self._cssClass = cssClass

    def _testWidget(self, widget):
        if self._cssClass in widget.CSSClasses:
            return widget
        else:
            return None

    def __unicode__(self):
        return "{0}.{1}".format(self._chained, self._cssClass)


class HasAttributes(Selector):
    def __init__(self, *attrs, **kwargs):
        super(Selector, self).__init__(**kwargs)
        self._attrs = list(attrs)
        for i, attr in enumerate(self._attrs):
            attr = tuple(attr)
            if len(attr) < 1 or len(attr) > 2:
                raise ValueError("HasAttributes only accepts 1-tuples and 2-tuples.")
            self._attrs[i] = attr
    
    def _testWidget(self, widget):
        for attr in self._attrs:
            if not hasattr(widget, attr[0]):
                return None
            if len(attr) == 2:
                if not unicode(getattr(widget, attr[0])) == attr[1]:
                    return None
        return widget

    def __unicode__(self):
        return "{0}[{1}]".format(self._chained, self._attrName)
