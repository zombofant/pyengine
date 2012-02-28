# File name: test_Properties.py
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
from random import randint

import Box
import Border
import Fill

class BoxModelTest(unittest.TestCase):
    def checkValues(self, l, t, r, b):
        self.assertEqual(self.instance.Left, l)
        self.assertEqual(self.instance.Top, t)
        self.assertEqual(self.instance.Right, r)
        self.assertEqual(self.instance.Bottom, b)

class BoxModelInit(BoxModelTest):
    def test_init0(self):
        self.instance = Box.BaseBox()
        self.checkValues(0, 0, 0, 0)

    def test_init1(self):
        r = randint(0, 100)
        self.instance = Box.BaseBox(r)
        self.checkValues(r, r, r, r)

    def test_init4(self):
        # guarantee that all values are random and non-equal
        l = randint(1, 100)
        r = randint(1, 100) + l
        t = randint(1, 100) + r
        b = randint(1, 100) + t
        self.instance = Box.BaseBox(l, t, r, b)
        self.checkValues(l, t, r, b)

    def tearDown(self):
        del self.instance

class BoxModelInstanceTest(BoxModelTest):
    def setUp(self):
        self.instance = Box.BaseBox()

    def tearDown(self):
        del self.instance

class BoxModelProperties(BoxModelInstanceTest):
    def test_left(self):
        v = randint(0, 100)
        self.instance.Left = v
        self.assertEqual(self.instance.Left, v)

    def test_right(self):
        v = randint(0, 100)
        self.instance.Right = v
        self.assertEqual(self.instance.Right, v)

    def test_top(self):
        v = randint(0, 100)
        self.instance.Top = v
        self.assertEqual(self.instance.Top, v)

    def test_bottom(self):
        v = randint(0, 100)
        self.instance.Bottom = v
        self.assertEqual(self.instance.Bottom, v)

class BoxModelConstraints(BoxModelInstanceTest):
    def setattrWrapper(self, attr, value):
        def c():
            return setattr(self.instance, attr, value)
        return c
    
    def test_left(self):
        self.assertRaises(ValueError, self.setattrWrapper("Left", -1))

    def test_right(self):
        self.assertRaises(ValueError, self.setattrWrapper("Right", -1))

    def test_top(self):
        self.assertRaises(ValueError, self.setattrWrapper("Top", -1))

    def test_bottom(self):
        self.assertRaises(ValueError, self.setattrWrapper("Bottom", -1))

class PropertyEq(BoxModelTest):
    def test_BaseBox(self):
        a, b = Box.BaseBox(), Box.BaseBox()
        self.assertEqual(a, b)
        a.Left = 10
        b.Left = 10
        self.assertEqual(a, b)
        a.Left = 20
        self.assertNotEqual(a, b)

    def test_Border(self):
        a, b = Border.Border(), Border.Border()
        self.assertEqual(a, b)
        a.Width = 10
        b.Width = 10
        self.assertEqual(a, b)
        a.Width = 20
        self.assertNotEqual(a, b)
        b.Left.Width = 20
        b.Right.Width = 20
        a.Top.Width = 10
        a.Bottom.Width = 10
        self.assertEqual(a, b)

    def test_BackgroundColour(self):
        a, b = Fill.Colour(), Fill.Colour()
        self.assertEqual(a, b)
        a.Colour = Fill.Colour(0.1, 0.2, 0.3, 0.4)
        b.Colour = Fill.Colour(0.1, 0.2, 0.3, 0.4)
        self.assertEqual(a, b)
        a.Colour = Fill.Colour(0.1, 0.1, 0.1, 1.0)
        self.assertNotEqual(a, b)

class TypeConstraints(unittest.TestCase):
    def test_Border(self):
        self.assertRaises(TypeError, setattr, Border.Border(), "Fill", 10)
        self.assertRaises(TypeError, setattr, Border.Border().Left, "Fill", 10)
