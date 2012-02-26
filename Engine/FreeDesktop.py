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

import os
import platform

if platform.system() == "Linux":
    XDGDefaults = {
        'XDG_DATA_DIRS': '/usr/local/share:/usr/share:.',
        'XDG_CONFIG_DIRS': '/etc/xdg',
        'XDG_CONFIG_HOME': '~/.config',
        'XDG_DATA_HOME': '~/.local/share',
    }
elif platform.system() == "Windows":
    XDGDefaults = {
        'XDG_DATA_DIRS': '',
        'XDG_CONFIG_DIRS': '',
        'XDG_CONFIG_HOME': '~/.config',
        'XDG_DATA_HOME': '~/.local/share',
    }
else:
    raise NotImplementedError("XDG paths are not defined on {0}".format(platform.system()))
    
def _splitDirs(dirs, appDirName):
    for dir in dirs.split(os.path.pathsep):
        if len(dir) == 0:
            continue
        dir = os.path.join(os.path.abspath(dir), appDirName)
        if not os.path.isdir(dir):
            continue
        yield dir

def __platform_includeCWD():
    return platform.system() == "Windows"

def getDirSet(globalKey, homeKey, appDirName, includeCWD):
    globalDirs = _splitDirs(os.environ.get(globalKey, XDGDefaults[globalKey]), appDirName)
    if includeCWD:
        globalDirs.append(os.path.abspath('.'))

    homeDir = os.path.join(os.environ.get(homeKey, os.path.expanduser(XDGDefaults[homeKey])), appDirName)
    return globalDirs, homeDir

def requireDirs(appDirName, includeCWD=None):
    """
    Returns a tuple whose entries represent the recommended data
    and config paths respectively.
    
    The returned tuple has the following structure:
    ``(dataDirs, dataHome, configDirs, configHome)``
    with dataDirs and configDirs being lists and dataHome and configHome
    being strings.
    
    dataDirs and configDirs may be empty if the directories they were
    supposed to point to did not exist.
    
    dataHome and configHome will always be set, directories are created
    as neccessary.
    """
    if includeCWD is None:
        includeCWD = __platform_includeCWD()

    dataDirs, dataHome = getDirSet("XDG_DATA_DIRS", "XDG_DATA_HOME", appDirName, includeCWD)
    if not os.path.isdir(dataHome):
        makedirs(dataHome)

    configDirs, configHome = getDirSet("XDG_CONFIG_DIRS", "XDG_CONFIG_HOME", appDirName, includeCWD)
    if not os.path.isdir(configHome):
        makedirs(configHome)
    
    return dataDirs, dataHome, configDirs, configHome
