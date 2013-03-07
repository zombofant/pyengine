# File name: test_Parser.py
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
import StringIO

import Literals
import Selectors
import Rules
import Box
from Fill import Image, Colour, Gradient
import Parser as _Parser
import Minilanguage

class ParserInstanceTest(unittest.TestCase):
    def _parse_css(self, src):
        io = StringIO.StringIO(src)
        return self._parser.parse(io)

    def _test_rule(self, src, reference):
        parsedrule = self._parse_css(src)[0]
        try:
            self.assertEqual(parsedrule, reference)
        except:
            raise

    def setUp(self):
        self._parser = _Parser.Parser()
        Minilanguage.elementnames["test"] = "test"
        Minilanguage.elementnames["test1"] = "test1"
        Minilanguage.elementnames["test2"] = "test2"
        Minilanguage.elementnames["test3"] = "test3"

    def tearDown(self):
        Minilanguage.elementnames.clear()
        del self._parser

class ParseSelectors(ParserInstanceTest):
    def _test_rule(self, src, reference):
        super(ParseSelectors, self)._test_rule(src, reference)
        parsedrule = self._parse_css(unicode(reference))[0]
        self.assertEqual(parsedrule, reference)

    def test_empty_rule(self):
        self._test_rule(
            """
            test {

            }
            """,
            Rules.Rule([Selectors.Is("test")], [])
        )

    def test_comment(self):
        self._test_rule(
            """
/* ignored content */
            test  /* some more ignored content */{
 /* more ignored content
*/
            } /* even more ignored content */
            """,
            Rules.Rule([Selectors.Is("test")], [])
        )

    def test_invalid_selector(self):
        # FIXME: Make the parser raise a proper exception here
        self.assertRaises(Exception, self._parse_css, """{}""")

    def test_classes(self):
        self._test_rule(
            """
            test.class1.class2 {

            }
            """,
            Rules.Rule([Selectors.HasAttributes(
                Selectors.AttributeClass("class1"),
                Selectors.AttributeClass("class2"),
                chained=Selectors.Is("test")
            )], [])
        )

    def test_attributes(self):
        self._test_rule(
            """
            test[attr,attr2="value"] {

            }
            """,
            Rules.Rule([Selectors.HasAttributes(
                Selectors.AttributeExists("attr"),
                Selectors.AttributeValue("attr2", "value"),
                chained=Selectors.Is("test")
            )], [])
        )

    def test_nesting1(self):
        self._test_rule(
            """
            test1 > test2 {

            }
            """,
            Rules.Rule([Selectors.DirectChildOf(
                Selectors.Is("test1"),
                chained=Selectors.Is("test2")
            )], [])
        )


    def test_nesting2(self):
        self._test_rule(
            """
            test1 test2 {

            }
            """,
            Rules.Rule([Selectors.ChildOf(
                Selectors.Is("test1"),
                chained=Selectors.Is("test2")
            )], [])
        )

    def test_state(self):
        self._test_rule(
            """
            test1:hover {

            }
            """,
            Rules.Rule([Selectors.State("hover",
                                        chained=Selectors.Is("test1"))], [])
        )

    def test_complex(self):
        self._test_rule(
            """
            test1.class1 > test2[attr2="value2"] test3.class3[attr3] {

            }
            """,
            Rules.Rule([
                Selectors.DirectChildOf(
                    Selectors.HasAttributes(
                        Selectors.AttributeClass("class1"),
                        chained=Selectors.Is("test1")
                    ),
                    chained=Selectors.ChildOf(
                        Selectors.HasAttributes(
                            Selectors.AttributeValue("attr2", "value2"),
                            chained=Selectors.Is("test2")
                        ),
                        chained=Selectors.HasAttributes(
                            Selectors.AttributeExists("attr3"),
                            Selectors.AttributeClass("class3"),
                            chained=Selectors.Is("test3")
                        )
                    )
                )
            ], [])
        )

class ParseProperties(ParserInstanceTest):
    def _test_rule(self, propsrc, props):
        super(ParseProperties, self)._test_rule(
            """test {{ {0} }}""".format(propsrc),
            Rules.Rule([Selectors.Is("test")], props)
        )

    def _test_box(self, boxprop, boxkw):
        self._test_rule(
            """{0}: 1 2 3 4;""".format(boxprop),
            [(boxkw, (1, 2, 3, 4))]
        )

    def test_background_colour(self):
        self._test_rule(
            """background: rgba(0.1, 0.2, 0.3, 0.4);""",
            [("background", (Colour(0.1, 0.2, 0.3, 0.4),))]
        )

    def test_padding(self):
        self._test_box("padding", "padding")

    def test_margin(self):
        self._test_box("margin", "margin")

    def test_text_align(self):
        self._test_rule(
            """text-align: center;""",
            [("text-align", (Literals.Pango.Alignment.CENTER,))]
        )

    def test_negative_margin(self):
        self._test_rule(
            """margin: -1;""",
            [("margin", (-1,))]
        )
