# File name: test_FileSystem.py
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
from FileSystem import MountPriority, MountDict, FileSystem

class MountPriorities(unittest.TestCase):
    def test_priorities(self):
        self.assertGreater(MountPriority.Discriminated, MountPriority.PractiallyInexistant)
        self.assertGreater(MountPriority.Fallback, MountPriority.Discriminated)
        self.assertGreater(MountPriority.FileSystem, MountPriority.Fallback)
        self.assertGreater(MountPriority.Important, MountPriority.FileSystem)
        self.assertGreater(MountPriority.Override, MountPriority.Important)
        self.assertGreater(MountPriority.Penetrant, MountPriority.Override)

    def test_list_order(self):
        dict, list = MountDict()
        prev_key, _ = list[0]
        for key, value in list[1:]:
            self.assertLess(key, prev_key)

    def test_dict(self):
        dict, list = MountDict()
        for key, value in list:
            self.assertIs(dict[key], value)

class FileSystemInstanceTest(unittest.TestCase):
    def setUp(self):
        self.instance = FileSystem()

    def tearDown(self):
        del self.instance

class FileSystemMount(FileSystemInstanceTest):
    def test_type_check(self):
        self.assertRaises(TypeError, self.instance.mount, "/", int, MountPriority.FileSystem)
