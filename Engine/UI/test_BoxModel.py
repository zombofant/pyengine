import BoxModel
import unittest
from random import randint

class BoxModelInstanceTest(unittest.TestCase):
    def setUp(self):
        self.instance = BoxModel.BaseBox()

    def tearDown(self):
        del self.instance

class BoxModelInit(unittest.TestCase):
    def checkValues(self, l, t, r, b):
        self.assertEqual(self.instance.Left, l)
        self.assertEqual(self.instance.Top, t)
        self.assertEqual(self.instance.Right, r)
        self.assertEqual(self.instance.Bottom, b)
    
    def test_init0(self):
        self.instance = BoxModel.BaseBox()
        self.checkValues(0, 0, 0, 0)

    def test_init1(self):
        r = randint(0, 100)
        self.instance = BoxModel.BaseBox(r)
        self.checkValues(r, r, r, r)

    def test_init4(self):
        # guarantee that all values are random and non-equal
        l = randint(1, 100)
        r = randint(1, 100) + l
        t = randint(1, 100) + r
        b = randint(1, 100) + t
        self.instance = BoxModel.BaseBox(l, t, r, b)
        self.checkValues(l, t, r, b)

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
