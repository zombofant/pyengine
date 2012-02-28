# File name: test_Theme.py
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

import unittest

from Style import Style
from Theme import Theme
from Widget import ParentWidget
from Root import RootWidget
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

RootWidget ParentWidget {
    padding: 3;
}

RootWidget > ParentWidget {
    border: 1 solid rgba(0.1, 0.2, 0.3);
}
"""
    
    def setUp(self):
        Minilanguage.elementNames.update({
            "ParentWidget": ParentWidget,
            "RootWidget": RootWidget
        })
        self.theme = Theme()
        self.theme.addRules(Parser().parse(StringIO.StringIO(self.css)))
        self.referenceBorder = Border()
        self.referenceBorder.Right = BorderEdge(1, Colour(0.1, 0.2, 0.3))

    def tearDown(self):
        Minilanguage.elementNames.clear()
        del self.theme

class ThemeCascading(ThemeTest):
    def test_cascade(self):
        root = RootWidget()
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
        root = RootWidget()
        child1 = ParentWidget(root)
        child2 = ParentWidget(child1)
        child2.StyleRule = Rule(
            [],
            [("margin", ("1",))]
        )

        self.assertEqual(self.theme.getWidgetStyle(child2),
            Style(
                background=Transparent,
                padding=Padding(3),
                margin=Margin(0),
                border=self.referenceBorder
            )
        )

    def test_applyStyles(self):
        root = RootWidget()
        child1 = ParentWidget(root)
        child2 = ParentWidget(child1)
        child2.StyleRule = Rule(
            [],
            [("margin", ("1",))]
        )

        self.theme.applyStyles(root)
        
        self.assertEqual(child2.ComputedStyle,
            Style(
                background=Transparent,
                padding=Padding(3),
                margin=Margin(1),
                border=self.referenceBorder
            )
        )
