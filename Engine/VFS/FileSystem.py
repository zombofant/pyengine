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

import Engine.FreeDesktop as FreeDesktop

from Utils import normalizeVFSPath, validateVFSPath
from Mounts import Mount, MountDirectory

class VFSIOError(IOError):
    pass

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

class FileSystem(object):
    def __init__(self, **kwargs):
        super(FileSystem, self).__init__(**kwargs)
        self._mountDict, self._mounts = MountDict()

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

    def _findFileMounts(self, filePath):
        for key, value in self._mounts:
            for path, obj in value:
                if filePath.startswith(path):
                    if path == filePath:
                        continue
                    yield (path, filePath[len(path):], obj)

    def _sortMounts(self):
        for key, value in self._mounts:
            value.sort(key=lambda x: len(x[0]))

    def _getFileMounts(self, path):
        path = normalizeVFSPath(path)
        validateVFSPath(path)
        for mountPoint, subPath, mount in self._findFileMounts(path):
            yield mount, subPath

    def mount(self, mountPoint, mountObject, priority):
        if not isinstance(mountObject, Mount):
            raise TypeError("Expected Mount, got {0}".format(type(mountObject)))
        existing = self._findMount(mountObject)
        if existing is not None:
            raise ValueError("Mount {0} is already mounted at {1} with priority {2}".format(existing[3], existing[2], existing[0]))
        path = normalizeVFSPath(mountPoint)
        validateVFSPath(path)
        self._mountDict[priority].append((path, mountObject))
        self._sortMounts()

    def fileReadable(self, path):
        for mount, subPath in self._getFileMounts(path):
            try:
                if mount.fileReadable(subPath):
                    return True
            except IOError:
                continue
        return False

    def fileWritable(self, path):
        for mount, subPath in self._getFileMounts(path):
            try:
                if mount.fileWritable(subPath):
                    return True
            except IOError:
                continue
        return False

    def open(self, path, flag='r'):
        for mount, subPath in self._getFileMounts(path):
            try:
                f = mount.open(subPath, flag)
                if f is not None:
                    return f
            except IOError:
                continue
        raise VFSIOError("Cannot open file '{0}' with flags '{1}'".format(path, flag))

class XDGFileSystem(FileSystem):
    def _setupMounts(self, mountPoint, globalDirs, homeDir):
        for dir in globalDirs:
            self.mount(mountPoint, MountDirectory(dir, readOnly=True), MountPriority.FileSystem)
        self.mount(mountPoint, MountDirectory(homeDir, readOnly=False), MountPriority.Important)
    
    def __init__(self, appDirName, dataMountPoint="/data", configMountPoint="/config", **kwargs):
        super(XDGFileSystem, self).__init__(**kwargs)
        dataDirs, dataHome, configDirs, configHome = FreeDesktop.requireDirs(appDirName)

        self._setupMounts(dataMountPoint, dataDirs, dataHome)
        self._setupMounts(configMountPoint, configDirs, configHome)
