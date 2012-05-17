# File name: Style.py
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

import itertools
import iterutils
import copy
import math

from typeutils import number

import CSS.Literals as Literals
from CSS.Box import Padding, Margin, BaseBox
from CSS.Border import Border, BorderEdge
from CSS.Fill import Fill, Colour, Transparent, Image
from CSS.Rules import Rule
from CSS.Rect import Rect

class Style(object):
    __hash__ = None
    
    def __init__(self, *rules, **kwargs):
        self._background = kwargs.pop("background") if "background" in kwargs else Transparent
        self._padding = Padding()
        self._margin = Margin()
        self._border = Border()
        self._boxSpacing = (0, 0)
        self._flex = 1
        self._textColour = Colour(0., 0., 0., 1.)
        self._width, self._height = None, None
        self._shear = (0, 0)
        if "padding" in kwargs:
            self.Padding = kwargs.pop("padding")
        if "margin" in kwargs:
            self.Margin = kwargs.pop("margin")
        if "border" in kwargs:
            self.Border = kwargs.pop("border")
        if "boxSpacingX" in kwargs:
            self.BoxSpacingX = kwargs.pop("boxSpacingX")
        if "boxSpacingY" in kwargs:
            self.BoxSpacingY = kwargs.pop("boxSpacingY")
        if "flex" in kwargs:
            self.Flex = kwargs.pop("flex")
        if "textColour" in kwargs:
            self.TextColour = kwargs.pop("textColour")
        if "height" in kwargs:
            self.Height = kwargs.pop("height")
        if "width" in kwargs:
            self.Width = kwargs.pop("width")
        if "shear" in kwargs:
            shear = kwargs.pop("shear")
            self._setShear(shear)
        super(Style, self).__init__(**kwargs)
        for rule in rules:
            self._addRule(rule)

    def __deepcopy__(self, memo):
        # FIXME: implement pickle interface
        return Style(
            background=copy.deepcopy(self.Background, memo),
            padding=copy.deepcopy(self.Padding, memo),
            margin=copy.deepcopy(self.Margin, memo),
            border=copy.deepcopy(self.Border, memo),
            boxSpacingX=self.BoxSpacingX,
            boxSpacingY=self.BoxSpacingY,
            flex=self.Flex,
            textColour=copy.deepcopy(self.TextColour, memo),
            width=self.Width,
            height=self.Height,
            shear=(self.ShearLeft, self.ShearRight)
        )

    def _addRule(self, rule):
        for key, value in rule._properties:
            self._applyProperty(key, value)

    def __add__(self, rules):
        if isinstance(rules, Rule):
            new = copy.deepcopy(self)
            new._addRule(rules)
        else:
            new = copy.deepcopy(self)
            if rules is None:
                return new
            for rule in rules:
                new._addRule(rules)
        return new

    def __eq__(self, other):
        if not isinstance(other, Style):
            return NotImplemented
        return (
            self._background == other._background and
            self._padding == other._padding and
            self._margin == other._margin and
            self._border == other._border and
            self._boxSpacing == other._boxSpacing and
            self._flex == other._flex and
            self._textColour == other._textColour and
            self._width == other._width and
            self._height == other._height and
            self._shear == other._shear
        )

    def __ne__(self, other):
        r = self.__eq__(other)
        if r is NotImplemented:
            return r
        else:
            return not r

    def __repr__(self):
        return """<Style
    Padding={0!r},
    Margin={1!r},
    Background={2!r},
    Border={3!r}>""".format(self._padding, self._margin, self._background, self._border)
        
    def _applyProperty(self, key, value):
        if key in self._literalSetters:
            if not isinstance(value, tuple):
                value = (value, )
            literalClass, propName = self._literalSetters[key]
            setattr(self, propName, literalClass(*value))
            return
        if key in self._propertySetters:
            self._propertySetters[key](self, value)
            return
        raise KeyError("{0} is not a valid stylesheet property".format(key))

    def _setBackgroundRepeat(self, value):
        if len(value) == 1:
            self.Background.RepeatX = value
            self.Background.RepeatY = value
        elif len(value) == 2:
            self.Background.RepeatX, self.Background.RepeatY = value
        else:
            raise ValueError("Unsupported amount of axis: {0}".format(len(value)))

    def _setBackgroundImage(self, value):
        img, = value
        self.Background = img

    def _setBackgroundColour(self, value):
        colour, = value
        self.Background = colour

    def _setBorderEdge(self, edge, value):
        setattr(self.Border, edge, BorderEdge(value[0], value[2]))

    def _setBaseBoxEdge(self, box, edge, value):
        box = getattr(self, box)
        if len(value) != 1:
            raise ValueError("Setting a box edge requires exactly one number.")
        setattr(box, edge, value[0])

    def _setBoxSpacing(self, value):
        if len(value) == 1:
            self.BoxSpacingX, self.BoxSpacingY = value[0], value[0]
        elif len(value) == 2:
            self.BoxSpacingX, self.BoxSpacingY = value
        else:
            raise ValueError("Box spacing requires one or two integer numbers.")

    def _setFlex(self, value):
        if len(value) != 1:
            raise ValueError("Flex requires one integer number.")
        self.Flex = int(value[0])

    def _setTextColour(self, value):
        color, = value
        self.TextColour = color

    def _setDimension(self, axis, value):
        dim, = value
        setattr(self, axis, dim)

    def _setShear(self, value, key=None):
        if key is None:
            if len(value) == 2:
                left, right = value
            else:
                both, = value
                left = both
                right = both
            self._shear = (left, right)
        else:
            if not hasattr(self, "Shear"+key):
                raise KeyError("shear-{0} is not a valid stylesheet property.".format(value.lower()))
            else:
                shear, = value
                setattr(self, "Shear"+key, shear)

    def _setBorderRadius(self, edgeY, edgeX, value):
        radius, = value
        setattr(self._border, "{0}{1}Radius".format(edgeY, edgeX), number(radius))

    def _setBorderRadiusAll(self, value):
        radius, = value
        self._border.setRadius(number(radius))

    @property
    def Background(self):
        return self._background

    @Background.setter
    def Background(self, value):
        if value is not None and not isinstance(value, Fill):
            raise TypeError("Background must be a Fill instance. Got {0} {1}".format(type(value), value))
        self._background = value

    @property
    def Padding(self):
        return self._padding

    @Padding.setter
    def Padding(self, value):
        if not isinstance(value, BaseBox):
            raise TypeError("Padding must be a BaseBox instance. Got {0} {1}".format(type(value), value))
        self._padding.assign(value)

    @property
    def Margin(self):
        return self._margin

    @Margin.setter
    def Margin(self, value):
        if not isinstance(value, BaseBox):
            raise TypeError("Margin must be a BaseBox instance. Got {0} {1}".format(type(value), value))
        self._margin.assign(value)

    @property
    def Border(self):
        return self._border

    @Border.setter
    def Border(self, value):
        if not isinstance(value, Border):
            raise TypeError("Border must be a Border instance. Got {0} {1}".format(type(value), value))
        self._border.assign(value)

    @property
    def BoxSpacing(self):
        return self._boxSpacing

    @BoxSpacing.setter
    def BoxSpacing(self, value):
        x, y = value
        x, y = int(x), int(x)
        if x < 0 or y < 0:
            raise ValueError("BoxSpacing must be non-negative.")
        self._boxSpacing = x, y

    @property
    def BoxSpacingX(self):
        return self._boxSpacing[0]

    @BoxSpacingX.setter
    def BoxSpacingX(self, value):
        x = int(value)
        if x < 0:
            raise ValueError("BoxSpacing must be non-negative.")
        self._boxSpacing = x, self._boxSpacing[1]

    @property
    def BoxSpacingY(self):
        return self._boxSpacing[1]

    @BoxSpacingY.setter
    def BoxSpacingY(self, value):
        y = int(value)
        if y < 0:
            raise ValueError("BoxSpacing must be non-negative.")
        self._boxSpacing = self._boxSpacing[0], y

    @property
    def Flex(self):
        return self._flex

    @Flex.setter
    def Flex(self, value):
        if value < 1:
            raise ValueError("Flex must be at least one or undefined.")
        self._flex = value

    @property
    def TextColour(self):
        return self._textColour

    @TextColour.setter
    def TextColour(self, value):
        if not isinstance(value, Colour):
            raise TypeError("TextColour must be a Colour instance. Got {0} {1}".format(type(value), value))
        self._textColour = value

    @property
    def Width(self):
        return self._width

    @Width.setter
    def Width(self, value):
        if value is not None:
            value = float(value)
            if value <= 0:
                raise ValueError("Width must be positive. Got {0} {1}".format(type(value), value))
        self._width = value

    @property
    def Height(self):
        return self._height

    @Height.setter
    def Height(self, value):
        if value is not None:
            value = float(value)
            if value <= 0:
                raise ValueError("Height must be positive. Got {0} {1}".format(type(value), value))
        self._height = value

    @property
    def Shear(self):
        return self._shear

    @Shear.setter
    def Shear(self, value):
        self._setShear(value)

    @property
    def ShearLeft(self):
        return self._shear[0]

    @ShearLeft.setter
    def ShearLeft(self, value):
        self._shear = (number(value), self._shear[1])

    @property
    def ShearRight(self):
        return self._shear[1]

    @ShearRight.setter
    def ShearRight(self, value):
        self._shear = (self._shear[0], number(value))

    _literalSetters = {
        "background": (Literals.BackgroundLiteral, "Background"),
        "padding": (Literals.BoxLiteral, "Padding"),
        "margin": (Literals.BoxLiteral, "Margin"),
        "border": (Literals.BorderLiteral, "Border"),
        "box-spacing-x": (Literals.IntLiteral, "BoxSpacingX"),
        "box-spacing-y": (Literals.IntLiteral, "BoxSpacingY"),
    }

    _propertySetters = {
        "flex": lambda self, value: self._setFlex(value),
        "box-spacing": lambda self, value: self._setBoxSpacing(value),
        "background-image": lambda self, value: self._setBackgroundImage(value),
        "background-repeat": lambda self, value: self._setBackgroundRepeat(value),
        "background-color": lambda self, value: self._setBackgroundColour(value),
        "border-left": lambda self, value: self._setBorderEdge("Left", value),
        "border-right": lambda self, value: self._setBorderEdge("Right", value),
        "border-top": lambda self, value: self._setBorderEdge("Top", value),
        "border-bottom": lambda self, value: self._setBorderEdge("Bottom", value),
        "border-radius": lambda self, value: self._setBorderRadiusAll(value),
        "border-top-left-radius": lambda self, value: self._setBorderRadius("Top", "Left", value),
        "border-top-right-radius": lambda self, value: self._setBorderRadius("Top", "Right", value),
        "border-bottom-left-radius": lambda self, value: self._setBorderRadius("Bottom", "Left", value),
        "border-bottom-right-radius": lambda self, value: self._setBorderRadius("Bottom", "Right", value),
        "padding-left": lambda self, value: self._setBaseBoxEdge("Padding", "Left", value),
        "padding-right": lambda self, value: self._setBaseBoxEdge("Padding", "Right", value),
        "padding-top": lambda self, value: self._setBaseBoxEdge("Padding", "Top", value),
        "padding-bottom": lambda self, value: self._setBaseBoxEdge("Padding", "Bottom", value),
        "margin-left": lambda self, value: self._setBaseBoxEdge("Margin", "Left", value),
        "margin-right": lambda self, value: self._setBaseBoxEdge("Margin", "Right", value),
        "margin-top": lambda self, value: self._setBaseBoxEdge("Margin", "Top", value),
        "margin-bottom": lambda self, value: self._setBaseBoxEdge("Margin", "Bottom", value),
        "color": lambda self, value: self._setTextColour(value),
        "width": lambda self, value: self._setDimension("Width", value),
        "height": lambda self, value: self._setDimension("Height", value),
        "shear-left": lambda self, value: self._setShear(value, "Left"),
        "shear-right": lambda self, value: self._setShear(value, "Right"),
        "shear": lambda self, value: self._setShear(value)
    }

    def geometryForRect(self, rect, faceBuffer):
        """
        Takes a widget rect and creates ui geometry for it using
        the geometryForRect calls of the respective filler objects.

        Returns a Rect instance representing the client rect for the
        passed rect, that is the rect shrinked by any Border and Padding
        defined in this style.

        Does not take into account margin.
        """
        clientRect = copy.copy(rect)
        clientRect.shrink(self.Border.geometryForRect(rect, faceBuffer))
        self.Background.geometryForRect(clientRect, faceBuffer)
        clientRect.shrink(self.Padding)
        return clientRect

    def inCairo(self, rect, ctx):
        clientRect = copy.copy(rect)
        clientRect.shrink(self.Border.getBox())
        
        widths = (  self._border.Left.Width, self._border.Top.Width,
                    self._border.Right.Width, self._border.Bottom.Width)

        colours = ( self._border.Left.Fill, self._border.Top.Fill,
                    self._border.Right.Fill, self._border.Bottom.Fill)

        # this is slow like hell
        # equalWidths = not any(widths[0] != width for width in widths)
        # equalColours = not any(colours[0] != colour for colour in colours)
        
        # this is 15% faster than the above
        # equalWidths = False
        # equalColours = False
        
        # this is (for equal colours + widths) 3% faster than the
        # previous and 17% faster than the first
        # for unequal colours (first is already unequal), we find
        # that this is still 14% faster than the first solution.
        equalWidths = (widths[0] == widths[1] and
                       widths[1] == widths[2] and
                       widths[2] == widths[3])
        if equalWidths and self._background is Transparent:            
            equalColours = (colours[0] == colours[1] and
                            colours[1] == colours[2] and
                            colours[2] == colours[3])
            if equalColours:
                return
            bothEqual = False
        else:
            # comparing colours is way more expensive than comparing
            # widths. So we only compare if widths are equal. We only
            # need both information at the same time anyways
            bothEqual = (equalWidths and
                colours[0] == colours[1] and
                colours[1] == colours[2] and
                colours[2] == colours[3])
        
        radii = self._border.getAllRadii()

        shears = self._shear

        shearTopLeft = shears[0] if shears[0] > 0 else 0
        shearTopRight = -shears[1] if shears[1] > 0 else 0
        shearBottomLeft = -shears[0] if shears[0] < 0 else 0
        shearBottomRight = shears[1] if shears[1] < 0 else 0

        corners = [
            (clientRect.Left + shearTopLeft + radii[0], clientRect.Top + radii[0]),
            (clientRect.Right + shearTopRight - radii[1], clientRect.Top + radii[1]),
            (clientRect.Right + shearBottomRight - radii[2], clientRect.Bottom - radii[2]),
            (clientRect.Left + shearBottomLeft + radii[3], clientRect.Bottom - radii[3])
        ]

        # pulling these in the local namespace gives only 1% speedup,
        # but we take everything we can get
        background = self._background
        backgroundNotTransparent = background is not Transparent
        
        if sum(radii) == 0:
            # optimize for straight borders
            if backgroundNotTransparent or bothEqual:
                ctx.move_to(*corners[0])
                ctx.line_to(*corners[1])
                ctx.line_to(*corners[2])
                ctx.line_to(*corners[3])
                ctx.close_path()

            if bothEqual:
                if backgroundNotTransparent:
                    background.setSource(ctx)
                    ctx.fill_preserve()
                ctx.set_line_width(widths[0])
                colours[0].setSource(ctx)
                ctx.stroke()
            else:
                background.setSource(ctx)
                ctx.fill()
                
                for i, (width, fill) in enumerate(itertools.izip(widths, colours)):
                    if width <= 0 or fill is Transparent:
                        continue
                    ctx.set_line_width(width)
                    fill.setSource(ctx)
                    ctx.move_to(*corners[i-1])
                    ctx.line_to(*corners[i])
                    ctx.stroke()
        else:
            leftLessAngle = math.atan(shears[0]/clientRect.Height)
            rightLessAngle = math.atan(shears[1]/clientRect.Height)
            pi = math.pi

            # explicitly put this in a list here
            pathSegments = list(itertools.izip(corners, itertools.izip(radii,
                [   # start angles
                    pi + leftLessAngle,
                    3*(pi/2),
                    -rightLessAngle,
                    pi/2
                ],
                [   # stop angles
                    3*(pi/2),
                    2*pi - rightLessAngle,
                    pi/2,
                    pi + leftLessAngle
                ]
            )))

            if backgroundNotTransparent or bothEqual:
                for (x, y), segment in pathSegments:
                    ctx.arc(x, y, *segment)
                ctx.close_path()
            
            if bothEqual:
                if backgroundNotTransparent:
                    background.setSource(ctx)
                    ctx.fill_preserve()
                ctx.set_line_width(widths[0])
                colours[0].setSource(ctx)
                ctx.stroke()
            else:
                background.setSource(ctx)
                ctx.fill()

                for i, width, fill in itertools.izip(itertools.count(0), widths, colours):
                    if width <= 0 or fill is Transparent:
                        continue
                    # FIXME: optimize odd widths
                    ctx.set_line_width(width)
                    fill.setSource(ctx)
                    (x, y), segment = pathSegments[i-1]
                    ctx.arc(x, y, *segment)
                    (x, y), segment = pathSegments[i]
                    ctx.arc(x, y, *segment)
                    ctx.stroke()
