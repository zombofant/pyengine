from __future__ import unicode_literals, print_function, division
from our_future import *

from random import randint
import unittest
import copy

import Rect
import Box
import Constants

class BoxTest(unittest.TestCase):
    def assertLRTB(self, inst, l, r, t, b):
        self.assertEqual(inst.Left, l)
        self.assertEqual(inst.Right, r)
        self.assertEqual(inst.Top, t)
        self.assertEqual(inst.Bottom, b)


class BaseBoxInitAssignCopy(BoxTest):
    def test_init_default(self):
        box = Box.BaseBox()
        self.assertLRTB(box, 0, 0, 0, 0)

    def test_init_one_arg(self):
        box = Box.BaseBox(10)
        self.assertLRTB(box, 10, 10, 10, 10)
        v = randint(0, 1000)
        box = Box.BaseBox(v)
        self.assertLRTB(box, v, v, v, v)

    def test_init_all(self):
        box = Box.BaseBox(1, 2, 3, 4)
        self.assertLRTB(box, 1, 3, 2, 4)

    def test_init_faulty(self):
        self.assertRaises(ValueError, Box.BaseBox, 1, 2, 3)
        self.assertRaises(ValueError, Box.BaseBox, 1, 2, 3, 4, 5)

    def test_assign(self):
        box = Box.BaseBox()
        box2 = Box.BaseBox(1, 2, 3, 4)
        box.assign(box2)
        self.assertLRTB(box, 1, 3, 2, 4)

    def test_copy(self):
        box = Box.BaseBox(*[randint(0, 1000) for i in range(4)])
        box2 = copy.copy(box)
        box3 = copy.deepcopy(box)
        self.assertEqual(box, box2)
        self.assertIsNot(box, box2)
        self.assertEqual(box, box3)
        self.assertIsNot(box, box3)
        self.assertIsNot(box2, box3)

    def test_non_negative(self):
        self.assertRaises(ValueError, Box.BaseBox, -1)
        box = Box.BaseBox()
        self.assertRaises(ValueError, setattr, box, "Left", -1)

class BaseBoxProperties(BoxTest):
    def test_horizontal(self):
        box = Box.BaseBox(1, 0, 1, 0)
        self.assertEqual(box.Horizontal, 2)
        self.assertEqual(box.Vertical, 0)

    def test_vertical(self):
        box = Box.BaseBox(0, 1, 0, 1)
        self.assertEqual(box.Horizontal, 0)
        self.assertEqual(box.Vertical, 2)

class MarginProperties(BoxTest):
    def test_negative(self):
        margin = Box.Margin(-1)
        self.assertLRTB(margin, -1, -1, -1, -1)
        margin = Box.Margin(-1, 2, -3, 4)
        self.assertLRTB(margin, -1, -3, 2, 4)

    def test_deautoify(self):
        auto = Constants.Auto
        objRect, outerRect = Rect.Rect(0, 0, 10, 10), Rect.Rect(0, 0, 100, 100)

        margin = Box.Margin(auto, 0, auto, 0)
        margin.deautoify(objRect, outerRect)
        self.assertLRTB(margin, 45, 45, 0, 0)

        margin = Box.Margin(auto, auto, auto, auto)
        margin.deautoify(objRect, outerRect)
        self.assertLRTB(margin, 45, 45, 45, 45)

        margin = Box.Margin(auto, 0, 0, 0)
        margin.deautoify(objRect, outerRect)
        self.assertLRTB(margin, 90, 0, 0, 0)

        margin = Box.Margin(0, 0, 0, auto)
        margin.deautoify(objRect, outerRect)
        self.assertLRTB(margin, 0, 0, 0, 90)
