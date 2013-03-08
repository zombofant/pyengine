# File name: Style.py
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

import itertools
import iterutils
import copy
import math

from typeutils import number

import Engine.CEngine.Pango as Pango

import CSS.Literals as Literals
import CSS.Box as Box
import CSS.Border
from CSS.Box import Padding, Margin, BaseBox
from CSS.Border import Border, BorderEdge
from CSS.Fill import Fill, Colour, Transparent, Image
from CSS.Rules import Rule
from CSS.Rect import Rect
from CSS.Selectors import State
from CSS.Utils import css_inheritable, Inheritables, css_inheritance_recurse

class Style(object):
    __metaclass__ = Inheritables
    __hash__ = None

    class PropGroup(object):
        pass

    Visual = PropGroup()
    Layout = PropGroup()

    def __init__(self, *rules, **kwargs):
        inherit = Literals.Inherit

        self._background = kwargs.pop("background", Transparent)
        self._padding = Padding()
        self._margin = Margin()
        self._border = Border()
        self._box_spacing = (0, 0)
        self._flex = 1
        self._text_colour = inherit
        self._width, self._height = None, None
        self._shear = (0, 0)
        self._text_align = kwargs.pop("text_align", inherit)
        self._font_weight = kwargs.pop("font_weight", inherit)
        self._font_size = kwargs.pop("font_size", inherit)
        self._font_family = kwargs.pop("font_family", inherit)
        self._vertical_align = kwargs.pop("vertical_align", inherit)
        if "padding" in kwargs:
            self.Padding = kwargs.pop("padding")
        if "margin" in kwargs:
            self.Margin = kwargs.pop("margin")
        if "border" in kwargs:
            self.Border = kwargs.pop("border")
        if "box_spacing_x" in kwargs:
            self.BoxSpacingX = kwargs.pop("box_spacing_x")
        if "box_spacing_y" in kwargs:
            self.BoxSpacingY = kwargs.pop("box_spacing_y")
        if "flex" in kwargs:
            self.Flex = kwargs.pop("flex")
        if "text_colour" in kwargs:
            self.TextColour = kwargs.pop("text_colour")
        if "height" in kwargs:
            self.Height = kwargs.pop("height")
        if "width" in kwargs:
            self.Width = kwargs.pop("width")
        if "shear" in kwargs:
            shear = kwargs.pop("shear")
            self._set_shear(shear)
        self._fontdesc_cache = None
        super(Style, self).__init__(**kwargs)
        for rule in rules:
            self._add_rule(rule)

    def __deepcopy__(self, memo):
        # FIXME: implement pickle interface
        new = Style(
            background=copy.deepcopy(self.Background, memo),
            padding=copy.deepcopy(self.Padding, memo),
            margin=copy.deepcopy(self.Margin, memo),
            border=copy.deepcopy(self.Border, memo),
            box_spacing_x=self.BoxSpacingX,
            box_spacing_y=self.BoxSpacingY,
            flex=self.Flex,
            text_colour=copy.deepcopy(self.TextColour, memo),
            width=self.Width,
            height=self.Height,
            shear=(self.ShearLeft, self.ShearRight),
            text_align=self._text_align,
            font_weight=self._font_weight,
            font_size=self._font_size,
            font_family=self._font_family,
            vertical_align=self._vertical_align
        )
        new._fontdesc_cache = self._fontdesc_cache
        return new

    def diff(self, other):
        diffs = set()
        if (self._background != other._background or
            self._text_colour != other._text_colour or
            self._shear != other._shear or
            self._border != other._border or
            self._font_size != other._font_size or
            self._font_weight != other._font_weight or
            self._font_family != other._font_family or
            self._text_align != other._text_align or
            self._vertical_align != other._vertical_align):

            diffs.add(self.Visual)

        if (self._border != other._border or
            self._width != other._width or
            self._height != other._height or
            self._flex != other._flex or
            self._box_spacing != other._box_spacing or
            self._margin != other._margin or
            self._padding != other._padding):

            diffs.add(self.Layout)

        return diffs

    def _add_rule(self, rule):
        for key, value in rule._properties:
            self._apply_property(key, value)

    def _invalidate_fontdesc_cache(self):
        self._fontdesc_cache = None

    def __add__(self, rules):
        new = copy.deepcopy(self)
        new += rules
        return new

    def __iadd__(self, rules):
        if isinstance(rules, Rule):
            self._add_rule(rules)
            return self
        else:
            if rules is None:
                return self
            for rule in rules:
                self._add_rule(rule)
            return self
        return NotImplemented

    def __eq__(self, other):
        if not isinstance(other, Style):
            return NotImplemented
        return (
            self._background == other._background and
            self._padding == other._padding and
            self._margin == other._margin and
            self._border == other._border and
            self._box_spacing == other._box_spacing and
            self._flex == other._flex and
            self._text_colour == other._text_colour and
            self._width == other._width and
            self._height == other._height and
            self._shear == other._shear and
            self._text_align == other._text_align and
            self._font_weight == other._font_weight and
            self._font_size == other._font_size and
            self._vertical_align == other._vertical_align and
            self._font_family == other._font_family
        )

    def __ne__(self, other):
        r = self.__eq__(other)
        if r is NotImplemented:
            return r
        else:
            return not r

    def __repr__(self):
        return """<Style
    Padding={!r},
    Margin={!r},
    Background={!r},
    Border={!r},
    BoxSpacing={!r},
    Flex={!r},
    TextColour={!r},
    Width={!r},
    Height={!r},
    Shear={!r},
    TextAlign={!r},
    FontWeight={!r},
    FontSize={!r},
    FontFamily={!r},
    VerticalAlign={!r}>""".format(
            self._padding,
            self._margin,
            self._background,
            self._border,
            self._box_spacing,
            self._flex,
            self._text_colour,
            self._width,
            self._height,
            self._shear,
            self._text_align,
            self._font_weight,
            self._font_size,
            self._font_family,
            self._vertical_align)

    def _apply_property(self, key, value):
        if key in self._literal_setters:
            if not isinstance(value, tuple):
                value = (value, )
            literal_class, prop_name = self._literal_setters[key]
            setattr(self, prop_name, literal_class(*value))
            return
        if key in self._property_setters:
            self._property_setters[key](self, value)
            return
        if key in self._trivial_setters:
            propname, = self._trivial_setters[key]
            actual_value, = value
            setattr(self, propname, actual_value)
            return
        raise KeyError("{0} is not a valid stylesheet property".format(key))

    def _set_background_repeat(self, value):
        if len(value) == 1:
            self.Background.RepeatX = value
            self.Background.RepeatY = value
        elif len(value) == 2:
            self.Background.RepeatX, self.Background.RepeatY = value
        else:
            raise ValueError("Unsupported amount of axis: {0}".format(len(value)))

    def _set_background_image(self, value):
        img, = value
        self.Background = img

    def _set_background_colour(self, value):
        colour, = value
        self.Background = colour

    def _set_border_edge(self, edge, value):
        setattr(self.Border, edge, BorderEdge(value[0], value[2]))

    def _set_base_box_edge(self, box, edge, value):
        box = getattr(self, box)
        if len(value) != 1:
            raise ValueError("Setting a box edge requires exactly one number.")
        setattr(box, edge, value[0])

    def _set_box_spacing(self, value):
        if len(value) == 1:
            self.BoxSpacingX, self.BoxSpacingY = value[0], value[0]
        elif len(value) == 2:
            self.BoxSpacingX, self.BoxSpacingY = value
        else:
            raise ValueError("Box spacing requires one or two integer numbers.")

    def _set_flex(self, value):
        if len(value) != 1:
            raise ValueError("Flex requires one integer number.")
        self.Flex = int(value[0])

    def _set_text_colour(self, value):
        color, = value
        self.TextColour = color

    def _set_dimension(self, axis, value):
        dim, = value
        setattr(self, axis, dim)

    def _set_shear(self, value, key=None):
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

    def _set_border_radius(self, edgeY, edgeX, value):
        radius, = value
        setattr(self._border, "{0}{1}Radius".format(edgeY, edgeX), number(radius))

    def _set_border_radius_all(self, value):
        radius, = value
        self._border.set_radius(number(radius))

    @css_inheritable
    def Background(self):
        return self._background

    @Background.setter
    def Background(self, value):
        if value is not None and not isinstance(value, Fill):
            raise TypeError("Background must be a Fill instance. Got {0} {1}".format(type(value), value))
        self._background = value

    @css_inheritance_recurse(Box.Padding)
    def Padding(self):
        return self._padding

    @Padding.setter
    def Padding(self, value):
        if not isinstance(value, BaseBox):
            raise TypeError("Padding must be a BaseBox instance. Got {0} {1}".format(type(value), value))
        self._padding.assign(value)

    @css_inheritance_recurse(Box.Margin)
    def Margin(self):
        return self._margin

    @Margin.setter
    def Margin(self, value):
        if not isinstance(value, BaseBox):
            raise TypeError("Margin must be a BaseBox instance. Got {0} {1}".format(type(value), value))
        self._margin.assign(value)

    @css_inheritance_recurse(CSS.Border.Border)
    def Border(self):
        return self._border

    @Border.setter
    def Border(self, value):
        if not isinstance(value, Border):
            raise TypeError("Border must be a Border instance. Got {0} {1}".format(type(value), value))
        self._border.assign(value)

    @property
    def BoxSpacing(self):
        return self._box_spacing

    @BoxSpacing.setter
    def BoxSpacing(self, value):
        x, y = value
        x, y = int(x), int(x)
        if x < 0 or y < 0:
            raise ValueError("BoxSpacing must be non-negative.")
        self._box_spacing = x, y

    @css_inheritable
    def BoxSpacingX(self):
        return self._box_spacing[0]

    @BoxSpacingX.setter
    def BoxSpacingX(self, value):
        x = int(value)
        if x < 0:
            raise ValueError("BoxSpacing must be non-negative.")
        self._box_spacing = x, self._box_spacing[1]

    @css_inheritable
    def BoxSpacingY(self):
        return self._box_spacing[1]

    @BoxSpacingY.setter
    def BoxSpacingY(self, value):
        y = int(value)
        if y < 0:
            raise ValueError("BoxSpacing must be non-negative.")
        self._box_spacing = self._box_spacing[0], y

    @css_inheritable
    def Flex(self):
        return self._flex

    @Flex.setter
    def Flex(self, value):
        if value < 1:
            raise ValueError("Flex must be at least one or undefined.")
        self._flex = value

    @css_inheritable
    def TextColour(self):
        return self._text_colour

    @TextColour.setter
    def TextColour(self, value):
        self._text_colour = value

    @css_inheritable
    def Width(self):
        return self._width

    @Width.setter
    def Width(self, value):
        if value is not None and value is not Literals.Auto:
            value = float(value)
            if value <= 0:
                raise ValueError("Width must be positive. Got {0} {1}".format(type(value), value))
        self._width = value

    @css_inheritable
    def Height(self):
        return self._height

    @Height.setter
    def Height(self, value):
        if value is not None and value is not Literals.Auto:
            value = float(value)
            if value <= 0:
                raise ValueError("Height must be positive. Got {0} {1}".format(type(value), value))
        self._height = value

    @property
    def Shear(self):
        return self._shear

    @Shear.setter
    def Shear(self, value):
        self._set_shear(value)

    @css_inheritable
    def ShearLeft(self):
        return self._shear[0]

    @ShearLeft.setter
    def ShearLeft(self, value):
        self._shear = (number(value), self._shear[1])

    @css_inheritable
    def ShearRight(self):
        return self._shear[1]

    @ShearRight.setter
    def ShearRight(self, value):
        self._shear = (self._shear[0], number(value))

    @css_inheritable
    def TextAlign(self):
        return self._text_align

    @TextAlign.setter
    def TextAlign(self, value):
        self._text_align = value

    @css_inheritable
    def FontWeight(self):
        return self._font_weight

    @FontWeight.setter
    def FontWeight(self, value):
        self._font_weight = value
        self._invalidate_fontdesc_cache()

    @css_inheritable
    def FontSize(self):
        return self._font_size

    @FontSize.setter
    def FontSize(self, value):
        self._font_size = float(value) if value is not Literals.Inherit else value
        self._invalidate_fontdesc_cache()

    @css_inheritable
    def FontFamily(self):
        return self._font_family

    @FontFamily.setter
    def FontFamily(self, value):
        self._font_family = value
        self._invalidate_fontdesc_cache()

    @css_inheritable
    def VerticalAlign(self):
        return self._vertical_align

    @VerticalAlign.setter
    def VerticalAlign(self, value):
        self._vertical_align = value

    _literal_setters = {
        "background": (Literals.BackgroundLiteral, "Background"),
        "padding": (Box.Padding, "Padding"),
        "margin": (Box.Margin, "Margin"),
        "border": (Literals.BorderLiteral, "Border"),
        "box-spacing-x": (Literals.IntLiteral, "BoxSpacingX"),
        "box-spacing-y": (Literals.IntLiteral, "BoxSpacingY"),
    }

    _property_setters = {
        "flex": lambda self, value: self._set_flex(value),
        "box-spacing": lambda self, value: self._set_box_spacing(value),
        "background-image": lambda self, value: self._set_background_image(value),
        "background-repeat": lambda self, value: self._set_background_repeat(value),
        "background-color": lambda self, value: self._set_background_colour(value),
        "border-left": lambda self, value: self._set_border_edge("Left", value),
        "border-right": lambda self, value: self._set_border_edge("Right", value),
        "border-top": lambda self, value: self._set_border_edge("Top", value),
        "border-bottom": lambda self, value: self._set_border_edge("Bottom", value),
        "border-radius": lambda self, value: self._set_border_radius_all(value),
        "border-top-left-radius": lambda self, value: self._set_border_radius("Top", "Left", value),
        "border-top-right-radius": lambda self, value: self._set_border_radius("Top", "Right", value),
        "border-bottom-left-radius": lambda self, value: self._set_border_radius("Bottom", "Left", value),
        "border-bottom-right-radius": lambda self, value: self._set_border_radius("Bottom", "Right", value),
        "padding-left": lambda self, value: self._set_base_box_edge("Padding", "Left", value),
        "padding-right": lambda self, value: self._set_base_box_edge("Padding", "Right", value),
        "padding-top": lambda self, value: self._set_base_box_edge("Padding", "Top", value),
        "padding-bottom": lambda self, value: self._set_base_box_edge("Padding", "Bottom", value),
        "margin-left": lambda self, value: self._set_base_box_edge("Margin", "Left", value),
        "margin-right": lambda self, value: self._set_base_box_edge("Margin", "Right", value),
        "margin-top": lambda self, value: self._set_base_box_edge("Margin", "Top", value),
        "margin-bottom": lambda self, value: self._set_base_box_edge("Margin", "Bottom", value),
        "color": lambda self, value: self._set_text_colour(value),
        "width": lambda self, value: self._set_dimension("Width", value),
        "height": lambda self, value: self._set_dimension("Height", value),
        "shear-left": lambda self, value: self._set_shear(value, "Left"),
        "shear-right": lambda self, value: self._set_shear(value, "Right"),
        "shear": lambda self, value: self._set_shear(value),
    }

    _trivial_setters = {
        "text-align": ("TextAlign",),
        "font-weight": ("FontWeight",),
        "font-size": ("FontSize",),
        "vertical-align": ("VerticalAlign",),
        "font-family": ("FontFamily",)
    }

    @property
    def FontDescription(self):
        if self._fontdesc_cache is not None:
            return self._fontdesc_cache

        fontdesc = Pango.FontDescription()
        fontdesc.set_weight(self._font_weight)
        fontdesc.set_size(self._font_size * Pango.SCALE)
        fontdesc.set_family(self._font_family)
        self._fontdesc_cache = fontdesc
        return fontdesc

    def geometry_for_rect(self, rect, face_buffer):
        """
        Takes a widget rect and creates ui geometry for it using
        the geometry_for_rect calls of the respective filler objects.

        Returns a Rect instance representing the client rect for the
        passed rect, that is the rect shrinked by any Border and Padding
        defined in this style.

        Does not take into account margin.
        """
        client_rect = copy.copy(rect)
        client_rect.shrink(self.Border.geometry_for_rect(rect, face_buffer))
        self.Background.geometry_for_rect(client_rect, face_buffer)
        client_rect.shrink(self.Padding)
        return client_rect

    def in_cairo(self, ctx, rect):
        cl, cr, ct, cb = rect.LRTB
        bl, bt, br, bb = self.Border.Widths

        cl += bl / 2
        cr -= br / 2
        ct += bt / 2
        cb -= bb / 2
        ch = cb - ct
        cw = cr - cl

        widths = (  self._border.Left.Width, self._border.Top.Width,
                    self._border.Right.Width, self._border.Bottom.Width)

        colours = ( self._border.Left.Fill, self._border.Top.Fill,
                    self._border.Right.Fill, self._border.Bottom.Fill)

        # this is slow like hell
        # equal_widths = not any(widths[0] != width for width in widths)
        # equal_colours = not any(colours[0] != colour for colour in colours)

        # this is 15% faster than the above
        # equal_widths = False
        # equal_colours = False

        # this is (for equal colours + widths) 3% faster than the
        # previous and 17% faster than the first
        # for unequal colours (first is already unequal), we find
        # that this is still 14% faster than the first solution.
        equal_widths = (widths[0] == widths[1] and
                       widths[1] == widths[2] and
                       widths[2] == widths[3])
        if equal_widths and self._background is Transparent:
            equal_colours = (colours[0] == colours[1] and
                            colours[1] == colours[2] and
                            colours[2] == colours[3])
            if equal_colours and widths[0] == 0:
                return
            both_equal = False
        else:
            # comparing colours is way more expensive than comparing
            # widths. So we only compare if widths are equal. We only
            # need both information at the same time anyways
            both_equal = (equal_widths and
                colours[0] == colours[1] and
                colours[1] == colours[2] and
                colours[2] == colours[3])
        radii = self._border.get_all_radii()

        shears = self._shear

        shear_top_left = shears[0] if shears[0] > 0 else 0
        shear_top_right = -shears[1] if shears[1] > 0 else 0
        shear_bottom_left = -shears[0] if shears[0] < 0 else 0
        shear_bottom_right = shears[1] if shears[1] < 0 else 0

        corners = [
            (cl + shear_top_left + radii[0], ct + radii[0]),
            (cr + shear_top_right - radii[1], ct + radii[1]),
            (cr + shear_bottom_right - radii[2], cb - radii[2]),
            (cl + shear_bottom_left + radii[3], cb - radii[3])
        ]

        # pulling these in the local namespace gives only 1% speedup,
        # but we take everything we can get
        background = self._background
        background_not_transparent = background is not Transparent

        if sum(radii) == 0:
            # optimize for straight borders
            if background_not_transparent or both_equal:
                ctx.move_to(*corners[0])
                ctx.line_to(*corners[1])
                ctx.line_to(*corners[2])
                ctx.line_to(*corners[3])
                ctx.close_path()

            if both_equal:
                if background_not_transparent:
                    background.set_source(ctx, crect)
                    ctx.fill_preserve()
                ctx.set_line_width(widths[0])
                colours[0].set_source(ctx, crect)
                ctx.stroke()
            else:
                background.set_source(ctx, crect)
                ctx.fill()

                for i, (width, fill) in enumerate(itertools.izip(widths, colours)):
                    if width <= 0 or fill is Transparent:
                        continue
                    ctx.set_line_width(width)
                    fill.set_source(ctx, crect)
                    ctx.move_to(*corners[i-1])
                    ctx.line_to(*corners[i])
                    ctx.stroke()
        else:
            left_less_angle = math.atan(shears[0]/ch)
            right_less_angle = math.atan(shears[1]/ch)
            pi = math.pi

            # explicitly put this in a list here
            path_segments = list(itertools.izip(corners, itertools.izip(radii,
                [   # start angles
                    pi + left_less_angle,
                    3*(pi/2),
                    -right_less_angle,
                    pi/2
                ],
                [   # stop angles
                    3*(pi/2),
                    2*pi - right_less_angle,
                    pi/2,
                    pi + left_less_angle
                ]
            )))

            if background_not_transparent or both_equal:
                for (x, y), segment in path_segments:
                    ctx.arc(x, y, *segment)
                ctx.close_path()

            if both_equal:
                if background_not_transparent:
                    background.set_source(ctx)
                    ctx.fill_preserve()
                ctx.set_line_width(widths[0])
                colours[0].set_source(ctx)
                ctx.stroke()
            else:
                background.set_source(ctx)
                ctx.fill()

                for i, width, fill in itertools.izip(itertools.count(0), widths, colours):
                    if width <= 0 or fill is Transparent:
                        continue
                    # FIXME: optimize odd widths
                    ctx.set_line_width(width)
                    fill.set_source(ctx)
                    (x, y), segment = path_segments[i-1]
                    ctx.arc(x, y, *segment)
                    (x, y), segment = path_segments[i]
                    ctx.arc(x, y, *segment)
                    ctx.stroke()

class BaseStyle(Style):
    def __init__(self):
        super(BaseStyle, self).__init__(
            font_family="sans",
            font_size=12,
            text_colour=Colour(0, 0, 0, 1),
            font_weight=Literals.Pango.Weight.NORMAL,
            text_align=Literals.Pango.Alignment.LEFT,
            vertical_align=Literals.VerticalAlign.Top
            )
