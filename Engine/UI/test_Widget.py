# File name: test_Widget.py
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
from Widget import Widget, ParentWidget
from Root import RootWidget
import unittest

class WidgetInit(unittest.TestCase):
    def test_init(self):
        instance = RootWidget()
        self.assertEqual(len(instance), 0)
        del instance

    def test_separate(self):
        instanceA = RootWidget()
        instanceB = RootWidget()
        self.assertIsNot(instanceA._children, instanceB._children)

class WidgetInstanceTest(unittest.TestCase):
    def setUp(self):
        self.instance = RootWidget()

    def tearDown(self):
        del self.instance

class WidgetChildren(WidgetInstanceTest):
    def test_add(self):
        child = Widget(self.instance)
        self.assertEqual(len(self.instance), 1)
        self.assertIs(child.Parent, self.instance)
        self.assertIn(child, self.instance)
        self.assertIs(self.instance[0], child)
    
    def test_dupe(self):
        child = Widget(self.instance)
        self.assertRaises(ValueError, self.instance.add, child)

    def test_multipleParents(self):
        parentB = RootWidget()
        child = Widget(self.instance)
        self.assertRaises(ValueError, parentB.add, child)

    def test_multipleChildren(self):
        childA, childB, childC = Widget(self.instance), Widget(self.instance), Widget(self.instance)
        self.assertEqual(list(self.instance), [childA, childB, childC])
        del self.instance[0]
        self.assertEqual(list(self.instance), [childB, childC])

    def test_typeEnforcement(self):
        self.assertRaises(TypeError, self.instance.add, 1)

    def test_init(self):
        child = Widget(parent=self.instance)
        self.assertEqual(len(self.instance), 1)
        self.assertEqual(self.instance[0], child)

class WidgetIteration(WidgetInstanceTest):
    def test_depthFirst(self):
        childA = ParentWidget(self.instance)
        childB = ParentWidget(self.instance)
        childAA = Widget(childA)
        childAB = Widget(childA)
        childBA = Widget(childB)

        self.assertSequenceEqual(
            list(self.instance.treeDepthFirst()),
            [self.instance, childA, childAA, childAB, childB, childBA]
        )

