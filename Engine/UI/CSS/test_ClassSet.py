# File name: test_ClassSet.py
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

from ClassSet import ClassSet

class ClassSetInstanceTest(unittest.TestCase):
    def setUp(self):
        self.instance = ClassSet()
    
    def test_init(self):
        self.assertEqual(len(self.instance), 0)

    def test_add(self):
        toAdd = ["testclass"]
        self.assertEqual(self.instance.__iadd__(10), NotImplemented)
        self.instance += toAdd
        self.assertIn(toAdd[0], self.instance)
        self.assertEqual(list(self.instance), toAdd)

    def test_sub(self):
        classes = ["testclass", "moreclass", "anotherclass"]
        self.instance += classes
        self.instance -= classes[0]
        self.assertEqual(list(self.instance), classes[1:])
        self.instance -= classes[1:]
        self.assertEqual(len(self.instance), 0)

    def tearDown(self):
        del self.instance
    
    
