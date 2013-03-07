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
import platform

from Utils import absolutify, normalize_vfs_path, validate_vfs_path, is_write_flag, join, splitext

class Absolutify(unittest.TestCase):
    def test_keeps_absolute_paths(self):
        path = "/some/test/path"
        self.assertEqual(path, absolutify(path))

    def test_dot(self):
        path_abs = "/some/test/path"
        path_rel = "/./some/./test/./path/."
        self.assertEqual(path_abs, absolutify(path_rel))

    def test_ddot(self):
        path_abs = "/some/other/test/path"
        path_rel = "/some/test/../other/test/../test/path"
        self.assertEqual(path_abs, absolutify(path_rel))

class NormalizeVFSPath(unittest.TestCase):
    def test_keeps_normalized_paths(self):
        path = "/some/normalized/path"
        self.assertEqual(path, normalize_vfs_path(path))

    def test_remove_trailing(self):
        path = "/with/trailing/slash/"
        self.assertEqual(path[:-1], normalize_vfs_path(path))

    def test_single_file(self):
        path_bad = "test.txt"
        path_good = "/test.txt"
        self.assertEqual(path_good, absolutify(path_bad))

class ValidateVFSPath(unittest.TestCase):
    def test_good_path(self):
        path = "/some/okay/vfs/path"
        try:
            validate_vfs_path(path)
        except ValueError:
            self.fail("ValueError was thrown unexpectedly")

    def test_trailing_slashes(self):
        path = "/some/not/okay/path/"
        self.assertRaises(ValueError, validate_vfs_path, path)

    def test_relative_root(self):
        path = "some/relative/path"
        self.assertRaises(ValueError, validate_vfs_path, path)

    def test_dot(self):
        path = "/some/./relative/path"
        self.assertRaises(ValueError, validate_vfs_path, path)

    def test_ddot(self):
        path = "/some/../relative/path"
        self.assertRaises(ValueError, validate_vfs_path, path)

    def test_sslash(self):
        path = "/some/broken//path"
        self.assertRaises(ValueError, validate_vfs_path, path)

class IsWriteFlag(unittest.TestCase):
    def test_r(self):
        self.assertFalse(is_write_flag("r"))
        self.assertTrue(is_write_flag("r+"))
        self.assertTrue(is_write_flag("r+b"))
        self.assertTrue(is_write_flag("r+bU"))
        self.assertFalse(is_write_flag("rb"))
        self.assertFalse(is_write_flag("rbU"))
        self.assertFalse(is_write_flag("rU"))

    def test_w(self):
        self.assertTrue(is_write_flag("w"))
        self.assertTrue(is_write_flag("w+"))
        self.assertTrue(is_write_flag("w+b"))
        self.assertTrue(is_write_flag("w+bU"))
        self.assertTrue(is_write_flag("wb"))
        self.assertTrue(is_write_flag("wbU"))
        self.assertTrue(is_write_flag("wU"))

    def test_a(self):
        self.assertTrue(is_write_flag("a"))
        self.assertTrue(is_write_flag("a+"))
        self.assertTrue(is_write_flag("a+b"))
        self.assertTrue(is_write_flag("a+bU"))
        self.assertTrue(is_write_flag("ab"))
        self.assertTrue(is_write_flag("abU"))
        self.assertTrue(is_write_flag("aU"))

class Join(unittest.TestCase):
    def test_one_root(self):
        self.assertEqual("/some/test/path", join("/some/test", "path"))
        self.assertEqual("/some/longer/test/path", join("/some/longer", "test", "path"))
        self.assertEqual("keeps/non/trailing/slashes", join("keeps", "non", "trailing", "slashes"))

    def test_multiple_roots(self):
        self.assertEqual("/some/test/path", join("garbage/path", "/some", "test", "path"))

class SplitExt(unittest.TestCase):
    def test_simple(self):
        self.assertEqual(("/root/path/test", "txt"), splitext("/root/path/test.txt"))
        self.assertEqual(("root/path/test", "txt"), splitext("root/path/test.txt"))

    def test_dot_in_path(self):
        self.assertEqual(("/root/path.git/test", ""), splitext("/root/path.git/test"))
        self.assertEqual(("root/path.git/test", ""), splitext("root/path.git/test"))
