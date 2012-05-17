# File name: Border.py
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

import iterutils
import copy

from Fill import Fill, Colour, Transparent, isPlainFill
from Box import BaseBox
from Rect import Rect, NotARect

class BorderComponent(object):
    """
    Base class for border components. This only implements the
    abstracted assignment and forms a base class to check against in
    more sophisticated assign procedures.
    """
    
    def assign(self, other):
        """
        Assign the width and fill of the *other*
        :class:`BorderComponent` to this instance.

        If *other* is not a :class:`BorderComponent`, a :exc:`TypeError`
        is raised.
        """
        if not isinstance(other, BorderComponent):
            raise TypeError("Can only assign BorderComponents to BorderComponents. Got {0} {1}".format(type(other), other))
        self.Width = other.Width
        self.Fill = other.Fill

class BorderEdge(BorderComponent):
    """
    Represents the edge of a box border. It can have both a width and
    a filling assigned. Check for equality and unequality is implemented
    for :class:`BorderEdge` instances, but as they are mutable, they
    cannot be hashed.
    """
    
    __hash__ = None
    
    def __init__(self, width=0, fill=Transparent, **kwargs):
        self._width = None
        self._fill = None
        super(BorderEdge, self).__init__(**kwargs)
        self.Width = width
        self.Fill = fill

    def __deepcopy__(self, memo):
        return BorderEdge(self._width, copy.deepcopy(self._fill, memo))

    @property
    def Width(self):
        """
        Control the width of a border edge.
        """
        return self._width

    @Width.setter
    def Width(self, value):
        if value == self._width:
            return
        if value < 0:
            raise ValueError("Border width must be non-negative")
        self._width = value
    
    @property
    def Fill(self):
        """
        Control the filling of a border edge.

        Border edges can only be filled with plain colour or can be set
        to :data:`Transparent`. An attempt to assign any other filler to
        a border will be punished with a :exc:`TypeError`
        """
        return self._fill

    @Fill.setter
    def Fill(self, value):
        if self._fill == value:
            return
        if not isPlainFill(value):
            raise TypeError("Border needs plain fill instances as fillers (e.g. colour or transparent). Got {0} {1}".format(type(value), value))
        self._fill = value

    def __eq__(self, other):
        if not isinstance(other, BorderEdge):
            return NotImplemented
        return (self._width == other._width and
                self._fill == other._fill)

    def __ne__(self, other):
        r = self.__eq__(other)
        if r is NotImplemented:
            return r
        else:
            return not r

    def __repr__(self):
        return "BorderEdge(width={0!r}, fill={1!r})".format(self._width, self._fill)

class Border(BorderComponent):
    """
    Represents a complete box border with four edges and radii for each
    corner to support rounded corners.

    :class:`Border` implements check for equality and inequality, but
    no hashing.
    """
    
    __hash__ = None
    
    def __init__(self, width=0, fill=None, **kwargs):
        self._edges = [BorderEdge() for i in range(4)]
        self._corners = [0] * 4
        super(Border, self).__init__(**kwargs)
        self.Width = width
        if fill is not None:
            self.Fill = fill
    
    def assign(self, other):
        """
        Assign the values of another :class:`BorderComponent` to this
        instance.

        If *other* is a :class:`Border`, each edge is transferred
        separately using its own assign method. The radii for each
        corner is also copied.

        If *other* is a :class:`BorderComponent`, *other* is assigned to
        all edges and the rounded corner radius is reset to `0`.

        If none of the above applies, a :exc:`TypeError` is raised.
        """
        if isinstance(other, Border):
            for edgeA, edgeB in zip(self._edges, other._edges):
                edgeA.assign(edgeB)
            self._corners = list(other._corners)
        elif isinstance(other, BorderComponent): 
            for edgeA in self._edges:
                edgaA.assign(other)
            self._corners = [0] * 4
        else:    
            raise TypeError("Can only assign BorderComponents to Border")

    def __deepcopy__(self, memo):
        new = Border()
        for i, edge in enumerate(self._edges):
            new._edges[i] = copy.deepcopy(edge, memo)
        new._corners = list(self._corners)
        return new

    def setWidth(self, value):
        """
        Set the width of all edges to the given *value*.
        """
        for edge in self._edges:
            edge.Width = value

    def setFill(self, value):
        """
        Set the filler for all edges to the given *value*. The same
        restrictions as for :attr:`BorderEdge.Fill` apply.
        """
        for edge in self._edges:
            edge.Fill = value

    def setRadius(self, value):
        """
        Set the rounding radius of each edge to *value*.
        """
        if value < 0:
            raise ValueError("Border radius must be a non-zero integer number.")
        self._corners = [int(value)] * 4

    def getAllRadii(self):
        return tuple(self._corners)

    @property
    def Width(self):
        raise NotImplementedError("Cannot read global border width.")

    @Width.setter
    def Width(self, value):
        self.setWidth(value)
    
    @property
    def Fill(self):
        raise NotImplementedError("Cannot read global border colour.")

    @Fill.setter
    def Fill(self, value):
        self.setFill(value)

    @property
    def Left(self):
        return self._edges[0]

    @Left.setter
    def Left(self, value):
        self._edges[0].assign(value)

    @property
    def Top(self):
        return self._edges[1]

    @Top.setter
    def Top(self, value):
        self._edges[1].assign(value)

    @property
    def Right(self):
        return self._edges[2]

    @Right.setter
    def Right(self, value):
        self._edges[2].assign(value)

    @property
    def Bottom(self):
        return self._edges[3]

    @Bottom.setter
    def Bottom(self, value):
        self._edges[3].assign(value)
    
    @property
    def TopLeftRadius(self):
        return self._corners[0]

    @TopLeftRadius.setter
    def TopLeftRadius(self, value):
        self._corners[0] = value

    @property
    def TopRightRadius(self):
        return self._corners[1]

    @TopRightRadius.setter
    def TopRightRadius(self, value):
        self._corners[1] = value

    @property
    def BottomRightRadius(self):
        return self._corners[2]

    @BottomRightRadius.setter
    def BottomRightRadius(self, value):
        self._corners[2] = value

    @property
    def BottomLeftRadius(self):
        return self._corners[3]

    @BottomLeftRadius.setter
    def BottomLeftRadius(self, value):
        self._corners[3] = value

    def __eq__(self, other):
        if not isinstance(other, Border):
            return NotImplemented
        for edgeA, edgeB in zip(self._edges, other._edges):
            if not edgeA == edgeB:
                return False
        return self._corners == other._corners

    def __ne__(self, other):
        r = self.__eq__(other)
        if r is NotImplemented:
            return r
        else:
            return not r

    def __repr__(self):
        return """<Border
    Left={0!r},
    Top={1!r},
    Right={2!r},
    Bottom={3!r},
    TopLeft={4!r},
    TopRight={5!r},
    BottomRight={6!r},
    BottomLeft={7!r}>""".format(self.Left, self.Top, self.Right, self.Bottom,
            self.TopLeft, self.TopRight, self.BottomRight, self.BottomLeft)

    def getBox(self):
        return BaseBox(self.Left.Width, self.Top.Width,
            self.Right.Width, self.Bottom.Width)

    def geometryForRect(self, rect, faceBuffer):
        """
        Adds geometry for this border as inner border in *rect* in
        *faceBuffer*.

        Returns the BaseBox representing this border.
        """
        box = self.getBox()
        rectsAndEdges = zip(
            rect.cut(box),
            iterutils.interleave((edge.Fill for edge in self._edges), self._corners)
        )
        prevFill = None
        for rect, fill in rectsAndEdges:
            if rect is NotARect:
                continue
            if fill is None:
                fill = prevFill
            else:
                prevFill = fill
            fill.geometryForRect(rect, faceBuffer)
        return box

    def inCairo(self, rect, cr):
        """
        Execute all instructions neccessary to draw this border as the
        inner border in *rect* on cairo context *cr*.
        """
        raise NotImplementedError("this should in fact be done by Style currently")

    def cairoGroupForRect(self, rect, cr):
        """
        Create and return a cairo group for this border as inner border
        in *rect*, using *cr* as cairo context.
        """
        cr.push_group()
        self.inCairo(rect, cr)
        return cr.pop_group()
