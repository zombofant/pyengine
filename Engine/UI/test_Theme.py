# File name: test_Theme.py
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

import unittest

from Style import Style, BaseStyle
from Theme import Theme
from WidgetBase import ParentWidget
from RootWidget import RootWidget
from LayerWidget import DesktopLayer
from CSS.Parser import Parser
from CSS.Fill import Transparent, Colour
from CSS.Box import Padding, Margin
from CSS.Rules import Rule
from CSS.Border import Border, BorderEdge
import CSS.Minilanguage as Minilanguage
import StringIO

class ThemeTest(unittest.TestCase):
    css = """
ParentWidget > ParentWidget {
    background: transparent;
    padding: 2;
}

ParentWidget {
    background: rgba(1.0, 1.0, 1.0, 1.0);
    padding: 1;
    border-right: 1 solid rgba(0.1, 0.2, 0.3);
}

DesktopLayer ParentWidget {
    padding: 3;
}

DesktopLayer > ParentWidget {
    background: rgba(1.0, 1.0, 1.0, 1.0);
    border: 1 solid rgba(0.1, 0.2, 0.3);
}

DesktopLayer:hover {
    background: rgba(1.0, 0.0, 0.0, 1.0);
}
"""

    def setUp(self):
        self._old_element_names = dict(Minilanguage.elementNames)
        Minilanguage.elementNames.update({
            "ParentWidget": ParentWidget,
            "DesktopLayer": DesktopLayer,
        })
        self.theme = Theme()
        self.theme.addRules(Parser().parse(StringIO.StringIO(self.css)))
        self.referenceBorder = Border()
        self.referenceBorder.Right = BorderEdge(1, Colour(0.1, 0.2, 0.3))

    def tearDown(self):
        Minilanguage.elementNames.clear()
        Minilanguage.elementNames.update(self._old_element_names)
        del self.theme

class ThemeCascading(ThemeTest):
    def test_cascade(self):
        rootW = RootWidget()
        root = rootW._desktopLayer
        rootW.Theme = self.theme
        child1 = ParentWidget(root)
        child2 = ParentWidget(child1)


        self.assertEqual(self.theme.getWidgetStyle(child2),
            Style(
                background=Transparent,
                padding=Padding(3),
                border=self.referenceBorder
            )
        )
        self.assertEqual(self.theme.getWidgetStyle(child1),
            Style(
                background=Colour(1., 1., 1., 1.),
                padding=Padding(3),
                border=Border(1, Colour(0.1, 0.2, 0.3))
            )
        )

    def test_customStyle(self):
        rootW = RootWidget()
        root = rootW._desktopLayer
        rootW.Theme = self.theme
        child1 = ParentWidget(root)
        child2 = ParentWidget(child1)
        child2.StyleRule = Rule(
            [],
            [("margin", ("1",))]
        )

        refStyle = Style(
            background=Transparent,
            padding=Padding(3),
            margin=Margin(1),
            border=self.referenceBorder
            )
        refStyle.solveInheritance(child1.ComputedStyle)
        self.assertEqual(
            child2.ComputedStyle,
            refStyle
        )

    def test_applyStyles(self):
        rootW = RootWidget()
        root = rootW._desktopLayer
        rootW.Theme = self.theme
        child1 = ParentWidget(root)
        child2 = ParentWidget(child1)
        child2.StyleRule = Rule(
            [],
            [("margin", ("1",))]
        )

        refStyle = Style(
            background=Transparent,
            padding=Padding(3),
            margin=Margin(1),
            border=self.referenceBorder
            )
        refStyle.solveInheritance(child1.ComputedStyle)
        self.assertEqual(
            child2.ComputedStyle,
            refStyle
        )

class States(ThemeTest):
    def test_states(self):
        rootW = RootWidget()
        root = rootW._desktopLayer
        rootW.Theme = self.theme
        refStyle = Style(
            background=Colour(1., 1., 1., 1.),
            padding=Padding(1),
            border=self.referenceBorder
            )
        refStyle.solveInheritance(BaseStyle())
        self.assertEqual(root.ComputedStyle, refStyle)
        root._isHovered = True
        root._invalidateComputedStyle()
        refStyle = Style(
            padding=Padding(1),
            border=self.referenceBorder,
            background=Colour(1., 0., 0., 1.)
            )
        refStyle.solveInheritance(BaseStyle())
        self.assertEqual(root.ComputedStyle, refStyle)
