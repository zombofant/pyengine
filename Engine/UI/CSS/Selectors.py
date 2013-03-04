# File name: Selectors.py
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

__all__ = ["ChildOf", "DirectChildOf", "Is",
    "HasAttributes", "State"]

class Specifity(object):
    def __init__(self, *args):
        if len(args) == 0:
            args = (0, 0, 0, 0)
        elif len(args) == 4:
            args = tuple(args)
        else:
            raise TypeError("Specifity() requires at zero or four arguments.")
        self._value = tuple(args)

    def __add__(self, other):
        if not isinstance(other, Specifity):
            return NotImplemented
        return Specifity(*(myVal + otherVal for myVal, otherVal in zip(self._value, other._value)))

    def __iadd__(self, other):
        if not isinstance(other, Specifity):
            return NotImplemented
        self._value = tuple((myVal + otherVal for myVal, otherVal in zip(self._value, other._value)))
        return self

    def __cmp__(self, other):
        if not isinstance(other, Specifity):
            return NotImplemented
        return cmp(self._value, other._value)

    def __repr__(self):
        return "Specifity({0})".format(", ".join(unicode(v) for v in self._value))

class Selector(object):
    def __init__(self, chained=None, **kwargs):
        super(Selector, self).__init__(**kwargs)
        self._chained = chained

    def testWidget(self, widget):
        if self._chained is not None:
            widget = self._chained.testWidget(widget)
        if widget is not None:
            return self._testWidget(widget)
        else:
            return None

    def _testEq(self, other):
        return (self._chained == other._chained)

    def __eq__(self, other):
        if not isinstance(other, Selector):
            return NotImplemented
        if not isinstance(other, self.__class__):
            return False
        return self._testEq(other)

    def __hash__(self):
        return self._hash()

    def __ne__(self, other):
        if not isinstance(other, Selector):
            return NotImplemented
        if not isinstance(other, self.__class__):
            return True
        return not self._testEq(other)

    def specifity(self):
        if self._chained is not None:
            return self._chained.specifity()
        else:
            return Specifity(0, 0, 0, 0)

class ParentSelector(Selector):
    def __init__(self, parentSelector, **kwargs):
        super(ParentSelector, self).__init__(**kwargs)
        self._parentSelector = parentSelector

    def _testEq(self, other):
        return super(ParentSelector, self)._testEq(other) and self._parentSelector == other._parentSelector

    def _hash(self):
        return hash(self.__class__) ^ hash(self._parentSelector) ^ hash(self._chained)

    def specifity(self):
        specifity = super(ParentSelector, self).specifity()
        specifity += Specifity(0, 0, 0, 1)
        return specifity

class ChildOf(ParentSelector):
    def _testWidget(self, widget):
        p = widget.Parent
        while p is not None:
            if self._parentSelector.testWidget(p):
                return p
            if not hasattr(p, "Parent"):
                return None
            p = p.Parent
        else:
            return None

    def __unicode__(self):
        return "{0} {1}".format(self._parentSelector, self._chained)

class DirectChildOf(ParentSelector):
    def _testWidget(self, widget):
        if not hasattr(widget, "Parent"):
            return None
        p = widget.Parent
        if self._parentSelector.testWidget(p):
            return p
        else:
            return None

    def __unicode__(self):
        return "{0} > {1}".format(self._parentSelector, self._chained)

class Is(Selector):
    def __init__(self, testClass, **kwargs):
        super(Is, self).__init__(**kwargs)
        self._testClass = testClass

    def _testWidget(self, widget):
        if isinstance(widget, self._testClass):
            return widget
        else:
            return None

    def _testEq(self, other):
        return super(Is, self)._testEq(other) and self._testClass == other._testClass

    def _hash(self):
        return hash(self.__class__) ^ hash(self._testClass) ^ hash(self._chained)

    def __unicode__(self):
        return "{0}".format(self._testClass)

    def specifity(self):
        specifity = super(Is, self).specifity()
        specifity += Specifity(0, 0, 0, 1)
        return specifity

class Attribute(object):
    def __init__(self, **kwargs):
        super(Attribute, self).__init__(**kwargs)

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return NotImplemented
        return self._testEq(other)

    def __hash__(self):
        return self._hash()

    def __ne__(self, other):
        if not isinstance(other, self.__class__):
            return NotImplemented
        return not self._testEq(other)

class AttributeClass(Attribute):
    def __init__(self, className, **kwargs):
        super(AttributeClass, self).__init__(**kwargs)
        self._className = className

    def testWidget(self, widget):
        return self._className in widget.StyleClasses

    def _hash(self):
        return hash(self.__class__) ^ hash(self._className)

    def _testEq(self, other):
        return self._className == other._className

    def __unicode__(self):
        return ".{0}".format(self._className)

class AttributeExists(Attribute):
    def __init__(self, attrName, **kwargs):
        super(AttributeExists, self).__init__(**kwargs)
        self._attrName = attrName

    def testWidget(self, widget):
        return hasattr(widget, self._attrName)

    def _hash(self):
        return hash(self.__class__) ^ hash(self._attrName)

    def _testEq(self, other):
        return self._attrName == other._attrName

    def __unicode__(self):
        return "[{0}]".format(self._attrName)

class AttributeValue(AttributeExists):
    def __init__(self, attrName, attrValue, **kwargs):
        super(AttributeValue, self).__init__(attrName, **kwargs)
        self._attrValue = attrValue

    def testWidget(self, widget):
        return super(AttributeValue, self).testWidget(widget) and getattr(widget, self._attrName) == self._attrValue

    def _hash(self):
        return hash(self.__class__) ^ hash(self._attrName) ^ hash(self._attrValue)

    def _testEq(self, other):
        return self._attrName == other._attrName and self._attrValue == other._attrValue

    def __unicode__(self):
        return '[{0}="{1}"]'.format(self._attrName, self._attrValue)

class HasAttributes(Selector):
    def __init__(self, *attrs, **kwargs):
        chainedAttrs = None
        if "chained" in kwargs:
            chained = kwargs["chained"]
            del kwargs["chained"]

            if isinstance(chained, HasAttributes):
                chainedAttrs = chained._attrs
                chained = chained._chained
        else:
            chained = None
        super(HasAttributes, self).__init__(chained=chained, **kwargs)
        self._attrs = set(attrs)
        for attr in self._attrs:
            if not isinstance(attr, Attribute):
                raise TypeError("HasAttributes only accepts Attribute instances.")
        if chainedAttrs is not None:
            self._attrs |= chainedAttrs
        self._attrs = frozenset(self._attrs)

    def _testWidget(self, widget):
        for attr in self._attrs:
            if not attr.testWidget(widget):
                return None
        return widget

    def _testEq(self, other):
        return super(HasAttributes, self)._testEq(other) and self._attrs == other._attrs

    def _hash(self):
        return hash(self.__class__) ^ hash(self._attrs)

    def __unicode__(self):
        return "{0}{1}".format(self._chained, "".join(map(unicode, self._attrs)))

    def specifity(self):
        specifity = super(HasAttributes, self).specifity()
        specifity += Specifity(0, 0, len(self._attrs), 0)
        return specifity

class State(Selector):
    state_map = {
        "hover": 0,
        "active": 1,
        "focused": 2,
    }

    def __init__(self, statestr, chained=None, **kwargs):
        if isinstance(chained, State):
            state = list(chained._state)
            chained = chained._chained
        else:
            state = [False]*len(self.state_map)
        state[self.state_map[statestr.lower()]] = True

        super(State, self).__init__(chained=chained, **kwargs)

        self._state = tuple(state)

    def _testWidget(self, widget):
        if any(my_state and not widget_state
               for widget_state, my_state
               in zip(widget.CSSState, self._state)):
            return None
        return widget

    def __unicode__(self):
        return "{0}:{1}".format(self._chained, ":".join(
                name
                for name, i in self.state_map.items()
                if self._state[i]
                ))

    def _hash(self):
        return hash(self._state)
