import Rect
import unittest
from random import randint

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
    def test_and(self):
        a = Rect.Rect(10, 10, 20, 20)
        b = Rect.Rect(-5, -5, 30, 30)
        self.assertEqual(a & b, a)

        b = Rect.Rect(15, 15, 30, 30)
        self.assertEqual(a & b, Rect.Rect(15, 15, 20, 20))

    def test_and_invalid(self):
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
        self.assertTrue(b in a)
        self.assertFalse(a in b)

        x, y = randint(1, 100), randint(1, 100)
        r, b = x + randint(1, 100), y + randint(1, 100)
        a = Rect.Rect(x, y, r, b)
        self.assertTrue(a in a)
