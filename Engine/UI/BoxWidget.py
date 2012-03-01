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
import operator
import functools

import CSS.Minilanguage

from WidgetBase import ParentWidget

def opSetattr(attr):
    def _setattr(obj, val):
        setattr(obj, attr, val)
    return _setattr

class BoxWidget(ParentWidget):
    def _getSpacingList(self, baseSpacing, getterA, getterB):
        myStyle = self.ComputedStyle
        widgets = list(self)
        leftSpaceIterator = itertools.chain(
            (getterA(myStyle.Padding),),
            (getterA(widget.ComputedStyle.Margin) for widget in widgets[:-1])
        )
        widgetsLeftSpace = [(widget, max(getterA(widget.ComputedStyle.Margin), baseSpacing, leftSpace)) for widget, leftSpace in zip(widgets, leftSpaceIterator)]
        widgetsLeftSpace.append((None, max(getterB(myStyle.Padding), baseSpacing, getterB(widgets[-1].ComputedStyle.Margin))))
        return widgetsLeftSpace

    def _doAlign(self, spacingList, sizeGetter, positionSetter,
            sizeSetter, otherSpacingA, otherSpacingB,
            otherPositionASetter, otherPositionBSetter):
        totalSpace = sum(space for widget, space in spacingList)
        widgetWidth = (sizeGetter(self.AbsoluteRect) - totalSpace) / len(self)
        if int(widgetWidth) <= 0:
            for widget in self:
                widget.Visible = False
            # TODO: Print this warning to a log
            print("Warning: {0} has not enough space for {1} widgets (totalSpace={2},size={3}).".format(self, len(self), totalSpace, sizeGetter(self.AbsoluteRect)))
            return
        
        x = spacingList[0][1] + self.AbsoluteRect.X
        y = otherSpacingA + self.AbsoluteRect.Y
        b = self.AbsoluteRect.Bottom - otherSpacingB
        for widget, space in spacingList[:-1]:
            positionSetter(widget.AbsoluteRect, x)
            sizeSetter(widget.AbsoluteRect, widgetWidth)
            otherPositionASetter(widget.AbsoluteRect, y)
            otherPositionBSetter(widget.AbsoluteRect, b)
            widget.Visible = True
            x += space + widgetWidth

class VBox(BoxWidget):
    def doAlign(self):
        myStyle = self.ComputedStyle
        spacingList = self._getSpacingList(self.ComputedStyle.BoxSpacingX, operator.attrgetter("Top"), operator.attrgetter("Bottom"))
        self._doAlign(spacingList,
            operator.attrgetter("Height"),
            opSetattr("Y"),
            opSetattr("Height"),
            myStyle.Padding.Left, myStyle.Padding.Right, opSetattr("Left"), opSetattr("Right"))

class HBox(BoxWidget):
    def doAlign(self):
        spacingList = self._getSpacingList(self.ComputedStyle.BoxSpacingX, operator.attrgetter("Left"), operator.attrgetter("Right"))
        self._doAlign(spacingList, operator.attrgetter("Width"), opSetattr("X"), opSetattr("Width"),
            myStyle.Padding.Top, myStyle.Padding.Bottom, opSetattr("Top"), opSetattr("Bottom"))

class Grid(ParentWidget):
    pass


CSS.Minilanguage.ElementNames().registerWidgetClass(VBox)
CSS.Minilanguage.ElementNames().registerWidgetClass(HBox)
CSS.Minilanguage.ElementNames().registerWidgetClass(Grid)