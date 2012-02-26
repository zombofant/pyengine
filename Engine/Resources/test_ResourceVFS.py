# File name: test_ResourceVFS.py
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

from Engine.VFS.FileSystem import FileSystem, MountPriority
from Engine.VFS.Mounts import MountVirtual
import Manager

__all__ = ["TestVFS", "TestMountPoint", "TestMount", "TestTestVFS"]

"""
This module does not implement any tests by itself. It rather sets the
VFS up for usage with the ResourceManager and also assigns the VFS to
the resource manager.
"""

TestVFS = FileSystem()
"""
The VFS used for testing.
"""

TestMountPoint = "/data"
"""
The mount point under which the virtual mount is accessible. This is
only for reference; changing this will not change the mount location.
"""

TestMount = MountVirtual()
"""
The mount object which can be used to create files in the test VFS.
"""

TestVFS.mount(TestMountPoint, TestMount, MountPriority.FileSystem)
Manager.ResourceManager(TestVFS)

class TestTestVFS(unittest.TestCase):
    def test_vfs(self):
        self.assertIs(Manager.ResourceManager()._fileSystem, TestVFS)

    def test_mount(self):
        testFile = "/test.txt"
        TestMount[testFile] = "data"
        self.assertTrue(TestVFS.fileReadable(TestMountPoint + testFile))
        del TestMount[testFile]
