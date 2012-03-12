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
from WidgetBase import Widget, ParentWidget
from RootWidget import RootWidget
import unittest

class WidgetInit(unittest.TestCase):
    def test_init(self):
        instance = RootWidget()
        self.assertEqual(len(instance), 3)
        del instance

    def test_separate(self):
        instanceA = RootWidget()
        instanceB = RootWidget()
        self.assertIsNot(instanceA._children, instanceB._children)

class WidgetInstanceTest(unittest.TestCase):
    def setUp(self):
        self.instance = RootWidget()
        self.parent = self.instance._desktopLayer

    def tearDown(self):
        del self.instance

class WidgetChildren(WidgetInstanceTest):
    def test_add(self):
        child = Widget(self.parent)
        self.assertEqual(len(self.parent), 1)
        self.assertIs(child.Parent, self.parent)
        self.assertIn(child, self.parent)
        self.assertIs(self.parent[0], child)
    
    def test_dupe(self):
        child = Widget(self.parent)
        self.assertRaises(ValueError, self.parent.add, child)

    def test_multipleParents(self):
        parentB = RootWidget()._desktopLayer
        child = Widget(self.parent)
        self.assertRaises(ValueError, parentB.add, child)

    def test_multipleChildren(self):
        childA, childB, childC = Widget(parent=self.parent), Widget(parent=self.parent), Widget(parent=self.parent)
        self.assertEqual(list(self.parent), [childA, childB, childC])
        del self.parent[0]
        self.assertEqual(list(self.parent), [childB, childC])

    def test_typeEnforcement(self):
        self.assertRaises(TypeError, self.instance.add, 1)

    def test_init(self):
        child = Widget(parent=self.parent)
        self.assertEqual(len(self.parent), 1)
        self.assertEqual(self.parent[0], child)

class WidgetIteration(WidgetInstanceTest):
    def test_depthFirst(self):
        childA = ParentWidget(self.parent)
        childB = ParentWidget(self.parent)
        childAA = Widget(childA)
        childAB = Widget(childA)
        childBA = Widget(childB)

        self.assertSequenceEqual(
            list(self.parent.treeDepthFirst()),
            [self.parent, childA, childAA, childAB, childB, childBA]
        )
