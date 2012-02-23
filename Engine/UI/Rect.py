from __future__ import unicode_literals, print_function, division
from our_future import *

class _NotARect(object):
    def __eq__(self, other):
        if isinstance(other, _NotARect):
            return True
        elif isinstance(other, Rect):
            return False
        else:
            return NotImplemented

    def __contains__(self, other):
        if isinstance(other, (_NotARect, Rect)):
            return False
        else:
            return NotImplemented

    def __and__(self, other):
        if isinstance(other, (_NotARect, Rect)):
            return self
        else:
            return NotImplemented

    def __repr__(self):
        return "NotARect"
NotARect = _NotARect()

class Rect(object):
    @staticmethod
    def _checkDimension(value):
        if value < 0:
            raise ValueError("Rect dimensions must be non negative.")
        return value
        
    def __init__(self, *args):
        """
        Takes zero, two or four integer (castable) arguments.

        If none are passed, the Rect gets initialized with all
        components set to zero.

        If two are passed, these are interpreted as X and Y coordinates.
        Width and Height of the rect are still set to zero.

        If four are passed, the first two are X and Y, while the other
        two are interpreted as Right and Bottom.
        """
        self._x, self._y = 0, 0
        self._right, self._bottom = 0, 0
        self._width, self._height = 0, 0
        if len(args) == 2:
            self._x, self._y = (int(x) for x in args)
            self._right, self._bottom = 0, 0
            self._width, self._height = 0, 0
        elif len(args) == 4:
            # Lets do the checks inside the property setters
            self.X, self.Y, self.Right, self.Bottom = args
        elif len(args) != 0:
            raise ValueError("Rect takes 0, 2 or 4 int arguments.")

    @property
    def X(self):
        return self._x

    @X.setter
    def X(self, value):
        value = int(value)
        if self._x == value:
            return
        self._x = value
        self._right = self._x + self._width

    @property
    def Y(self):
        return self._y

    @Y.setter
    def Y(self, value):
        value = int(value)
        if self._y == value:
            return
        self._y = value
        self._bottom = self._y + self._height

    @property
    def Left(self):
        return self._x

    @Left.setter
    def Left(self, value):
        value = int(value)
        if self._x == value:
            return
        if value > self._right:
            raise ValueError("Left must be smaller than or equal to right.")
        self._x = value
        self._width = self._right - self._x

    @property
    def Top(self):
        return self._y

    @Top.setter
    def Top(self, value):
        value = int(value)
        if self._y == value:
            return
        if value > self._bottom:
            raise ValueError("Top must be smaller than or equal to bottom.")
        self._y = value
        self._height = self._bottom - self._y

    @property
    def Width(self):
        return self._width

    @Width.setter
    def Width(self, value):
        value = int(value)
        if self._width == value:
            return
        self._width = self._checkDimension(value)
        self._right = self._x + self._width

    @property
    def Height(self):
        return self._height

    @Height.setter
    def Height(self, value):
        value = int(value)
        if self._height == value:
            return
        self._height = self._checkDimension(value)
        self._bottom = self._y + self._height

    @property
    def Right(self):
        return self._right

    @Right.setter
    def Right(self, value):
        value = int(value)
        if self._right == value:
            return
        if value < self._x:
            raise ValueError("Right must be larger than or equal to left.")
        self._right = value
        self._width = self._right - self._x

    @property
    def Bottom(self):
        return self._bottom

    @Bottom.setter
    def Bottom(self, value):
        value = int(value)
        if self._bottom == value:
            return
        if value < self._y:
            raise ValueError("Bottom must be larger than or equal to top.")
        self._bottom = value
        self._height = self._bottom - self._y

    def transpose(self, byX, byY):
        self.X = self._x + byX
        self.Y = sefl._y + byY

    def __eq__(self, other):
        if isinstance(other, _NotARect):
            return False
        elif isinstance(other, Rect):
            return self._x == other._x and self._y == other._y and self._width == other._width and self._height == other._height
        else:
            return NotImplemented

    def __and__(self, other):
        if isinstance(other, _NotARect):
            return NotARect
        if not isinstance(other, Rect):
            return NotImplemented
        x, y = max(self._x, other._x), max(self._y, other._y)
        r, b = min(self._right, other._right), min(self._bottom, other._bottom)
        if r < x or b < y:
            return NotARect
        return Rect(x, y, r, b)

    def __contains__(self, other):
        if isinstance(other, _NotARect):
            return True
        if not isinstance(other, Rect):
            return NotImplemented
        return self._x <= other._x and self._y <= other._y and self._right >= other._right and self._bottom >= other._bottom

    def __repr__(self):
        return "Rect({0}, {1}, {2}, {3})".format(self._x, self._y, self._right, self._bottom)
        
