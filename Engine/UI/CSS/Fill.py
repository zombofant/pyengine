# File name: Fill.py
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

__all__ = ["Repeat", "Tile", "Stretch", "Colour", "Transparent",
    "Gradient", "Image"]

import Engine.Resources.Manager as Manager

class RepeatMode(object):
    ValidModes = []

    def __init__(self, name):
        self._name = name

    @classmethod
    def check(cls, mode):
        if not mode in cls.ValidModes:
            raise ValueError("{0} is not a valid repeat mode.".format(mode))
        return mode

    def __repr__(self):
        return self._name

    def __unicode__(self):
        return self._name

Repeat = RepeatMode("Repeat")
Stretch = RepeatMode("Stretch")
Tile = Repeat

RepeatMode.ValidModes += [Repeat, Stretch]

class Fill(object):
    def __init__(self, repeatX=Stretch, repeatY=Stretch, **kwargs):
        assert self.__class__ is not Fill
        self._repeatX = Stretch
        self._repeatY = Stretch
        super(Fill, self).__init__(**kwargs)
        self.RepeatX = repeatX
        self.RepeatY = repeatY
    
    def geometryForRect(self, rect):
        raise NotImplementedError()

    @property
    def RepeatX(self):
        return self._repeatX

    @RepeatX.setter
    def RepeatX(self, value):
        self._repeatX = RepeatMode.check(value)

    @property
    def RepeatY(self):
        return self._repeatY

    @RepeatY.setter
    def RepeatY(self, value):
        self._repeatY = RepeatMode.check(value)
    

class __Transparent(Fill):
    def geometryForRect(self, rect):
        return ()

    # for fake constructor calls. We just hand out the instance as we
    # do not need to instanciate multiple Transparent fills.
    def __call__(self):
        return self

    def __deepcopy__(self, memo):
        return self

    def __copy__(self):
        return self

    def __repr__(self):
        return "Transparent"
Transparent = __Transparent()

class Colour(Fill):
    def __init__(self, r=0., g=0., b=0., a=1., **kwargs):
        super(Colour, self).__init__(**kwargs)
        self._r, self._g, self._b, self._a = float(r), float(g), float(b), float(a)

    @classmethod
    def rgba(cls, r, g, b, a=1.):
        return cls(r, g, b, a)

    @classmethod
    def hsva(cls, h, s, v, a=1.):
        raise NotImplementedError("# FIXME: implement hsva->rgba conversion")

    @classmethod
    def hsla(cls, h, s, l, a=1.):
        raise NotImplementedError("# FIXME: implement hsla->rgba conversion")
    
    def __eq__(self, other):
        if not isinstance(other, Fill):
            return NotImplemented
        if isinstance(other, Colour):
            return (self._r == other._r and
                    self._g == other._g and
                    self._b == other._b and
                    self._a == other._a)
        else:
            return False

    def __deepcopy__(self, memo):
        return Colour(self._r, self._g, self._b, self._a,
            repeatX=self.RepeatX,
            repeatY=self.RepeatY)

    def __unicode__(self):
        return "rgba({0}, {1}, {2}, {3})".format(self._r, self._g, self._b, self._a)

    def __repr__(self):
        return "Colour({0}, {1}, {2}, {3})".format(self._r, self._g, self._b, self._a)

class Gradient(Fill):
    class Step(object):
        def __init__(self, position, colour):
            if not isinstance(colour, Colour):
                raise TypeError("Gradient.Step needs a colour as second argument")
            position = float(position)
            if not (position >= 0 and position <= 1):
                raise ValueError("Gradient.Step position must be within 0 and 1")
            self._position = position
            self._colour = colour
    
    def __init__(self, direction, *steps):
        self._direction = {
            "horiz": (1, 0),
            "vert": (0, 1)
        }[direction]
        for step in steps:
            if not isinstance(step, Gradient.Step):
                raise TypeError("Gradient can only use Gradient.Step instances")
        self._steps = tuple(sorted(((step._position, i, step._colour) for i, step in enumerate(steps)), key=lambda x: (x[0],x[1])))

    def __eq__(self, other):
        if not isinstance(other, Fill):
            return NotImplemented
        if isinstance(other, Gradient):
            return (self._steps == other._steps)
        else:
            return False

    @Fill.RepeatX.setter
    def RepeatX(self, value):
        if value is not Stretch:
            raise ValueError("Gradient can only be stretched, not tiled.")
        self._repeatX = value

    @Fill.RepeatY.setter
    def RepeatY(self, value):
        if value is not Stretch:
            raise ValueError("Gradient can only be stretched, not tiled.")
        self._repeatY = value

    def __deepcopy__(self, memo):
        return Gradient(
            "horiz" if self._direction == (1, 0) else "vert",
            *(Step(position, colour) for position, i, colour in self._steps),
            repeatX=self.RepeatX,
            repeatY=self.RepeatY
        )

class Image(Fill):
    def __init__(self, resource, rect=None, **kwargs):
        super(Image, self).__init__(**kwargs)
        self._resource = Manager.ResourceManager().require(resource)
        if rect is None:
            self._rect = Rect(0, 0, *self._resource.Dimensions)
        else:
            self._rect = rect

    def __eq__(self, other):
        if not isinstance(other, Fill):
            return NotImplemented
        if isinstance(other, Image):
            return (self._resource is other._resource and
                    self._rect == other._rect and
                    self._repeatX == other._repeatX and
                    self._repeatY == other._repeatY)
        else:
            return False

    def __deepcopy__(self, memo):
        return Image(
            self._resource,
            copy.deepcopy(self._rect, memo),
            repeatX=self.RepeatX,
            repeatY=self.RepeatY
        )
