# File name: test_Parser.py
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
import StringIO

import Selectors
import Rules
import Parser as _Parser

class ParserInstanceTest(unittest.TestCase):
    def _parseCSS(self, src):
        io = StringIO.StringIO(src)
        return self._parser.parse(io)

    def _testRule(self, src, reference):
        parsedRule = self._parseCSS(src)[0]
        self.assertEqual(parsedRule, reference)
        parsedRule = self._parseCSS(unicode(reference))[0]
        self.assertEqual(parsedRule, reference)
    
    def setUp(self):
        self._parser = _Parser.Parser()

    def tearDown(self):
        del self._parser

class ParseSelectors(ParserInstanceTest):
    def test_emptyRule(self):
        self._testRule(
            """
            test {

            }
            """,
            Rules.Rule([Selectors.Is("test")], {})
        )

    def test_invalidSelector(self):
        # FIXME: Make the parser raise a proper exception here
        self.assertRaises(Exception, self._parseCSS, """{}""")

    def test_classes(self):
        self._testRule(
            """
            test.class1.class2 {

            }
            """,
            Rules.Rule([Selectors.HasCSSClasses("class1", "class2", chained=Selectors.Is("test"))], {})
        )

    def test_attributes(self):
        self._testRule(
            """
            test[attr,attr2="value"] {

            }
            """,
            Rules.Rule([Selectors.HasAttributes(("attr",), ("attr2", "value"), chained=Selectors.Is("test"))], {})
        )

    def test_nesting1(self):
        self._testRule(
            """
            test1 > test2 {

            }
            """,
            Rules.Rule([Selectors.DirectChildOf(Selectors.Is("test1"), chained=Selectors.Is("test2"))], {})
        )
        

    def test_nesting2(self):
        self._testRule(
            """
            test1 test2 {

            }
            """,
            Rules.Rule([Selectors.ChildOf(Selectors.Is("test1"), chained=Selectors.Is("test2"))], {})
        )

    def test_complex(self):
        self._testRule(
            """
            test1.class1 > test2[attr2="value2"] test3.class3[attr3] {

            }
            """,
            Rules.Rule([
                Selectors.DirectChildOf(
                    Selectors.HasCSSClasses("class1", chained=Selectors.Is("test1")),
                    chained=Selectors.ChildOf(
                    Selectors.HasAttributes(("attr2", "value2"), chained=Selectors.Is("test2")),
                    chained=Selectors.HasAttributes(("attr3",), chained=Selectors.HasCSSClasses("class3", chained=Selectors.Is("test3")))
                ))
            ], {})
        )
