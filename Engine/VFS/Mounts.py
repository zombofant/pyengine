# File name: Mounts.py
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

import os.path

from Errors import VFSPermissionDeniedError, VFSFileNotFoundError, SemanticException
from Utils import absolutify, normalize_vfs_path, is_write_flag
import StringIO

class Mount(object):
    def __init__(self, **kwargs):
        super(Mount, self).__init__(**kwargs)

    def get_real_path(self, file):
        return None

    def file_readable(self, file):
        raise NotImplementedError("Mount.file_readable not specified")

    def file_writable(self, file):
        raise NotImplementedError("Mount.file_writable not specified")

    def open(self, file, flag):
        raise NotImplementedError("Mount.open not specified")

    def listdir(self, path):
        raise NotImplementedError("Mount.listdir not specified")

class MountDirectory(Mount):
    """
    Provides access to a real file system directory.
    """

    def __init__(self, path, read_only=True, **kwargs):
        super(MountDirectory, self).__init__(**kwargs)
        self.path = os.path.realpath(os.path.abspath(path))
        self._read_only = read_only

    def get_real_path(self, vfspath):
        return os.path.join(self.path, *vfspath.split('/'))

    def file_readable(self, path):
        return os.access(self.get_real_path(path), os.R_OK)

    def file_writable(self, path):
        return (not self._read_only) and os.access(self.get_real_path(path), os.W_OK)

    def open(self, path, flag):
        # FIXME: is this sufficient to detect a write access?
        if self._read_only and is_write_flag(flag):
            raise VFSPermissionDeniedError(path)
        try:
            return open(self.get_real_path(path), flag)
        except IOError as err:
            new_type = SemanticException.get(err.errno, None)
            if new_type is not None:
                raise new_type(path)
            raise

    def listdir(self, path):
        try:
            return os.listdir(self.get_real_path(path))
        except OSError as err:
            new_type = SemanticException.get(err.errno, None)
            if new_type is not None:
                raise new_type(path)
            raise

    @property
    def ReadOnly(self):
        return self._read_only


# FIXME: Implement write IO
class MountVirtual(Mount):
    """
    Supports adding strings as text files in a virtual file system.
    """

    def __init__(self, **kwargs):
        super(MountVirtual, self).__init__(**kwargs)
        self._files = {}

    def _mangle_path(self, path):
        return absolutify(normalize_vfs_path(unicode(path)))

    def __setitem__(self, vfspath, data):
        vfspath = self._mangle_path(vfspath)
        if vfspath in self._files:
            # FIXME: Decide on an error to raise here
            pass
        self._files[vfspath] = unicode(data)

    def __delitem__(self, vfspath):
        del self._files[self._mangle_path(vfspath)]

    def file_readable(self, path):
        return self._mangle_path(path) in self._files

    def file_writable(self, path):
        return False and (self._mangle_path(path) in self._files)

    def open(self, path, flag):
        path = self._mangle_path(path)
        if is_write_flag(flag):
            raise VFSPermissionDeniedError(path)
        if not path in self._files:
            raise VFSFileNotFoundError(path)
        return StringIO.StringIO(self._files[path])

    @property
    def ReadOnly(self):
        return True
