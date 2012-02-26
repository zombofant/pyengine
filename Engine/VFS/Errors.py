# File name: Errors.py
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

import errno

class VFSError(BaseException):
    pass

class VFSIOError(IOError, VFSError):
    def _processFileNameArgument(self, fileName):
        return (": '"+fileName+"'") if fileName is not None else ""

class VFSPermissionDeniedError(VFSIOError):
    def __init__(self, fileName):
        super(VFSPermissionDeniedError, self).__init__(errno.EACCES, "Permission denied{0}".format(self._processFileNameArgument(fileName)))

class VFSFileNotFoundError(VFSIOError):
    def __init__(self, fileName):
        super(VFSFileNotFoundError, self).__init__(errno.ENOENT, "No such file or directory{0}".format(self._processFileNameArgument(fileName)))

SemanticException = {
    errno.EACCES: VFSPermissionDeniedError,
    errno.ENOENT: VFSFileNotFoundError
}
