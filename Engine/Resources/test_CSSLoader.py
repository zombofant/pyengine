# File name: test_CSSLoader.py
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

import Engine.UI.CSS.Minilanguage
from Engine.UI.CSS.Fill import Colour
from Engine.UI.CSS.Selectors import Is
from Engine.UI.CSS.Rules import Rule

from test_ResourceVFS import *
import CSSLoader
import Manager

class CSSLoaderTest(unittest.TestCase):
    def test_load_css(self):
        Engine.UI.CSS.Minilanguage.ElementNames().register_widget_class(int)
        TestMount["/test.css"] = """
int {
    background-color: rgba(1., 0., 0., 1.);
}
""".encode("utf8")
        rules = Manager.ResourceManager().require('/data/test.css')
        self.assertEqual(rules,
            [Rule([Is(int)], [("background-color", (Colour(1., 0., 0., 1.),))])]
        )
        del Engine.UI.CSS.Minilanguage.ElementNames()["int"]

