# File name: FreeDesktop.py
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

from FileSystem import FileSystem
from Mounts import MountDirectory

import os
import platform

if platform.system() == "Linux":
    XDGDefaults = {
        'XDG_DATA_DIRS': '/usr/local/share:/usr/share',
        'XDG_CONFIG_DIRS': '/etc/xdg',
        'XDG_CONFIG_HOME': '~/.config',
        'XDG_DATA_HOME': '~/.local/share'
    }
elif platform.system() == "Windows":
    # FIXME: add XDG defaults for windows
    raise NotImplementedError("nothing to see here yet.")
    
def _splitDirs(dirs):
    # TODO: split directories.
    for dir in dirs.split(os.path.pathsep):
        pass

def XDGFileSystem(applicationName):
    fs = FileSystem()
    userDir = os.path.expanduser("~")
    dataDirs = os.environ.get('XDG_DATA_DIRS', '/usr/local/share/:/usr/share/')

    # TODO
    # fs.mount('/data', MountDirectory()
