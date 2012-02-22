import BoxModel
import unittest
from random import randint

class BoxModelInit(unittest.TestCase):
    def test_init0(self):
        self.assertRaises(ValueError, BoxModel.BaseBox)

    def test_init1(self):
        r = randint(0, 100)
        instance = BoxModel.BaseBox(r)
        self.assertEqual(instance.Left, r)
        self.assertEqual(instance.Top, r)
        self.assertEqual(instance.Right, r)
        self.assertEqual(instance.Bottom, r)

    def test_init4(self):
        l, r, t, b = randint(0, 100), randint(0, 100), randint(0, 100), randint(0, 100)
        instance = BoxModel.BaseBox(l, t, r, b)
        self.assertEqual(instance.Left, l)
        self.assertEqual(instance.Top, t)
        self.assertEqual(instance.Right, r)
        self.assertEqual(instance.Bottom, b)

class BoxModelProperties(unittest.TestCase):
    def setUp(self):
        self.instance = BoxModel.BaseBox(0)

    def tearDown(self):
        del self.instance

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
