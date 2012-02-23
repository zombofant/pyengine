import Rect
import unittest
from random import randint

class RectInstanceTest(unittest.TestCase):
    def setUp(self):
        self.instance = Rect.Rect()

    def tearDown(self):
        del self.instance

class RectInit(unittest.TestCase):
    def checkValues(self, x, y, w, h):
        self.assertEqual(self.instance.X, x)
        self.assertEqual(self.instance.Y, y)
        self.assertEqual(self.instance.Width, w)
        self.assertEqual(self.instance.Height, h)
    
    def test_init0(self):
        self.instance = Rect.Rect()
        self.checkValues(0, 0, 0, 0)

    def test_init2(self):
        x = randint(0, 100)
        y = randint(0, 100) + x
        self.instance = Rect.Rect(x, y)
        self.checkValues(x, y, 0, 0)

    def test_init4(self):
        # guarantee that all values are random and non-equal
        x = randint(1, 100)
        y = randint(1, 100) + x
        w = randint(1, 100) + y
        h = randint(1, 100) + w
        self.instance = Rect.Rect(x, y, w, h)
        self.checkValues(x, y, w, h)

    def tearDown(self):
        del self.instance

class RectProperties(RectInstanceTest):
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

class RectConstraints(RectInstanceTest):
    def setattrWrapper(self, attr, value):
        def c():
            return setattr(self.instance, attr, value)
        return c

    def test_width(self):
        self.assertRaises(ValueError, self.setattrWrapper("Width", -1))

    def test_height(self):
        self.assertRaises(ValueError, self.setattrWrapper("Height", -1))
