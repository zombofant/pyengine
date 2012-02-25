# File name: FileSystem.py
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

class MountPriority(object):
    def __new__(cls):
        raise NotImplementedError("Not instanciatable")
    
    PractiallyInexistant = -3
    Discriminated = -2
    Fallback = -1
    FileSystem = 0
    Important = 1
    Override = 2
    Penetrant = 3

def MountDict():
    values = sorted((value for key, value in MountPriority.__dict__.iteritems() if not key.startswith("__")), reverse=True)
    l = [(value, []) for value in values]
    d = dict(l)
    return (d, l)

class Mount(object):
    def __init__(self, **kwargs):
        super(Mount, self).__init__(**kwargs)

    def getRealPath(self, file):
        return None
    
    def fileReadable(self, file):
        raise NotImplementedError("Mount.fileReadable not specified")

    def fileWritable(self, file):
        raise NotImplementedError("Mount.fileWritable not specified")

    def open(self, file, flag):
        raise NotImplementedError("Mount.open not specified")

class FileSystem(object):
    def __init__(self, **kwargs):
        super(FileSystem, self).__init__(**kwargs)
        self._mountDict, self._mounts = MountDict()

    def _normalizePath(self, path):
        path = path.replace('\\', '/')
        if len(path) > 1 and path[-1] == '/':
            path = path[:-1]
        return path

    def _checkPath(self, path):
        nodes = path.split('/')
        if len(nodes[0]) > 0:
            raise ValueError("Paths must start with /")
        for node in nodes[1:]:
            if node == "." or node == "..":
                raise ValueError("Relative paths are not allowed")
            if len(node) == 0:
                raise ValueError("// must not occur in a path")

    def _findMount(self, mount):
        for key, value in self._mounts:
            for i, (path, obj) in enumerate(value):
                if obj is mount:
                    return (key, i, path, obj)
        return None

    def _findMountPoint(self, path):
        for key, value in self._mounts:
            for i, (path, obj) in enumerate(value):
                if path == path:
                    return (key, i, path, obj)
        return None

    def _findFile(self, filePath):
        for key, value in self._mounts:
            for path, obj in value:
                if filePath.startswith(path):
                    if path == filePath:
                        continue
                    return (path, filePath[len(path):], obj)
        return None

    def _sortMounts(self):
        for key, value in self._mounts:
            value.sort(key=lambda x: len(x[0]))

    def _getFileMount(self, path):
        path = self._normalizePath(path)
        self._checkPath(path)
        mountPoint, subPath, mount = self._findFile(path)
        return mount, subPath

    def mount(self, mountPoint, mountObject, priority):
        if not isinstance(mountObject, Mount):
            raise TypeError("Expected Mount, got {0}".format(type(mountObject)))
        existing = self._findMount(mountObject)
        if existing is not None:
            raise ValueError("Mount {0} is already mounted at {1} with priority {2}".format(existing[3], existing[2], existing[0]))
        path = self._normalizePath(mountPoint)
        self._checkPath(path)
        existing = self._findMountPoint(path)
        if existing is not None:
            raise ValueError("Mount {0} is already mounted at {1} with priority {2}".format(existing[3], existing[2], existing[0]))
        self._mountDict[priority].append(mountPoint, mountObject)
        self._sortMounts()

    def fileReadable(self, path):
        mount, subPath = self._getFileMount(path)
        return mount.fileReadable(path)

    def fileWritable(self, path):
        mount, subPath = self._getFileMount(path)
        return mount.fileWritable(path)

    def open(self, path, flag='r'):
        mount, subPath = self._getFileMount(path)
        return mount.open(subPath, flag)
