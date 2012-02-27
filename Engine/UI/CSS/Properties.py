# File name: Properties.py
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

__all__ = ["Padding", "Margin", "Border", "Background"]

from Values import RGBA, Colour

class BaseBox(object):
    def __init__(self, *args):
        if len(args) == 0:
            self._left, self._right, self._top, self._bottom = 0, 0, 0, 0
        elif len(args) == 1:
            self._left, self._right, self._top, self._bottom = (int(args[0]), )*4
        elif len(args) == 4:
            self._left, self._top, self._right, self._bottom = (int(x) for x in args)
        else:
            raise ValueError("BaseBox expects 1 or 4 arguments.")

    def assign(self, value):
        if type(value) == int:
            self._left, self._right, self._top, self._bottom = value
            return
        if isinstance(value, BaseBox):
            self._left, self._right, self._top, self._bottom = (
                value._left, value._right, value._top, value._bottom
            )
        else:
            value = tuple((int(x) for x in value))
            if len(value) != 4:
                raise ValueError("BaseBox needs a tuple of 4 ints or 1 int")
            self._left, self._right, self._top, self._bottom = value

    def _checkValue(self, v):
        if int(v) < 0:
            raise ValueError("Box value must be non negative.")
    
    @property
    def Left(self):
        return self._left

    @Left.setter
    def Left(self, value):
        if self._left == value:
            return
        self._checkValue(value)
        self._left = value

    @property
    def Right(self):
        return self._right

    @Right.setter
    def Right(self, value):
        if self._right == value:
            return
        self._checkValue(value)
        self._right = value

    @property
    def Top(self):
        return self._top

    @Top.setter
    def Top(self, value):
        if self._top == value:
            return
        self._checkValue(value)
        self._top = value

    @property
    def Bottom(self):
        return self._bottom

    @Bottom.setter
    def Bottom(self, value):
        if self._bottom == value:
            return
        self._checkValue(value)
        self._bottom = value

    def __eq__(self, other):
        if not isinstance(other, BaseBox):
            return NotImplemented
        return (self._top == other._top and
            self._left == other._left and
            self._right == other._right and
            self._bottom == other._bottom)

    def __repr__(self):
        return "{0}({1}, {2}, {3}, {4})".format(self.__class__.__name__, self._left, self._top, self._right, self._bottom)

class Margin(BaseBox):
    pass

class Padding(BaseBox):
    pass

class BorderComponent(object):
    def __init__(self, width=0, style=None, colour=None, **kwargs):
        super(BorderComponent, self).__init__(**kwargs)
        self.Width = width
        self.Style = style or "solid"
        self.Colour = colour or RGBA(0., 0., 0., 0.)
    
    def assign(self, other):
        if not isinstance(other, BorderComponent):
            raise TypeError("Can only assign BorderComponents to BorderComponents")
        self.Width = other.Width
        self.Style = other.Style
        self.Colour = other.Colour

class BorderEdge(object):
    def __init__(self, **kwargs):
        self._width = None
        self._style = None
        self._colour = None
        super(BorderEdge, self).__init__(**kwargs)

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
    def Style(self):
        return self._style

    @Style.setter
    def Style(self, value):
        if value == self._style:
            return
        if value != "solid":
            raise ValueError("Border style must be solid")
        self._style = value
    
    @property
    def Colour(self):
        return self._colour

    @Colour.setter
    def Colour(self, value):
        if self._colour == value:
            return
        if not isinstance(value, Colour):
            raise TypeError("Border needs Colour instances as colours. Got {0} {1}".format(type(value), value))
        self._colour = value

    def __eq__(self, other):
        if not isinstance(other, BorderEdge):
            return NotImplemented
        return (self._width == other._width and
                self._style == other._style and
                self._colour == other._colour)

    def __repr__(self):
        return "BorderEdge(width={0!r}, style={1!r}, colour={2!r})".format(self._width, self._style, self._colour)

class Border(BorderComponent):
    def __init__(self, **kwargs):
        self._edges = [BorderEdge() for i in xrange(4)]
        super(Border, self).__init__(**kwargs)
    
    def assign(self, other):
        if isinstance(other, Border):            
            for edgeA, edgeB in zip(self._edges, other._edges):
                edgeA.Width = edgeB.Width
                edgeA.Style = edgeB.Style
                edgeA.Colour = edgeB.Colour
        elif isinstance(other, BorderComponent): 
            for edgeA in self._edges:
                edgeA.Width = other.Width
                edgeA.Style = other.Style
                edgeA.Colour = other.Colour
        else:    
            raise TypeError("Can only assign BorderComponents to BorderComponents")

    @property
    def Width(self):
        raise NotImplementedError("Cannot read global border width.")

    @Width.setter
    def Width(self, value):
        for edge in self._edges:
            edge.Width = value

    @property
    def Style(self):
        raise NotImplementedError("Cannot read global border style.")

    @Style.setter
    def Style(self, value):
        for edge in self._edges:
            edge.Style = value
    
    @property
    def Colour(self):
        raise NotImplementedError("Cannot read global border colour.")

    @Colour.setter
    def Colour(self, value):
        for edge in self._edges:
            edge.Colour = value

    @property
    def Left(self):
        return self._edges[0]

    @property
    def Top(self):
        return self._edges[1]

    @property
    def Right(self):
        return self._edges[2]

    @property
    def Bottom(self):
        return self._edges[3]

    def __eq__(self, other):
        if not isinstance(other, Border):
            return NotImplemented
        for edgeA, edgeB in zip(self._edges, other._edges):
            if not edgeA == edgeB:
                print("{0} neq {1}".format(edgeA, edgeB))
                return False
        return True

class Background(object):
    def __init__(self, **kwargs):
        super(Background, self).__init__(**kwargs)

class BackgroundImage(Background):
    def __init__(self, image, **kwargs):
        super(BackgroundImage, self).__init__(**kwargs)
        self._image = image
        # TODO: load image resource

    def __eq__(self, other):
        if not isinstance(other, Background):
            return NotImplemented
        if isinstance(other, BackgroundImage):
            return self._image == other._image
        else:
            return False

class BackgroundColour(Background):
    def __init__(self, colour, **kwargs):
        super(BackgroundColour, self).__init__(**kwargs)
        self._colour = colour

    def __eq__(self, other):
        if not isinstance(other, Background):
            return NotImplemented
        if isinstance(other, BackgroundColour):
            return self._colour == other._colour
        else:
            return False

class BackgroundGradient(Background):
    def __init__(self, gradient, **kwargs):
        super(BackgroundGradient, self).__init__(**kwargs)
        self._gradient = gradient

    def __eq__(self, other):
        if not isinstance(other, Background):
            return NotImplemented
        if isinstance(other, BackgroundGradient):
            return self._gradient == other._gradient
        else:
            return False
    
