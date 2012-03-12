# File name: test_ShaderParser.py
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

from test_ResourceVFS import *
from ShaderParser import ShaderParser

class ParserTest(TestResourceLoader):
    def tearDown(self):
        super(ParserTest, self).tearDown()

    def parseMultiLineString(self, s):
        return self.parseFileLike(s.split("\n"))

    def parseFileLike(self, fl):
        return ShaderParser(fl).parse()
    
    def test_simple(self):
        variables, vp, fp = self.parseMultiLineString("""[vertex shader]
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
        vertexShader = """void main() {
    // vertex shader goes here
}"""
        fragmentShader = """void main() {
    // fragment shader goes here
}"""
        TestMount["/test.vp"] = vertexShader
        TestMount["/test.fp"] = fragmentShader
        TestMount["/test.shader"] = """[vertex shader]
#include "/data/test.vp"
[fragment shader]
#include "/data/test.fp" """

        variables, vp, fp = self.parseFileLike(TestVFS.open("/data/test.shader", "r"))
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
        vertexShader = """void main() {
#if var
    // vertex shader goes here
#else
    // vertex shader goes not here
#endif
}"""
        fragmentShader = """void main() {
    // fragment shader goes here
}"""
        TestMount["/test.vp"] = vertexShader
        TestMount["/test.fp"] = fragmentShader
        TestMount["/test.shader"] = """
[variables]
var=False

[vertex shader]
#include "/data/test.vp"
[fragment shader]
#include "/data/test.fp" """

        variables, vp, fp = self.parseFileLike(TestVFS.open("/data/test.shader", "r"))
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

    def test_conditionalInclude(self):
        vertexShader1 = """void main() {
    // vertex shader goes here
}"""
        vertexShader2 = """void main() {
    // vertex shader goes not here
}"""
        fragmentShader = """void main() {
    // fragment shader goes here
}"""
        TestMount["/test1.vp"] = vertexShader1
        TestMount["/test2.vp"] = vertexShader2
        TestMount["/test.fp"] = fragmentShader
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

        variables, vp, fp = self.parseFileLike(TestVFS.open("/data/test.shader", "r"))
        self.assertEqual(vertexShader1, vp({"var": True}))
        self.assertEqual(vertexShader2, vp({"var": False}))
        self.assertEqual(fragmentShader, fp({}))
        
        del TestMount["/test1.vp"]
        del TestMount["/test2.vp"]
        del TestMount["/test.fp"]
        del TestMount["/test.shader"]
