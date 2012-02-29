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

from Fill import Fill, Colour, Transparent
from Box import BaseBox

class BorderComponent(object):
    def __init__(self, width=0, fill=Transparent, **kwargs):
        super(BorderComponent, self).__init__(**kwargs)
        self.Width = width
        self.Fill = fill or Colour(0., 0., 0., 0.)
    
    def assign(self, other):
        if not isinstance(other, BorderComponent):
            raise TypeError("Can only assign BorderComponents to BorderComponents. Got {0} {1}".format(type(other), other))
        self.Width = other.Width
        self.Fill = other.Fill

class BorderEdge(BorderComponent):
    def __init__(self, width=0, fill=Transparent, **kwargs):
        self._width = None
        self._fill = None
        super(BorderEdge, self).__init__(width, fill, **kwargs)

    @property
    def Width(self):
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
        return self._fill

    @Fill.setter
    def Fill(self, value):
        if self._fill == value:
            return
        if not isinstance(value, Fill):
            raise TypeError("Border needs Fill instances as fillers. Got {0} {1}".format(type(value), value))
        self._fill = value

    def __eq__(self, other):
        if not isinstance(other, BorderEdge):
            return NotImplemented
        return (self._width == other._width and
                self._fill == other._fill)

    def __repr__(self):
        return "BorderEdge(width={0!r}, fill={1!r})".format(self._width, self._fill)

class Border(BorderComponent):
    def __init__(self, width=0, fill=Transparent, **kwargs):
        self._edges = [BorderEdge() for i in range(4)]
        self._corners = [Transparent for i in range(4)]
        super(Border, self).__init__(width, fill, **kwargs)
    
    def assign(self, other):
        if isinstance(other, Border):            
            for edgeA, edgeB in zip(self._edges, other._edges):
                edgeA.Width = edgeB.Width
                edgeA.Fill = edgeB.Fill
            cornerA = list(other._corners)
        elif isinstance(other, BorderComponent): 
            for edgeA in self._edges:
                edgeA.Width = other.Width
                edgeA.Fill = other.Fill
        else:    
            raise TypeError("Can only assign BorderComponents to Border")

    @property
    def Width(self):
        raise NotImplementedError("Cannot read global border width.")

    @Width.setter
    def Width(self, value):
        for edge in self._edges:
            edge.Width = value
    
    @property
    def Fill(self):
        raise NotImplementedError("Cannot read global border colour.")

    @Fill.setter
    def Fill(self, value):
        for edge in self._edges:
            edge.Fill = value

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
    def TopLeft(self):
        return self._corners[0]

    @TopLeft.setter
    def TopLeft(self, value):
        self._corners[0].assign(value)

    @property
    def TopRight(self):
        return self._corners[1]

    @TopRight.setter
    def TopRight(self, value):
        self._corners[1].assign(value)

    @property
    def BottomRight(self):
        return self._corners[2]

    @BottomRight.setter
    def BottomRight(self, value):
        self._corners[2].assign(value)

    @property
    def BottomLeft(self):
        return self._corners[3]

    @BottomLeft.setter
    def BottomLeft(self, value):
        self._corners[3].assign(value)

    def __eq__(self, other):
        if not isinstance(other, Border):
            return NotImplemented
        for edgeA, edgeB in zip(self._edges, other._edges):
            if not edgeA == edgeB:
                return False
        for cornerA, cornerB in zip(self._corners, other._corners):
            if not cornerA == cornerB:
                return False
        return True

    def __repr__(self):
        return """<Border
    Left={0!r},
    Top={1!r},
    Right={2!r},
    Bottom={3!r}>""".format(self.Left, self.Top, self.Right, self.Bottom)

    def getBox(self):
        return BaseBox(self.Left.Width, self.Top.Width,
            self.Right.Width, self.Bottom.Width)

    def geometryForRect(self, rect):
        """
        Creates geometry and returns a tuple
        ``(shrinkedRect, geometry)`` where shrinked rect is the rect
        shrinked by the border edge size.
        """
        
