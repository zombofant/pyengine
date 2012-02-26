# encoding=utf8
# File name: test_TextLoader.py
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
from TextLoader import TextLoader

class TextLoaderTest(unittest.TestCase):
    def _encodedDataIterable(self, encoding):
        return (line.encode(encoding) for line in self.data.split("\n"))

class TextLoaderInitTest(unittest.TestCase):
    def test_init(self):
        data = """foo bar
this is a blindtext"""
        instance = TextLoader()
        result = instance.load(data.split("\n"))
        self.assertEqual(result, data)
        del instance
        
class TextLoaderEncodingTest(TextLoaderTest):
    def setUp(self):
        self.data = "✔"

    def test_encodingUTF8(self):
        instance = TextLoader()
        result = instance.load(self._encodedDataIterable("utf8"))
        self.assertEqual(result, self.data)
        del instance

    def test_decodeError(self):
        instance = TextLoader()
        self.assertRaises(UnicodeDecodeError, instance.load, self._encodedDataIterable("utf8"), None, encoding="ascii")
        del instance

    def tearDown(self):
        del self.data
