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

__all__ = ["VBox", "HBox", "Grid"]

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

def opSetattr(attr):
    def _setattr(obj, val):
        setattr(obj, attr, val)
    return _setattr

class BoxWidget(ParentWidget):
    @staticmethod
    def getSpaceFlexWidth(iterable):
        flexSum = 0
        staticWidthSum = 0
        spaceSum = 0
        for widget, space, flex, staticWidth in iterable:
            spaceSum += space
            if staticWidth is None:
                flexSum += flex
            else:
                staticWidthSum += staticWidth
        return spaceSum, flexSum, staticWidthSum

    def __init__(self, parent,
            aaPositionSetter,
            faPositionSetter,
            aaBoxEdges,
            faBoxEdges,
            aaSizeGetter,
            aaDimensionIndex,
            **kwargs):
        super(BoxWidget, self).__init__(parent, **kwargs)
        self._aaPositionSetter = aaPositionSetter # e.g. opSetattr("LeftRight")
        self._faPositionSetter = faPositionSetter # e.g. opSetattr("TopBottom")
        self._aaBoxEdges = aaBoxEdges # e.g. attrgetter("Left"), attrgetter("Right")
        self._faBoxEdges = faBoxEdges # e.g. attrgetter("Top"), attrgetter("Bottom")
        self._aaSizeGetter = aaSizeGetter # e.g. attrgetter("Width")
        self._aaDimensionIndex = aaDimensionIndex

    def _getSpacingList(self, baseSpacing):
        """
        Calculate the space to the left (top) for each child widget
        and the space of the last widget to the right (bottom).

        *baseSpacing* must be the box-spacing-x (box-spacing-y) value
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
        getterA, getterB = self._aaBoxEdges
        myStyle = self.ComputedStyle
        visible_widgets = [widget for widget in self if widget.Visible]

        widgets_l = [None] + visible_widgets

        results = []
        for left, this in zip(widgets_l, visible_widgets + [None]):

            left_space = getterA(myStyle.Padding) if left is None else getterB(left.ComputedStyle.Margin)
            if left_space < 0:
                left_space = baseSpacing + left_space
            elif left is not None and this is not None:
                left_space = max(baseSpacing, left_space)

            if this is not None:
                this_style = this.ComputedStyle

                my_left_margin = getterA(this_style.Margin)
                my_right_margin = getterB(this_style.Margin)
                flex = this_style.Flex
                size = this.getDimensions()[self._aaDimensionIndex]

                if my_left_margin < 0:
                    left_margin = left_space + my_left_margin
                else:
                    left_margin = max(my_left_margin, left_space)
            else:
                left_margin = max(left_space, getterB(myStyle.Padding))
                flex = 0
                size = None
            results.append((this, left_margin, flex, size))

        return results

    def _doAlign(self, spacingList,
            myAAFAPos,
            myAASize, myFASize,
            myFAPadding):
        """
            AA => Alignment Axis
            FA => Free Axis
        """
        aaPositionSetter = self._aaPositionSetter
        faPositionSetter = self._faPositionSetter
        faBoxEdgeA, faBoxEdgeB = self._faBoxEdges

        totalSpace, totalFlex, totalStaticWidth = self.getSpaceFlexWidth(spacingList)
        if totalFlex > 0:
            widgetWidthPerFlex = (myAASize - (totalSpace+totalStaticWidth)) / totalFlex
        else:
            # value doesn't matter, won't be used
            widgetWidthPerFlex = 1

        if math.ceil(widgetWidthPerFlex) <= 0:
            for widget in self:
                # this makes them implicitly invisible for any tests
                widget.AbsoluteRect = Rect.Rect(0, 0, 0, 0)
            log.log(Severity.Warning, "{0} has not enough space for {1} flexes (totalSpace={2},size={3},staticSize={4})".format(self, totalFlex, totalSpace, myAASize, totalStaticWidth))
            return

        faPosA = myAAFAPos[1]
        faPosB = faPosA + myFASize

        aaPosA = spacingList[0][1] + myAAFAPos[0]

        for widget, space, flex, width in spacingList[:-1]:
            #print("{0} aligning {1}:".format(self, widget))
            #print("  aaRange = ({0}, {1})".format(aaPosA, aaPosB))
            #print("  faRange = ({0}, {1})".format(faPosA, faPosB))
            #print("  aaWidgetSize = {0}".format(widgetAAWidth))

            aaPosA += space

            if width is None:
                widgetWidth = flex * widgetWidthPerFlex
            else:
                widgetWidth = width

            widgetMargin = widget.ComputedStyle.Margin
            faSpacingA = max(myFAPadding[0], faBoxEdgeA(widgetMargin))
            faSpacingB = max(myFAPadding[1], faBoxEdgeB(widgetMargin))

            if faPosA + faSpacingA < faPosB - faSpacingB:
                faPositionSetter(widget.AbsoluteRect, (faPosA + faSpacingA, faPosB - faSpacingB))
                aaPositionSetter(widget.AbsoluteRect, (round(aaPosA), round(aaPosA+widgetWidth)))
                if widget.ClientRect.Area <= 0:
                    widget.AbsoluteRect = Rect.Rect(0, 0, 0, 0)
            else:
                widget.AbsoluteRect = Rect.Rect(0, 0, 0, 0)

            aaPosA += widgetWidth

class VBox(BoxWidget):
    def __init__(self, parent, **kwargs):
        super(VBox, self).__init__(parent,
            opSetattr("TopBottom"),
            opSetattr("LeftRight"),
            (attrgetter("Top"), attrgetter("Bottom")),
            (attrgetter("Left"), attrgetter("Right")),
            attrgetter("Height"),
            **kwargs)

    def doAlign(self):
        # print("{0} {1}".format(self, self.AbsoluteRect))
        if len(self) == 0:
            return
        myStyle = self.ComputedStyle
        spacingList = self._getSpacingList(self.ComputedStyle.BoxSpacingY)
        x, y, w, h = self.AbsoluteRect.XYWH
        self._doAlign(spacingList, (y, x), h, w, (myStyle.Padding.Left, myStyle.Padding.Right))

class HBox(BoxWidget):
    def __init__(self, parent, **kwargs):
        super(HBox, self).__init__(parent,
            opSetattr("LeftRight"),
            opSetattr("TopBottom"),
            (attrgetter("Left"), attrgetter("Right")),
            (attrgetter("Top"), attrgetter("Bottom")),
            attrgetter("Width"),
            **kwargs)

    def doAlign(self):
        # print("{0} {1}".format(self, self.AbsoluteRect))
        if len(self) == 0:
            return
        myStyle = self.ComputedStyle
        spacingList = self._getSpacingList(self.ComputedStyle.BoxSpacingX)
        rect = copy.copy(self.AbsoluteRect)
        rect.shrink(myStyle.Border.getBox())
        x, y, w, h = rect.XYWH
        self._doAlign(spacingList, (x, y), w, h, (myStyle.Padding.Top, myStyle.Padding.Bottom))

class Grid(ParentWidget):
    pass


CSS.Minilanguage.ElementNames().registerWidgetClass(VBox)
CSS.Minilanguage.ElementNames().registerWidgetClass(HBox)
CSS.Minilanguage.ElementNames().registerWidgetClass(Grid)
