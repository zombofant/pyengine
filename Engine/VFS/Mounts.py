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
        if flag.startswith("w") and self._readOnly:
            raise IOError(13, "Permission denied: '"+self.getRealPath(path)+"'")
        return open(self.getRealPath(path), flag)

    @property
    def ReadOnly(self):
        return self._readOnly
