import BoxModel
import unittest
import random

class BoxModelTestCase(unittest.TestCase):
    def test_init0(self):
        self.assertRaises(ValueError, BoxModel.BaseBox)

    def test_init1(self):
        r = 1
        instance = BoxModel.BaseBox(r)
        self.assertEqual(instance.Left, r)
        self.assertEqual(instance.Top, r)
        self.assertEqual(instance.Right, r)
        self.assertEqual(instance.Bottom, r)

    def test_init4(self):
        l, r, t, b = 1, 2, 3, 4
        instance = BoxModel.BaseBox(l, t, r, b)
        self.assertEqual(instance.Left, l)
        self.assertEqual(instance.Top, t)
        self.assertEqual(instance.Right, r)
        self.assertEqual(instance.Bottom, b)
