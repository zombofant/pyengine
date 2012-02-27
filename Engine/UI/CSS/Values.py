# File name: Values.py
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

__all__ = ["Stretch", "Repeat", "Rect", "Repeats", "RGBA", "HSLA",
    "HSVA", "GradientStep", "Gradient", "Image"]

class Rect(object):
    def __init__(self, x, y, w, h):
        if w <= 0 or h <= 0:
            raise ValueError("Rect must have positive width and height")
        self._rect = (x, y, w, h)


class RepeatMode(object):
    pass


Stretch = RepeatMode()
Repeat = RepeatMode()


__repeatModes = [Stretch, Repeat]


class Repeats(object):
    def __init__(self, xrepeat, yrepeat):
        if not xrepeat in __repeatModes:
            raise ValueError("Repeat needs a RepeatMode as first argument")
        if not yrepeat in __repeatModes:
            raise ValueError("Repeat needs a RepeatMode as second argument")
        self._x = xrepeat
        self._y = yrepeat

class Colour(object):
    def __eq__(self, other):
        if not isinstance(other, Colour):
            return NotImplemented
        return (self._r == other._r and
                self._g == other._g and
                self._b == other._b and
                self._a == other._a)


class RGBA(Colour):
    def __init__(self, r, g, b, a=1.0):
        self._r = r
        self._g = g
        self._b = b
        self._a = a


class HSLA(Colour):
    def __init__(self, h, s, l, a=1.0):
        raise NotImplementedError("HSLA not implemented yet.")


class HSVA(Colour):
    def __init__(self, h, s, v, a=1.0):
        raise NotImplementedError("HSLA not implemented yet.")


class GradientStep(object):
    def __init__(self, position, colour):
        if not isinstance(colour, Colour):
            raise TypeError("GradientStep needs a colour as second argument")
        self._position = position
        self._colour = colour


class Gradient(object):
    def __init__(self, direction, *steps):
        if not direction in ["horiz", "vert"]:
            raise ValueError("Gradient direction must be horiz or vert.")
        self._direction = {
            "horiz": 0,
            "vert": 1
        }[direction]
        self._steps = steps
        for step in steps:
            if not isinstance(step, GradientStep):
                raise TypeError("Gradient can only use GradientSteps")


class Image(object):
    def __init__(self, vfspath, rect=None, repeat=None):
        if rect is not None and not isinstance(rect, Rect):
            raise TypeError("ImageRect needs a Rect as second argument")
        self._repeatX = Stretch
        self._repeatY = Stretch
        if repeat is not None:
            if not isinstance(repeat, Repeats):
                raise TypeError("ImageRect needs a Repeats instance as repeat= argument")
            self._repeatX = repeat._x or self._repeatX
            self._repeatY = repeat._y or self._repeatY
        self._vfspath = vfspath
        self._rect = rect
