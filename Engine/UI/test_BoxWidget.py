# File name: test_BoxWidget.py
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

from CSS.Rect import Rect

from WidgetBase import Widget
from RootWidget import RootWidget
from BoxWidget import VBox, HBox

from test_Widget import WidgetInstanceTest

class VBoxAlignment(WidgetInstanceTest):
    def setUp(self):
        super(VBoxAlignment, self).setUp()
        self.instance.AbsoluteRect = Rect(0, 0, 256, 256)
        self.root = self.instance
        self.instance = VBox(self.root)
        self.instance.AbsoluteRect = Rect(0, 0, 256, 256)

    def tearDown(self):
        del self.instance
        del self.root
    
    def test_basic(self):
        self.widgetA, self.widgetB = Widget(self.instance), Widget(self.instance)
        self.root.realign()
        self.assertEqual(self.widgetA.AbsoluteRect, Rect(0, 0, 256, 128))
        self.assertEqual(self.widgetB.AbsoluteRect, Rect(0, 128, 256, 256))
