# File name: Fill.py
# This file is part of: pyengine
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
# For feedback and questions about pyengine please e-mail one of the
# authors named in the AUTHORS file.
########################################################################
from __future__ import unicode_literals, print_function, division
from our_future import *

__all__ = ["Repeat", "Tile", "Stretch", "Colour", "Transparent",
    "Gradient", "Image"]

import math
import itertools
import iterutils
import copy

import abc

import cairo

import Engine.Resources.Manager as Manager

from Rect import Rect
from Engine.Resources.CairoWrapper import CairoSurface

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
    __metaclass__ = abc.ABCMeta
    
    def __init__(self, repeatX=Stretch, repeatY=Stretch, **kwargs):
        assert self.__class__ is not Fill
        self._repeatX = Stretch
        self._repeatY = Stretch
        super(Fill, self).__init__(**kwargs)
        self.RepeatX = repeatX
        self.RepeatY = repeatY

    # FIXME: use abc.abstractmethod as soon as possible
    def geometry_for_rect(self, rect, facebuffer):
        """
        Adds the geometry created by this filler for the given Rect
        *rect* to the FaceBuffer *facebuffer*.
        """
        raise NotImplementedError("geometry_for_rect not implemented in {0}".format(type(self).__name__))

    def in_cairo(self, ctx, rect):
        """
        Execute all calls neccessary to draw this filling in a cairo
        context *ctx* in *rect*.
        """
        self.set_source(ctx, rect)
        ctx.rectangle(*rect.XYWH)
        ctx.fill()

    def cairo_group_for_rect(self, rect, ctx):
        ctx.push_group()
        self.in_cairo(ctx, rect)
        return ctx.pop_group()

    def set_source(self, ctx, rect):
        raise NotImplementedError("set_source not implemented for {0}".format(type(self).__name__))

    def __ne__(self, other):
        r = self.__eq__(other)
        if r is NotImplemented:
            return r
        else:
            return not r

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

    def __eq__(self, other):
        return other is self.Transparent
        
    def geometry_for_rect(self, rect, facebuffer):
        pass

    def in_cairo(self, ctx, rect):
        pass

    def set_source(self, ctx, rect):
        ctx.set_source_rgba(0., 0., 0., 0.)
    
Transparent = __Transparent()
__Transparent.Transparent = Transparent

class Colour(Fill):
    __hash__ = None
    
    def __init__(self, r=0., g=0., b=0., a=1., **kwargs):
        r, g, b, a = float(r), float(g), float(b), float(a)
        if a < 0 or a > 1:
            raise ValueError("Alpha must be in [0.,1.]. Got {0}".format(a))
        if r < 0:
            raise ValueError("Red must be non-negative. Got {0}".format(r))
        if g < 0:
            raise ValueError("Green must be non-negative. Got {0}".format(g))
        if b < 0:
            raise ValueError("Blue must be non-negative. Got {0}".format(b))
        super(Colour, self).__init__(**kwargs)
        self._r, self._g, self._b, self._a = r, g, b, a

    @classmethod
    def rgba(cls, r, g, b, a=1.):
        if a == 0:
            return Transparent
        return cls(r, g, b, a)

    @classmethod
    def hsva(cls, h, s, v, a=1.):
        if a == 0:
            return Transparent
        raise NotImplementedError("# FIXME: implement hsva->rgba conversion")

    @classmethod
    def hsla(cls, h, s, l, a=1.):
        if a == 0:
            return Transparent
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

    def geometry_for_rect(self, rect, facebuffer):
        x1, x2, y1, y2 = rect.Left, rect.Right, rect.Top, rect.Bottom
        colour = ((self._r, self._g, self._b, self._a),) * 3
        facebuffer.add_face(
            (
                (x1, y1),
                (x1, y2),
                (x2, y1)
            ),
            colour,
            None
        )
        facebuffer.add_face(
            (
                (x1, y2),
                (x2, y2),
                (x2, y1)
            ),
            colour,
            None
        )

    def set_source(self, ctx, rect):
        ctx.set_source_rgba(self._r, self._g, self._b, self._a)


class Gradient(Fill):
    __hash__ = None
    
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

class FakeImage(Fill):
    __hash__ = None
    
    def __init__(self, resdimensions, rect=None, **kwargs):
        super(FakeImage, self).__init__(**kwargs)
        if not hasattr(self, "_resource"):
            self._resource = None
            self._cairo_surface = None
        self._dimensions = resdimensions
        if rect is None:
            self._rect = Rect(0, 0, *self._dimensions)
        else:
            self._rect = rect
        self._calc_uv()

    def _calc_uv(self):
        rw, rh = self._dimensions
        u1, u2, v1, v2 = self._rect.Left / rw, self._rect.Right / rw, self._rect.Top / rh, self._rect.Bottom / rh
        if u1 < 0 or u1 > 1 or u2 < 0 or u2 > 1 or v2 < 0 or v2 > 1 or v1 < 0 or v2 > 1:
            raise ValueError("UV coordinates leave valid bounds ([0.,1.]). Check your image rect.")
        self._uv = u1, v1, u2, v2
        self._quad_uv = (
            (   self._resource, (
                    (u1, v1),
                    (u1, v2),
                    (u2, v1)
                )
            ),
            (
                self._resource, (
                    (u1, v2),
                    (u2, v2),
                    (u2, v1)
                )
            )
        )

    def _create_vertex_tex_coords(self, splits, v1, v2, vw, t1, t2, tw):
        if splits > 0:
            maxstep = int(math.ceil(splits))
            vt = iterutils.interleave(
                ((i/splits*vw + v1, t1) for i in range(0, maxstep)),
                ((i/splits*vw + v1, t2) for i in range(1, maxstep))
            )
            if int(splits) == splits:
                t = t2
            else:
                t = t1 + (splits - int(splits)) * tw
            return itertools.chain(vt, ((v2, t),))
        else:
            return [(v1, t1), (v2, t2)]

    def _create_quads(self, xu_iterable, yv_iterable):
        i = iter(xu_iterable)
        xu_iterable = list(zip(i, i))
        i = iter(yv_iterable)
        yv_iterable = zip(i, i)
        for yv1, yv2 in yv_iterable:
            for xu1, xu2 in xu_iterable:
                yield (xu1, yv1, xu2, yv2)
        

    def _add_geometry_iterable(self, xuyv_iterable, facebuffer):
        iterator = iter(xuyv_iterable)
        for (x1, u1), (y1, v1), (x2, u2), (y2, v2) in iterator:
            vbottomleft = (x1, y2)
            vtopright = (x2, y1)
            tbottomleft = (u1, v2)
            ttopright = (u2, v1)
            facebuffer.add_faces(
                self._resource,
                (
                    (
                        (x1, y1),
                        vbottomleft,
                        vtopright
                    ),
                    (
                        vtopright,
                        vbottomleft,
                        (x2, y2)
                    )
                ),
                None,
                (
                    (
                        (u1, v1),
                        tbottomleft,
                        ttopright,
                    ),
                    (
                        ttopright,
                        tbottomleft,
                        (u2, v2)
                    )
                )
            )
    
    def geometry_for_rect(self, rect, facebuffer):
        splitsX = 0 if self._repeatX is Stretch else (rect.Width / self._rect.Width)
        splitsY = 0 if self._repeatY is Stretch else (rect.Height / self._rect.Height)
        if not (splitsX or splitsY):
            x1, x2, y1, y2 = rect.Left, rect.Right, rect.Top, rect.Bottom
            facebuffer.add_face(
                (
                    (x1, y1),
                    (x1, y2),
                    (x2, y1)
                ),
                None,
                self._quad_uv[0]
            )
            facebuffer.add_face(
                (
                    (x1, y2),
                    (x2, y2),
                    (x2, y1)
                ),
                None,
                self._quad_uv[1]
            )
        else:
            u1, v1, u2, v2 = self._uv
            uw, vw = u2 - u1, v2 - v1
            xu = self._create_vertex_tex_coords(splitsX,
                float(rect.Left), float(rect.Right),
                float(rect.Width),
                float(u1), float(u2),
                float(uw)
            )
            yv = self._create_vertex_tex_coords(splitsY,
                float(rect.Top), float(rect.Bottom),
                float(rect.Height),
                float(v1), float(v2),
                float(vw)
            )
            self._add_geometry_iterable(self._create_quads(xu, yv), facebuffer)

    def in_cairo(self, ctx, rect):
        ctx.set_source_surface(self._cairo_surface)
        ctx.rectangle(rect.Left, rect.Top, rect.Right, rect.Bottom)
        ctx.fill()

class Image(FakeImage):
    def __init__(self, resource, rect=None, **kwargs):
        if isinstance(resource, unicode):
            self._resource = Manager.ResourceManager().require(resource, CairoSurface)
        else:
            self._resource = resource
        self._cairo_surface = self._resource.surface
        super(Image, self).__init__(self._resource.Dimensions, rect, **kwargs)

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

def is_plain_fill(fill):
    return isinstance(fill, (Colour, __Transparent))
