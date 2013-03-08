# File name: BoxWidget.py
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

__all__ = ["AbstractVBox", "AbstractHBox", "VBox", "HBox", "Grid"]

import itertools
from operator import attrgetter
import functools
import math
import copy

import CSS.Minilanguage
import CSS.Rect as Rect

from WidgetBase import ParentWidget

from Engine.CEngine.Log import server, Severity
log = server.getChannel("ui")

def op_setattr(attr):
    def _setattr(obj, val):
        setattr(obj, attr, val)
    return _setattr

class BoxWidget(ParentWidget):
    @staticmethod
    def get_space_flex_width(iterable):
        flex_sum = 0
        static_width_sum = 0
        space_sum = 0
        for widget, space, flex, static_width in iterable:
            space_sum += space
            if static_width is None:
                flex_sum += flex
            else:
                static_width_sum += static_width
        return space_sum, flex_sum, static_width_sum

    def __init__(self, parent,
            aa_position_setter,
            fa_position_setter,
            aa_box_edges,
            fa_box_edges,
            aa_size_getter,
            aa_dimension_index,
            **kwargs):
        super(BoxWidget, self).__init__(parent, **kwargs)
        self._aa_position_setter = aa_position_setter # e.g. op_setattr("LeftRight")
        self._fa_position_setter = fa_position_setter # e.g. op_setattr("TopBottom")
        self._aa_box_edges = aa_box_edges # e.g. attrgetter("Left"), attrgetter("Right")
        self._fa_box_edges = fa_box_edges # e.g. attrgetter("Top"), attrgetter("Bottom")
        self._aa_size_getter = aa_size_getter # e.g. attrgetter("Width")
        self._aa_dimension_index = aa_dimension_index

    def _get_spacing_list(self, base_spacing):
        """
        Calculate the space to the left (top) for each child widget
        and the space of the last widget to the right (bottom).

        *base_spacing* must be the box-spacing-x (box-spacing-y) value
        of the widget.

        Return value is an iterable which consists of tuples of the
        following form::

             widget, space, widget_flex, widget_size

        where ``widget_flex`` is the flex value of the computed style
        of the widget and ``widget_size`` is the size the widget
        demands (currently the width (height) in the widgets computed
        style).

        Takes into account margin collapse, box spacing and negative
        margins properly.
        """
        getterA, getterB = self._aa_box_edges
        mystyle = self.ComputedStyle
        visible_widgets = [widget for widget in self if widget.Visible]

        widgets_l = [None] + visible_widgets

        results = []
        for left, this in zip(widgets_l, visible_widgets + [None]):

            left_space = getterA(mystyle.Padding) if left is None else getterB(left.ComputedStyle.Margin)
            if left_space < 0:
                left_space = base_spacing + left_space
            elif left is not None and this is not None:
                left_space = max(base_spacing, left_space)

            if this is not None:
                this_style = this.ComputedStyle

                my_left_margin = getterA(this_style.Margin)
                my_right_margin = getterB(this_style.Margin)
                flex = this_style.Flex
                size = this.get_dimensions()[self._aa_dimension_index]

                if my_left_margin < 0:
                    left_margin = left_space + my_left_margin
                else:
                    left_margin = max(my_left_margin, left_space)
            else:
                left_margin += getterB(mystyle.Padding)
                flex = 0
                size = None
            results.append((this, left_margin, flex, size))

        return results

    def _do_align(self, spacing_list,
            my_AAFAPos,
            my_AASize, my_FASize,
            my_FAPadding):
        """
            AA => Alignment Axis
            FA => Free Axis
        """
        aa_position_setter = self._aa_position_setter
        fa_position_setter = self._fa_position_setter
        fa_box_edge_a, fa_box_edge_b = self._fa_box_edges

        total_space, total_flex, total_static_width = self.get_space_flex_width(spacing_list)
        if total_flex > 0:
            widget_width_per_flex = (my_AASize - (total_space+total_static_width)) / total_flex
        else:
            # value doesn't matter, won't be used
            widget_width_per_flex = 1

        if math.ceil(widget_width_per_flex) <= 0:
            for widget in self:
                # this makes them implicitly invisible for any tests
                widget.AbsoluteRect = Rect.Rect(0, 0, 0, 0)
            log.log(Severity.Warning, "{0} has not enough space for {1} flexes (total_space={2},size={3},static_size={4})".format(self, total_flex, total_space, my_AASize, total_static_width))
            return

        fa_pos_a = my_AAFAPos[1]
        fa_pos_b = fa_pos_a + my_FASize

        aa_pos_a = spacing_list[0][1] + my_AAFAPos[0]

        for widget, space, flex, width in spacing_list[:-1]:
            #print("{0} aligning {1}:".format(self, widget))
            #print("  aa_range = ({0}, {1})".format(aa_pos_a, aa_pos_b))
            #print("  fa_range = ({0}, {1})".format(fa_pos_a, fa_pos_b))
            #print("  aa_widget_size = {0}".format(widget_AAWidth))

            aa_pos_a += space

            if width is None:
                widget_width = flex * widget_width_per_flex
            else:
                widget_width = width

            widget_margin = widget.ComputedStyle.Margin
            fa_spacing_a = my_FAPadding[0] + fa_box_edge_a(widget_margin)
            fa_spacing_b = my_FAPadding[1] + fa_box_edge_b(widget_margin)

            if fa_pos_a + fa_spacing_a < fa_pos_b - fa_spacing_b:
                fa_position_setter(widget.AbsoluteRect, (fa_pos_a + fa_spacing_a, fa_pos_b - fa_spacing_b))
                aa_position_setter(widget.AbsoluteRect, (round(aa_pos_a), round(aa_pos_a+widget_width)))
                if widget.ClientRect.Area <= 0:
                    widget.AbsoluteRect = Rect.Rect(0, 0, 0, 0)
            else:
                widget.AbsoluteRect = Rect.Rect(0, 0, 0, 0)

            aa_pos_a += widget_width

class AbstractVBox(BoxWidget):
    def __init__(self, parent, **kwargs):
        super(AbstractVBox, self).__init__(
            parent,
            op_setattr("TopBottom"),
            op_setattr("LeftRight"),
            (attrgetter("Top"), attrgetter("Bottom")),
            (attrgetter("Left"), attrgetter("Right")),
            attrgetter("Height"),
            1,
            **kwargs)

    def do_align(self):
        # print("{0} {1}".format(self, self.AbsoluteRect))
        if len(self) == 0:
            return
        mystyle = self.ComputedStyle
        spacing_list = self._get_spacing_list(self.ComputedStyle.BoxSpacingY)
        rect = copy.copy(self.AbsoluteRect)
        rect.shrink(mystyle.Border.get_box())
        x, y, w, h = rect.XYWH
        self._do_align(spacing_list, (y, x), h, w, (mystyle.Padding.Left, mystyle.Padding.Right))

class AbstractHBox(BoxWidget):
    def __init__(self, parent, **kwargs):
        super(AbstractHBox, self).__init__(
            parent,
            op_setattr("LeftRight"),
            op_setattr("TopBottom"),
            (attrgetter("Left"), attrgetter("Right")),
            (attrgetter("Top"), attrgetter("Bottom")),
            attrgetter("Width"),
            0,
            **kwargs)

    def do_align(self):
        # print("{0} {1}".format(self, self.AbsoluteRect))
        if len(self) == 0:
            return
        mystyle = self.ComputedStyle
        spacing_list = self._get_spacing_list(self.ComputedStyle.BoxSpacingX)
        rect = copy.copy(self.AbsoluteRect)
        rect.shrink(mystyle.Border.get_box())
        x, y, w, h = rect.XYWH
        self._do_align(spacing_list, (x, y), w, h, (mystyle.Padding.Top, mystyle.Padding.Bottom))

class Grid(ParentWidget):
    pass

class VBox(AbstractVBox):
    pass

class HBox(AbstractHBox):
    pass

CSS.Minilanguage.ElementNames().register_widget_class(VBox)
CSS.Minilanguage.ElementNames().register_widget_class(HBox)
CSS.Minilanguage.ElementNames().register_widget_class(Grid)
