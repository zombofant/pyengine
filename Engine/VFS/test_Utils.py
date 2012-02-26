# File name: test_Utils.py
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
import platform

from Utils import absolutify, normalizeVFSPath, validateVFSPath, isWriteFlag

class Absolutify(unittest.TestCase):
    def test_keepsAbsolutePaths(self):
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
    def test_keepsNormalizedPaths(self):
        path = "/some/normalized/path"
        self.assertEqual(path, normalizeVFSPath(path))

    def test_removeTrailing(self):
        path = "/with/trailing/slash/"
        self.assertEqual(path[:-1], normalizeVFSPath(path))

    def test_singleFile(self):
        path_bad = "test.txt"
        path_good = "/test.txt"
        self.assertEqual(path_good, absolutify(path_bad))

class ValidateVFSPath(unittest.TestCase):
    def test_goodPath(self):
        path = "/some/okay/vfs/path"
        try:
            validateVFSPath(path)
        except ValueError:
            self.fail("ValueError was thrown unexpectedly")

    def test_trailingSlashes(self):
        path = "/some/not/okay/path/"
        self.assertRaises(ValueError, validateVFSPath, path)

    def test_relativeRoot(self):
        path = "some/relative/path"
        self.assertRaises(ValueError, validateVFSPath, path)

    def test_dot(self):
        path = "/some/./relative/path"
        self.assertRaises(ValueError, validateVFSPath, path)

    def test_ddot(self):
        path = "/some/../relative/path"
        self.assertRaises(ValueError, validateVFSPath, path)

    def test_sslash(self):
        path = "/some/broken//path"
        self.assertRaises(ValueError, validateVFSPath, path)

class IsWriteFlag(unittest.TestCase):
    def test_r(self):
        self.assertFalse(isWriteFlag("r"))
        self.assertTrue(isWriteFlag("r+"))
        self.assertTrue(isWriteFlag("r+b"))
        self.assertTrue(isWriteFlag("r+bU"))
        self.assertFalse(isWriteFlag("rb"))
        self.assertFalse(isWriteFlag("rbU"))
        self.assertFalse(isWriteFlag("rU"))

    def test_w(self):
        self.assertTrue(isWriteFlag("w"))
        self.assertTrue(isWriteFlag("w+"))
        self.assertTrue(isWriteFlag("w+b"))
        self.assertTrue(isWriteFlag("w+bU"))
        self.assertTrue(isWriteFlag("wb"))
        self.assertTrue(isWriteFlag("wbU"))
        self.assertTrue(isWriteFlag("wU"))

    def test_a(self):
        self.assertTrue(isWriteFlag("a"))
        self.assertTrue(isWriteFlag("a+"))
        self.assertTrue(isWriteFlag("a+b"))
        self.assertTrue(isWriteFlag("a+bU"))
        self.assertTrue(isWriteFlag("ab"))
        self.assertTrue(isWriteFlag("abU"))
        self.assertTrue(isWriteFlag("aU"))
