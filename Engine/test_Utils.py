# File name: test_Utils.py
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

from Utils import lineNumbering, indent

class LineNumbering(unittest.TestCase):
    def test_string(self):
        self.assertMultiLineEqual(lineNumbering("""test
test2
test3"""), """1: test
2: test2
3: test3""")

    def test_lines(self):
        self.assertMultiLineEqual(lineNumbering(["test", "test2", "test3"]), """1: test
2: test2
3: test3""")

class Indent(unittest.TestCase):
    def test_string(self):
        self.assertMultiLineEqual("""  test
  test2
  test3""", indent("""test
test2
test3""", "  "))
