# File name: test_Mounts.py
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
from Mounts import Mount, MountDirectory
import os

class AbstractMount(unittest.TestCase):
    def test_interface(self):
        mount = Mount()
        self.assertEqual(mount.getRealPath(""), None)
        self.assertRaises(NotImplementedError, mount.fileReadable, "")
        self.assertRaises(NotImplementedError, mount.fileWritable, "")
        self.assertRaises(NotImplementedError, mount.open, "", "r")
        del mount

class MountDirectoryEquivalence(unittest.TestCase):
    def _translateModuleName(self):
        return os.path.join(*__name__.split(".")) + ".py"
    
    def setUp(self):
        self.instance = MountDirectory(os.getcwd(), readOnly=False)
        self.modulePath = self._translateModuleName()
        self.realPath = os.path.join(os.getcwd(), self.modulePath)

    def test_init(self):
        self.assertFalse(self.instance.ReadOnly)

    def test_path(self):
        self.assertEqual(self.instance.getRealPath(self.modulePath), self.realPath)

    def test_open(self):
        self.assertIsInstance(self.instance.open(self.modulePath, "r"), file)

    def test_readable(self):
        self.assertEqual(os.access(self.realPath, os.R_OK), self.instance.fileReadable(self.modulePath))

    def test_writable(self):
        self.assertEqual(os.access(self.realPath, os.W_OK), self.instance.fileWritable(self.modulePath))

    def tearDown(self):
        del self.instance
        del self.modulePath
        del self.realPath

class MountDirectoryReadOnly(MountDirectoryEquivalence):
    def setUp(self):
        super(MountDirectoryReadOnly, self).setUp()
        self.instance = MountDirectory(os.getcwd(), readOnly=True)

    def test_init(self):
        self.assertTrue(self.instance.ReadOnly)

    def test_writable(self):
        self.assertFalse(self.instance.fileWritable(self.modulePath))

    def test_openw(self):
        self.assertRaises(IOError, self.instance.open, self.modulePath, "w")
        
