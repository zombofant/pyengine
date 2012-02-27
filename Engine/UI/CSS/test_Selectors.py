# File name: test_Selectors.py
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

import unittest

import Selectors

class SelectorEq(unittest.TestCase):
    def test_switchAttributes(self):
        instanceA = Selectors.HasAttributes(Selectors.AttributeClass("testclass"), chained=Selectors.HasAttributes(Selectors.AttributeExists("testattr")))
        instanceB = Selectors.HasAttributes(Selectors.AttributeExists("testattr"), chained=Selectors.HasAttributes(Selectors.AttributeClass("testclass")))
        self.assertEqual(instanceA, instanceB)

selectorClasses = [getattr(Selectors, name) for name in Selectors.__all__]

def buildSetup(cls):
    if issubclass(cls, Selectors.ParentSelector):
        def setup():
            return cls(int)
    elif issubclass(cls, Selectors.Is):
        def setup():
            return cls(int)
    elif issubclass(cls, Selectors.HasAttributes):
        def setup():
            return cls(Selectors.AttributeExists("test"), Selectors.AttributeValue("test2", "value"), Selectors.AttributeClass("testclass2"))
    else:
        raise NotImplementedError("Don't know how to setup a test for {0}".format(cls))
    return setup

def buildSubTest(class1, class2, setup1, setup2):
    if class1 == class2:
        def testEq(self):
            instance1, instance2 = setup1(), setup2()
            self.assertEqual(instance1, instance2)
        return testEq
    else:
        def testNe(self):
            instance1, instance2 = setup1(), setup2()
            self.assertNotEqual(instance1, instance2)
        return testNe

def buildTest(class1, setup1, classes):
    tests = [buildSubTest(class1, class2, setup1, buildSetup(class2)) for class2 in classes]
    def test(self):
        for test in tests:
            test(self)
    return test

def populateTests(classes):
    for class1 in selectorClasses:
        #for class2 in selectorClasses:
        #    setattr(SelectorEqTest, "test_{0}_{1}".format(class1.__name__, class2.__name__), buildTest(class1, class2, buildSetup(class1), buildSetup(class2)))
        setattr(SelectorEq, "test_{0}".format(class1.__name__), buildTest(class1, buildSetup(class1), classes))

populateTests(selectorClasses)


class SelectorSpecifity(unittest.TestCase):
    """
    Tests the conformance to the W3C CSS specification regarding the
    specifity of rules.

    See: <http://www.w3.org/TR/CSS2/cascade.html#specificity>
    """

    def test_ParentSelector(self):
        self.assertEqual(Selectors.Specifity(0, 0, 0, 1), Selectors.ChildOf(int).specifity())
        self.assertEqual(Selectors.Specifity(0, 0, 0, 1), Selectors.DirectChildOf(int).specifity())

    def test_Is(self):
        self.assertEqual(Selectors.Specifity(0, 0, 0, 1), Selectors.Is(int).specifity())

    def test_HasAttribute(self):
        self.assertEqual(
            Selectors.Specifity(0, 0, 3, 0),
            Selectors.HasAttributes(
                Selectors.AttributeExists("test"),
                Selectors.AttributeValue("test2", "value"),
                Selectors.AttributeClass("test3")
            ).specifity())

class SelectorChainFolding(unittest.TestCase):
    def test_HasAttributes(self):
        attr1 = Selectors.AttributeExists("test")
        attr2 = Selectors.AttributeExists("test2")
        instanceB = Selectors.HasAttributes(attr1)
        instanceA = Selectors.HasAttributes(attr2, chained=instanceB)
        self.assertIsNone(instanceA._chained)
        self.assertEqual(instanceA._attrs, set((attr1, attr2)))

        instanceC = Selectors.Is(int)
        instanceB = Selectors.HasAttributes(attr1, chained=instanceC)
        instanceA = Selectors.HasAttributes(attr2, chained=instanceB)
        self.assertIs(instanceA._chained, instanceC)
        self.assertEqual(instanceA._attrs, set((attr1, attr2)))
        
    def test_HasCSSClasses(self):
        attr1 = Selectors.AttributeClass("test")
        attr2 = Selectors.AttributeClass("test2")
        instanceB = Selectors.HasAttributes(attr1)
        instanceA = Selectors.HasAttributes(attr2, chained=instanceB)
        self.assertIsNone(instanceA._chained)
        self.assertEqual(instanceA._attrs, set((attr1, attr2)))

        instanceC = Selectors.Is(int)
        instanceB = Selectors.HasAttributes(attr1, chained=instanceC)
        instanceA = Selectors.HasAttributes(attr2, chained=instanceB)
        self.assertIs(instanceA._chained, instanceC)
        self.assertEqual(instanceA._attrs, set((attr1, attr2)))
