# File name: FileSystem.py
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

import Engine.FreeDesktop as FreeDesktop

from Utils import normalize_vfs_path, validate_vfs_path
from Mounts import Mount, MountDirectory
from Errors import VFSPermissionDeniedError, VFSIOError, VFSFileNotFoundError

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
    """
    Creates a priority dict/list pair for use with a VFS. The dictionary
    uses the priority values from :class:`MountPriority` as keys and has
    empty lists assigned as values. The list contains of tuples of
    (priority, list), where `list` is the same instance as the list in
    the dict at key location `priority`.

    Returns a tuple (dict, list).
    """
    values = sorted((value for key, value in MountPriority.__dict__.iteritems() if not key.startswith("__")), reverse=True)
    l = [(value, []) for value in values]
    d = dict(l)
    return (d, l)

class FileSystem(object):
    def __init__(self, **kwargs):
        super(FileSystem, self).__init__(**kwargs)
        self._mount_dict, self._mounts = MountDict()

    def _find_mount(self, mount):
        for key, value in self._mounts:
            for i, (path, obj) in enumerate(value):
                if obj is mount:
                    return (key, i, path, obj)
        return None

    def _find_mount_point(self, path):
        for key, value in self._mounts:
            for i, (path, obj) in enumerate(value):
                if path == path:
                    return (key, i, path, obj)
        return None

    def _find_file_mounts(self, filepath):
        for key, value in self._mounts:
            for path, obj in value:
                if filepath.startswith(path):
                    if path == filepath:
                        continue
                    yield (path, filepath[len(path):], obj)

    def _sort_mounts(self):
        for key, value in self._mounts:
            value.sort(key=lambda x: len(x[0]))

    def _get_file_mounts(self, path):
        path = normalize_vfs_path(path)
        validate_vfs_path(path)
        for mount_point, subpath, mount in self._find_file_mounts(path):
            yield mount, subpath

    def mount(self, mount_point, mount_object, priority):
        """
        Mounts a Mount instance *mount_object* at *mount_point* with a
        given *priority*.

        If an attempt is made to mount the same *mount_object* at
        different locations, a ValueError is raised.

        If an attempt is made to mount an object which is not derived
        from *Mount*, a TypeError is raised.
        """
        if not isinstance(mount_object, Mount):
            raise TypeError("Expected Mount, got {0}".format(type(mount_object)))
        existing = self._find_mount(mount_object)
        if existing is not None:
            raise ValueError("Mount {0} is already mounted at {1} with priority {2}".format(existing[3], existing[2], existing[0]))
        path = normalize_vfs_path(mount_point)
        validate_vfs_path(path)
        self._mount_dict[priority].append((path, mount_object))
        self._sort_mounts()

    def file_readable(self, path):
        """
        Determines whether the file at *path* can be opened for reading.
        """
        for mount, subpath in self._get_file_mounts(path):
            try:
                if mount.file_readable(subpath):
                    return True
            except IOError:
                continue
        return False

    def file_writable(self, path):
        """
        Determines whether the file at *path* can be opened for writing.
        """
        for mount, subpath in self._get_file_mounts(path):
            try:
                if mount.file_writable(subpath):
                    return True
            except IOError:
                continue
        return False

    def open(self, path, flag='r'):
        """
        Tries to open the file at path with flags.

        For documentation of flags, see :class:`file`.

        If opening the file fails, a VFSIOError is raised. If a
        permission error is encountered during iteration of possible
        mounts, a VFSPermissionDeniedError is raised, otherwise a
        VFSFileNotFoundError is raised.

        Returns a file like object on success.
        """
        had_permission_denied = False
        for mount, subpath in self._get_file_mounts(path):
            try:
                f = mount.open(subpath, flag)
                if f is not None:
                    return f
            except VFSPermissionDeniedError:
                had_permission_denied = True
            except IOError:
                pass
        if had_permission_denied:
            raise VFSPermissionDeniedError(path)
        raise VFSFileNotFoundError(path)

    def listdir(self, path):
        """
        Return an iterable of all nodes in the directory pointed to by
        *path*.

        The returned iterable is not neccessarily a list; it might be
        an iterator which is depleted after one use. There is no
        guaranteed ordering in the items returned by the iterator.
        """
        for mount, subpath in self._get_file_mounts(path):
            try:
                for item in mount.listdir(subpath):
                    yield item
            except VFSFileNotFoundError:
                pass


class XDGFileSystem(FileSystem):
    """
    Automatically mounts the paths returned by :func:`require_dirs` from
    :mod:`FreeDesktop` at customizable locations.
    """

    def _setup_mounts(self, mount_point, global_dirs, home_dir):
        for dir in global_dirs:
            self.mount(mount_point, MountDirectory(dir, read_only=True), MountPriority.FileSystem)
        self.mount(mount_point, MountDirectory(home_dir, read_only=False), MountPriority.Important)

    def __init__(self, app_dir_name, data_mount_point="/data", config_mount_point="/config", **kwargs):
        """
        Initializes a FreeDesktop compliant VFS.

        The full path name is derived from the XDG environment
        variables (or their defaults) with *app_dir_name~ appended.

        Mounts all data directories at the location specified by
        *data_mount_point*. All global data directories are mounted
        read-only, while the home data directory is mounted writable
        and also with a higher priority than the shared directories.

        The same is done for config directories, except that
        *config_mount_point* is used instead.

        *data_mount_point* and *config_mount_point* can point to the same
        location in the VFS.
        """
        super(XDGFileSystem, self).__init__(**kwargs)
        data_dirs, data_home, config_dirs, config_home = FreeDesktop.require_dirs(app_dir_name)

        self._setup_mounts(data_mount_point, data_dirs, data_home)
        self._setup_mounts(config_mount_point, config_dirs, config_home)
