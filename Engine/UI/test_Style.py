# File name: test_Style.py
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
import copy

from CSS.Box import Padding
from CSS.Fill import Colour
from CSS.Rules import Rule

from Style import Style

class StyleIdentity(unittest.TestCase):
    def setUp(self):
        self.style = Style()
        self.border = self.style.Border
        self.padding = self.style.Padding
        self.margin = self.style.Margin

    def test_border(self):
        rule = Rule([], [
            ("border", ("1", "solid", Colour()))
        ])
        self.style._addRule(rule)
        self.assertIs(self.border, self.style.Border)

    def test_padding(self):
        rule = Rule([], [
            ("padding", ("1",)),
            ("padding-left", ("2",))
        ])
        self.style._addRule(rule)
        self.assertIs(self.padding, self.style.Padding)


    def test_deepcopy(self):
        rule = Rule([], [
            ("padding", ("1",)),
            ("padding-left", ("2",)),
            ("border", ("1", "solid", Colour())),
            ("background", (Colour(),)),
            ("color", (Colour(1., 0., 0., 1.),)),
            ("margin", ("1",)),
            ("flex", ("2",)),
            ("box-spacing", ("4",))
        ])
        self.style._addRule(rule)
        self.assertEqual(self.style, copy.deepcopy(self.style))

    def tearDown(self):
        del self.style


class StyleRules(unittest.TestCase):
    def setUp(self):
        self.style = Style()

    def tearDown(self):
        del self.style

    def test_boxSpacing(self):
        self.assertEqual(self.style.BoxSpacingX, 0)
        self.assertEqual(self.style.BoxSpacingY, 0)
        
        rule = Rule([], [
            ("box-spacing", ("1",))
        ])
        self.style._addRule(rule)
        self.assertEqual(self.style.BoxSpacingX, 1)
        self.assertEqual(self.style.BoxSpacingY, 1)
        
        rule = Rule([], [
            ("box-spacing-y", ("2",))
        ])
        self.style._addRule(rule)
        self.assertEqual(self.style.BoxSpacingX, 1)
        self.assertEqual(self.style.BoxSpacingY, 2)
        
        rule = Rule([], [
            ("box-spacing-x", ("2",))
        ])
        self.style._addRule(rule)
        self.assertEqual(self.style.BoxSpacingX, 2)
        self.assertEqual(self.style.BoxSpacingY, 2)

    def test_flex(self):
        self.assertEqual(self.style.Flex, 1)

        rule = Rule([], [
            ("flex", ("2",))
        ])
        self.style._addRule(rule)
        self.assertEqual(self.style.Flex, 2)


    def test_border(self):
        rule = Rule([], [
            ("border", ("1", "solid", Colour()))
        ])
        self.style._addRule(rule)
        self.assertEqual(self.style.Border.Left.Width, 1)

    def test_borderCorner(self):
        rule = Rule([], [
            ("border-left", ("1", "solid", Colour())),
            ("border-radius", ("2",)),
            ("border-top-left-radius", ("8",))
        ])
        self.style._addRule(rule)
        self.assertEqual(self.style.Border.TopRightRadius, 2)
        self.assertEqual(self.style.Border.BottomLeftRadius, 2)
        self.assertEqual(self.style.Border.BottomRightRadius, 2)
        self.assertEqual(self.style.Border.TopLeftRadius, 8)
        self.assertEqual(copy.deepcopy(self.style).Border.TopLeftRadius, 8)
        
    def test_padding(self):
        rule = Rule([], [
            ("padding", ("1",)),
            ("padding-left", ("2",))
        ])
        self.style._addRule(rule)
        self.assertEqual(self.style.Padding, Padding(2, 1, 1, 1))
        
    def test_background(self):
        rule = Rule([], [
            ("background", (Colour(1., 0., 1., 1.),)),
        ])
        self.style._addRule(rule)
        self.assertEqual(self.style.Background, Colour(1, 0, 1, 1))
        
    def test_dimensions(self):
        self.assertEqual(self.style.Width, None)
        self.assertEqual(self.style.Height, None)
        
        rule = Rule([], [
            ("width", ("100",)),
        ])
        self.style._addRule(rule)
        self.assertEqual(self.style.Width, 100)
