# File name: Mounts.py
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

import os.path

from Utils import absolutify, normalizeVFSPath, isWriteFlag
import StringIO

# FIXME: Introduce proper Error classes

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

class MountDirectory(Mount):
    def __init__(self, path, readOnly=True, **kwargs):
        super(MountDirectory, self).__init__(**kwargs)
        self.path = os.path.realpath(os.path.abspath(path))
        self._readOnly = readOnly

    def getRealPath(self, vfspath):
        return os.path.join(self.path, *vfspath.split('/'))

    def fileReadable(self, path):
        return os.access(self.getRealPath(path), os.R_OK)

    def fileWritable(self, path):
        return (not self._readOnly) and os.access(self.getRealPath(path), os.W_OK)

    def open(self, path, flag):
        # FIXME: is this sufficient to detect a write access?
        if self._readOnly and isWriteFlag(flag):
            raise IOError(13, "Permission denied: '"+self.getRealPath(path)+"'")
        return open(self.getRealPath(path), flag)

    @property
    def ReadOnly(self):
        return self._readOnly


# FIXME: Implement write IO
class MountVirtual(Mount):
    """
    Supports adding strings as text files in a virtual file system.
    """
    
    def __init__(self, **kwargs):
        super(MountVirtual, self).__init__(**kwargs)
        self._files = {}

    def _manglePath(self, path):
        return absolutify(normalizeVFSPath(unicode(path)))

    def __setitem__(self, vfspath, data):
        vfspath = self._manglePath(vfspath)
        if vfspath in self._files:
            # FIXME: Decide on an error to raise here
            pass
        self._files[vfspath] = unicode(data)

    def __delitem__(self, vfspath):
        del self._files[self._manglePath(vfspath)]

    def fileReadable(self, path):
        return self._manglePath(path) in self._files

    def fileWritable(self, path):
        return False and (self._manglePath(path) in self._files)

    def open(self, path, flag):
        path = self._manglePath(path)
        if isWriteFlag(flag):
            raise IOError(13, "Permission denied: '"+path+"'")
        if not path in self._files:
            raise IOError(2, "No such file or directory: '"+path+"'")
        return StringIO.StringIO(self._files[path])
        
    @property
    def ReadOnly(self):
        return True
