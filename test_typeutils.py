# File name: test_typeutils.py
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

from typeutils import number

class TypeUtils(unittest.TestCase):
    def test_number(self):
        self.assertEqual(number("10"), 10)
        self.assertEqual(number("10.2"), 10.2)
        self.assertEqual(number(10), 10)
        self.assertEqual(number(10.0), 10.0)
        self.assertIsInstance(number("10"), (int, long))
        self.assertIsInstance(number("10.2"), float)
        self.assertRaises(ValueError, number, "foobar")
        self.assertRaises(TypeError, number, object())
        
