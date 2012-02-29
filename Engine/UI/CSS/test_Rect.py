# File name: test_Rect.py
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

from random import randint
import unittest
import copy

import Rect
import Box

class RectTest(unittest.TestCase):
    def checkValuesXYWH(self, x, y, w, h):
        self.assertEqual(self.instance.X, x)
        self.assertEqual(self.instance.Y, y)
        self.assertEqual(self.instance.Width, w)
        self.assertEqual(self.instance.Height, h)
        
    def checkValuesXYRB(self, x, y, r, b):
        self.assertEqual(self.instance.X, x)
        self.assertEqual(self.instance.Y, y)
        self.assertEqual(self.instance.Right, r)
        self.assertEqual(self.instance.Bottom, b)

class RectInstanceTest(RectTest):
    def setUp(self):
        self.instance = Rect.Rect()

    def tearDown(self):
        del self.instance

class RectInit(RectTest):
    def test_init0(self):
        self.instance = Rect.Rect()
        self.checkValuesXYRB(0, 0, 0, 0)

    def test_init2(self):
        x = randint(0, 100)
        y = randint(0, 100) + x
        self.instance = Rect.Rect(x, y)
        self.checkValuesXYRB(x, y, 0, 0)

    def test_init4(self):
        # guarantee that all values are random and non-equal
        x = randint(1, 100)
        y = randint(1, 100) + x
        r = randint(1, 100) + x
        b = randint(1, 100) + y
        self.instance = Rect.Rect(x, y, r, b)
        self.checkValuesXYRB(x, y, r, b)

    def tearDown(self):
        del self.instance

class RectProperties(RectInstanceTest):
    def test_x(self):
        v = randint(1, 100)
        self.instance.X = v
        self.assertEqual(self.instance.X, v)
        self.assertEqual(self.instance.Right, v)

    def test_y(self):
        v = randint(1, 100)
        self.instance.Y = v
        self.assertEqual(self.instance.Y, v)
        self.assertEqual(self.instance.Bottom, v)

    def test_rw(self):
        w, x = randint(1, 100), randint(1, 100)
        r = x + randint(1, 100)
        self.instance.Width = w
        self.assertEqual(self.instance.Width, w)
        self.assertEqual(self.instance.Right, w)

        self.instance.X = x
        self.assertEqual(self.instance.Right, x + w)
        self.assertEqual(self.instance.Width, w)

        self.instance.Right = r
        self.assertEqual(self.instance.Right, r)
        self.assertEqual(self.instance.Width, r - x)

    def test_bh(self):
        h, y = randint(1, 100), randint(1, 100)
        b = y + randint(1, 100)
        self.instance.Height = h
        self.assertEqual(self.instance.Height, h)
        self.assertEqual(self.instance.Bottom, h)

        self.instance.Y = y
        self.assertEqual(self.instance.Bottom, y + h)
        self.assertEqual(self.instance.Height, h)

        self.instance.Bottom = b
        self.assertEqual(self.instance.Bottom, b)
        self.assertEqual(self.instance.Height, b - y)
        
class RectConstraints(RectInstanceTest):
    def setattrWrapper(self, attr, value):
        def c():
            return setattr(self.instance, attr, value)
        return c

    def test_width(self):
        self.assertRaises(ValueError, self.setattrWrapper("Width", -1))

    def test_height(self):
        self.assertRaises(ValueError, self.setattrWrapper("Height", -1))

    def test_right(self):
        x = randint(10, 100)
        self.instance.X = x
        self.assertRaises(ValueError, self.setattrWrapper("Right", x-1))

    def test_bottom(self):
        y = randint(10, 100)
        self.instance.Y = y
        self.assertRaises(ValueError, self.setattrWrapper("Bottom", y-1))

class RectOperators(RectTest):
    def test_copy(self):
        a = Rect.Rect(0, 0, 10, 10)
        b = copy.copy(a)
        self.assertIsNot(a, b)
        self.assertEqual(a, b)
        
    def test_and(self):
        a = Rect.Rect(10, 10, 20, 20)
        backup = copy.copy(a)
        b = Rect.Rect(-5, -5, 30, 30)
        self.assertEqual(a & b, a)
        self.assertEqual(a, backup)

        b = Rect.Rect(15, 15, 30, 30)
        self.assertEqual(a & b, Rect.Rect(15, 15, 20, 20))
        self.assertEqual(a, backup)

    def test_andInvalid(self):
        a = Rect.Rect(0, 0, 10, 10)
        b = Rect.Rect(10, 10, 20, 20)
        self.assertEqual(a & b, Rect.Rect(10, 10, 10, 10))
        a = Rect.Rect(0, 0, 9, 9)
        self.assertEqual(a & b, Rect.NotARect)

    def test_empty(self):
        self.assertEqual(Rect.Rect() & Rect.NotARect, Rect.NotARect)
        self.assertEqual(Rect.NotARect & Rect.Rect(), Rect.NotARect)

    def test_contains(self):
        a = Rect.Rect(0, 0, 10, 10)
        b = Rect.Rect(1, 1, 9, 9)
        self.assertIn(b, a)
        self.assertNotIn(a, b)

        x, y = randint(1, 100), randint(1, 100)
        r, b = x + randint(1, 100), y + randint(1, 100)
        a = Rect.Rect(x, y, r, b)
        self.assertIn(a, a)

    def test_containsTuple(self):
        a = Rect.Rect(0, 0, 10, 10)
        self.assertIn((5, 5), a)
        self.assertNotIn((13, 13), a)

class RectOr(RectTest):
    def test_onX(self):
        a = Rect.Rect( 0, 10, 10, 20)
        b = Rect.Rect(10, 10, 20, 20)
        backup = copy.copy(a)
        self.assertEqual(a | b, Rect.Rect(0, 10, 20, 20))
        self.assertEqual(a, backup)
        self.assertEqual(b | a, Rect.Rect(0, 10, 20, 20))

    def test_onY(self):
        a = Rect.Rect(10,  0, 20, 10)
        backup = copy.copy(a)
        b = Rect.Rect(10, 10, 20, 20)
        self.assertEqual(a | b, Rect.Rect(10, 0, 20, 20))
        self.assertEqual(a, backup)
        self.assertEqual(b | a, Rect.Rect(10, 0, 20, 20))

    def test_disjunct(self):
        a = Rect.Rect(10,  0, 20,  9)
        b = Rect.Rect(10, 10, 20, 20)
        self.assertIs(a | b, Rect.NotARect)
        self.assertIs(b | a, Rect.NotARect)

        a = Rect.Rect( 0, 10,  9, 20)
        b = Rect.Rect(10, 10, 20, 20)
        self.assertIs(a | b, Rect.NotARect)
        self.assertIs(b | a, Rect.NotARect)

    def test_invalid(self):
        a = Rect.Rect(0, 0, 10, 10)
        b = Rect.Rect(5, 5, 15, 15)
        self.assertIs(a | b, Rect.NotARect)
        self.assertIs(b | a, Rect.NotARect)

class RectBoxes(RectTest):
    def test_shrink(self):
        a = Rect.Rect(5, 5, 15, 15)
        a.shrink(Box.BaseBox(1, 1, 1, 1))
        self.assertEqual(a, Rect.Rect(6, 6, 14, 14))

    def test_expand(self):
        a = Rect.Rect(5, 5, 15, 15)
        a.expand(Box.BaseBox(5, 5, 5, 5))
        self.assertEqual(a, Rect.Rect(0, 0, 20, 20))

    def test_cut(self):
        a = Rect.Rect(0, 0, 20, 20)
        b = Box.BaseBox(5, 5, 5, 5)

        left, topLeft, top, topRight, right, bottomRight, bottom, bottomLeft = a.cut(b)
        self.assertEqual(
            left,
            Rect.Rect(0, 5, 5, 15)
        )
        self.assertEqual(
            topLeft,
            Rect.Rect(0, 0, 5, 5)
        )
        self.assertEqual(
            top,
            Rect.Rect(5, 0, 15, 5)
        )
        self.assertEqual(
            topRight,
            Rect.Rect(15, 0, 20, 5)
        )
        self.assertEqual(
            right,
            Rect.Rect(15, 5, 20, 15)
        )
        self.assertEqual(
            bottomRight,
            Rect.Rect(15, 15, 20, 20)
        )
        self.assertEqual(
            bottom,
            Rect.Rect(5, 15, 15, 20)
        )
        self.assertEqual(
            bottomLeft,
            Rect.Rect(0, 15, 5, 20)
        )
    
    def test_cut2(self):
        a = Rect.Rect(0, 0, 20, 20)
        b = Box.BaseBox(0, 0, 0, 0)

        rects = a.cut(b)

        self.assertEqual(
            rects,
            (Rect.NotARect, Rect.NotARect, Rect.NotARect, Rect.NotARect, Rect.NotARect, Rect.NotARect, Rect.NotARect, Rect.NotARect)
        )
