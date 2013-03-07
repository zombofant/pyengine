# encoding=utf8
# File name: test_Mounts.py
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

import os
import unittest

import Mounts
import Errors

class AbstractMount(unittest.TestCase):
    def test_interface(self):
        mount = Mounts.Mount()
        self.assertEqual(mount.get_real_path(""), None)
        self.assertRaises(NotImplementedError, mount.file_readable, "")
        self.assertRaises(NotImplementedError, mount.file_writable, "")
        self.assertRaises(NotImplementedError, mount.open, "", "r")
        del mount


class MountDirectoryEquivalence(unittest.TestCase):
    def _translate_module_name(self):
        return os.path.join(*__name__.split(".")) + ".py"
    
    def setUp(self):
        self.instance = Mounts.MountDirectory(os.getcwd(), read_only=False)
        self.module_path = self._translate_module_name()
        self.real_path = os.path.join(os.getcwd(), self.module_path)

    def test_init(self):
        self.assertFalse(self.instance.ReadOnly)

    def test_path(self):
        self.assertEqual(self.instance.get_real_path(self.module_path), self.real_path)

    def test_open(self):
        self.assertIsInstance(self.instance.open(self.module_path, "r"), file)

    def test_open_noent(self):
        self.assertRaises(Errors.VFSFileNotFoundError, self.instance.open, "/some/bogus/path/to/a/bogus/file.bogus", "r")

    def test_readable(self):
        self.assertEqual(os.access(self.real_path, os.R_OK), self.instance.file_readable(self.module_path))

    def test_writable(self):
        self.assertEqual(os.access(self.real_path, os.W_OK), self.instance.file_writable(self.module_path))

    def tearDown(self):
        del self.instance
        del self.module_path
        del self.real_path


class MountDirectoryReadOnly(MountDirectoryEquivalence):
    def setUp(self):
        super(MountDirectoryReadOnly, self).setUp()
        self.instance = Mounts.MountDirectory(os.getcwd(), read_only=True)

    def test_init(self):
        self.assertTrue(self.instance.ReadOnly)

    def test_writable(self):
        self.assertFalse(self.instance.file_writable(self.module_path))

    def test_openw(self):
        self.assertRaises(Errors.VFSPermissionDeniedError, self.instance.open, self.module_path, "w")


class MountVirtual(unittest.TestCase):
    def setUp(self):
        self.instance = Mounts.MountVirtual()

    def test_read_only(self):
        path = "/test.txt"
        data = """Meow"""
        self.assertTrue(self.instance.ReadOnly)
        self.instance[path] = data
        self.assertRaises(Errors.VFSPermissionDeniedError, self.instance.open, path, "w")

    def test_open_noent(self):
        self.assertRaises(Errors.VFSFileNotFoundError, self.instance.open, "/anypath.txt", "r")

    def test_add(self):
        path = "/test.txt"
        data = """Meow"""
        self.assertFalse(self.instance.file_readable(path))
        self.instance[path] = data
        self.assertTrue(self.instance.file_readable(path))
        self.assertEqual(self.instance.open(path, "r").read(), data)

    def test_add_ASCII(self):
        path = "/test.txt"
        data = b"""Meow"""
        self.instance[path] = data
        self.assertTrue(self.instance.file_readable(path))
        self.assertEqual(str(self.instance.open(path, "r").read()), data)

    def test_add_latin1(self):
        path = "/test.txt"
        data = """Meöw""".encode("latin1")
        self.assertRaises(UnicodeDecodeError, self.instance.__setitem__, path, data)

    def test_add_UTF8(self):
        path = "/test.txt"
        data = """Meöw""".encode("utf8")
        self.assertRaises(UnicodeDecodeError, self.instance.__setitem__, path, data)
    
    def tearDown(self):
        del self.instance
