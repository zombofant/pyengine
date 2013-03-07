# File name: test_ShaderParser.py
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

from test_ResourceVFS import *
from ShaderParser import ShaderParser

class ParserTest(TestResourceLoader):
    def tearDown(self):
        super(ParserTest, self).tearDown()

    def parse_multi_line_string(self, s):
        return self.parse_file_like(s.split("\n"))

    def parse_file_like(self, fl):
        return ShaderParser(fl).parse()
    
    def test_simple(self):
        variables, vp, fp = self.parse_multi_line_string("""[vertex shader]
void main() {
    // vertex shader goes here
}
[fragment shader]
void main() {
    // fragment shader goes here
}""")
        self.assertEqual("""void main() {
    // vertex shader goes here
}""", vp({}))
        self.assertEqual("""void main() {
    // fragment shader goes here
}""", fp({}))
        self.assertEqual(0, len(variables))

    def test_include(self):
        vertex_shader = """void main() {
    // vertex shader goes here
}"""
        fragment_shader = """void main() {
    // fragment shader goes here
}"""
        TestMount["/test.vp"] = vertex_shader
        TestMount["/test.fp"] = fragment_shader
        TestMount["/test.shader"] = """[vertex shader]
#include "/data/test.vp"
[fragment shader]
#include "/data/test.fp" """

        variables, vp, fp = self.parse_file_like(TestVFS.open("/data/test.shader", "r"))
        self.assertEqual("""void main() {
    // vertex shader goes here
}""", vp({}))
        self.assertEqual("""void main() {
    // fragment shader goes here
}""", fp({}))
        
        del TestMount["/test.vp"]
        del TestMount["/test.fp"]
        del TestMount["/test.shader"]

    def test_conditional(self):
        vertex_shader = """void main() {
#if var
    // vertex shader goes here
#else
    // vertex shader goes not here
#endif
}"""
        fragment_shader = """void main() {
    // fragment shader goes here
}"""
        TestMount["/test.vp"] = vertex_shader
        TestMount["/test.fp"] = fragment_shader
        TestMount["/test.shader"] = """
[variables]
var=False

[vertex shader]
#include "/data/test.vp"
[fragment shader]
#include "/data/test.fp" """

        variables, vp, fp = self.parse_file_like(TestVFS.open("/data/test.shader", "r"))
        self.assertEqual("""void main() {
    // vertex shader goes not here
}""", vp({"var": False}))
        self.assertEqual("""void main() {
    // vertex shader goes here
}""", vp({"var": True}))
        self.assertEqual("""void main() {
    // fragment shader goes here
}""", fp({}))
        
        del TestMount["/test.vp"]
        del TestMount["/test.fp"]
        del TestMount["/test.shader"]

    def test_conditional_include(self):
        vertex_shader1 = """void main() {
    // vertex shader goes here
}"""
        vertex_shader2 = """void main() {
    // vertex shader goes not here
}"""
        fragment_shader = """void main() {
    // fragment shader goes here
}"""
        TestMount["/test1.vp"] = vertex_shader1
        TestMount["/test2.vp"] = vertex_shader2
        TestMount["/test.fp"] = fragment_shader
        TestMount["/test.shader"] = """
[variables]
var=False

[vertex shader]
#if var
#include "/data/test1.vp"
#else
#include "/data/test2.vp"
#endif
[fragment shader]
#include "/data/test.fp" """

        variables, vp, fp = self.parse_file_like(TestVFS.open("/data/test.shader", "r"))
        self.assertEqual(vertex_shader1, vp({"var": True}))
        self.assertEqual(vertex_shader2, vp({"var": False}))
        self.assertEqual(fragment_shader, fp({}))
        
        del TestMount["/test1.vp"]
        del TestMount["/test2.vp"]
        del TestMount["/test.fp"]
        del TestMount["/test.shader"]
