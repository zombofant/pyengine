import BoxModel
import unittest
from random import randint

class BoxModelInstanceTest(unittest.TestCase):
    def setUp(self):
        self.instance = BoxModel.BaseBox(0)

    def tearDown(self):
        del self.instance

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
