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
        self.assertEqual(self.border.Left.Width, 1)

    def test_padding(self):
        rule = Rule([], [
            ("padding", ("1",)),
            ("padding-left", ("2",))
        ])
        self.style._addRule(rule)
        self.assertIs(self.padding, self.style.Padding)
        self.assertEqual(self.style.Padding, Padding(2, 1, 1, 1))

    def tearDown(self):
        del self.style
