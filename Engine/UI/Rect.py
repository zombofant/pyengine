from __future__ import unicode_literals, print_function, division
from our_future import *

class Rect(object):
    @staticmethod
    def _checkDimension(value):
        value = int(value)
        if value < 0:
            raise ValueError("Rect dimensions must be non negative.")
        return value
        
    def __init__(self, *args):
        if len(args) == 0:
            self._x, self._y, self._width, self._height = 0, 0, 0, 0
        elif len(args) == 2:
            self._x, self._y = (int(x) for x in args)
            self._width = 0
            self._height = 0
        elif len(args) == 4:
            self._x, self._y = (int(x) for x in args[:2])
            self._width, self._height = (self._checkDimension(x) for x in args[2:])
        else:
            raise ValueError("Rect requires 0, 2 or 4 integer arguments")

    @property
    def X(self):
        return self._x

    @X.setter
    def X(self, value):
        self._x = int(value)

    @property
    def Y(self):
        return self._y

    @Y.setter
    def Y(self, value):
        self._y = int(value)

    @property
    def Width(self):
        return self._width

    @Width.setter
    def Width(self, value):
        self._width = self._checkDimension(value)

    @property
    def Height(self):
        return self._height

    @Height.setter
    def Height(self, value):
        self._heigth = self._checkDimension(value)
    
