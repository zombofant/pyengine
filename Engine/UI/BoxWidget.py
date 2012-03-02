# File name: BoxWidget.py
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

__all__ = ["VBox", "HBox", "Grid"]

import itertools
from operator import attrgetter
import functools

import CSS.Minilanguage

from WidgetBase import ParentWidget

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
            **kwargs):
        super(BoxWidget, self).__init__(parent, **kwargs)
        self._aaPositionSetter = aaPositionSetter # e.g. opSetattr("LeftRight")
        self._faPositionSetter = faPositionSetter # e.g. opSetattr("TopBottom")
        self._aaBoxEdges = aaBoxEdges # e.g. attrgetter("Left"), attrgetter("Right")
        self._faBoxEdges = faBoxEdges # e.g. attrgetter("Top"), attrgetter("Bottom")
        self._aaSizeGetter = aaSizeGetter # e.g. attrgetter("Width")
    
    def _getSpacingList(self, baseSpacing):
        getterA, getterB = self._aaBoxEdges
        myStyle = self.ComputedStyle
        widgets = list(self)
        leftSpaceIterator = itertools.chain(
            (getterA(myStyle.Padding),),
            (getterA(widget.ComputedStyle.Margin) for widget in widgets[:-1])
        )
        widgetsLeftSpace = [(widget, max(getterA(widget.ComputedStyle.Margin), baseSpacing, leftSpace), widget.ComputedStyle.Flex, self._aaSizeGetter(widget.ComputedStyle)) for widget, leftSpace in zip(widgets, leftSpaceIterator)]
        widgetsLeftSpace.append((None, max(getterB(myStyle.Padding), baseSpacing, getterB(widgets[-1].ComputedStyle.Margin)), 0, None))
        return widgetsLeftSpace

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
        
        totalSpace, totalFlex, totalStaticWidth = BoxWidget.getSpaceFlexWidth(spacingList)
        widgetWidthPerFlex = (myAASize - (totalSpace+totalStaticWidth)) / totalFlex
        
        if int(widgetWidthPerFlex) <= 0:
            for widget in self:
                widget.Visible = False
            # TODO: Print this warning to a log
            print("Warning: {0} has not enough space for {1} flexes (totalSpace={2},size={3},staticSize={4}).".format(self, totalFlex, totalSpace, myAASize, totalStaticWidth))
            return
        
        aaPosA = spacingList[0][1] + myAAFAPos[0]
        faPosA = myAAFAPos[1]
        faPosB = faPosA + myFASize
        
        for widget, space, flex, width in spacingList[:-1]:
            #print("{0} aligning {1}:".format(self, widget))
            #print("  aaRange = ({0}, {1})".format(aaPosA, aaPosB))
            #print("  faRange = ({0}, {1})".format(faPosA, faPosB))
            #print("  aaWidgetSize = {0}".format(widgetAAWidth))

            if width is None:
                widgetWidth = flex * widgetWidthPerFlex
            else:
                widgetWidth = width
            widgetMargin = widget.ComputedStyle.Margin
            aaPositionSetter(widget.AbsoluteRect, (round(aaPosA), round(aaPosA+widgetWidth)))
            
            faSpacingA = max(myFAPadding[0], faBoxEdgeA(widgetMargin))
            faSpacingB = max(myFAPadding[1], faBoxEdgeB(widgetMargin))
            faPositionSetter(widget.AbsoluteRect, (faPosA + faSpacingA, faPosB - faSpacingB))
            
            widget.Visible = True
            offset = space + widgetWidth
            aaPosA += offset

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
        x, y, w, h = self.AbsoluteRect.XYWH
        self._doAlign(spacingList, (x, y), w, h, (myStyle.Padding.Top, myStyle.Padding.Bottom))

class Grid(ParentWidget):
    pass


CSS.Minilanguage.ElementNames().registerWidgetClass(VBox)
CSS.Minilanguage.ElementNames().registerWidgetClass(HBox)
CSS.Minilanguage.ElementNames().registerWidgetClass(Grid)
