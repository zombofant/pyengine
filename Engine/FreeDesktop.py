# File name: FreeDesktop.py
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
u"""
Implements the FreeDesktop.org basedir standard from
<http://standards.freedesktop.org/basedir-spec/basedir-spec-0.8.html>
"""

from __future__ import unicode_literals, print_function, division
from our_future import *

import os
import platform

XDGDefaults = {
    'XDG_DATA_DIRS': '',
    'XDG_CONFIG_DIRS': '',
    'XDG_CONFIG_HOME': '',
    'XDG_DATA_HOME': '',
}
"""
Provides drop-in replacement defaults according to the basedir spec
(with additional values for windows machines) for the environment
variables defined by the spec.
"""

if platform.system() == "Linux":
    XDGDefaults = {
        'XDG_DATA_DIRS': '/usr/local/share:/usr/share',
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
    
def _split_dirs(dirs, app_dir_name):
    for dir in dirs.split(os.path.pathsep):
        if len(dir) == 0:
            continue
        dir = os.path.join(os.path.abspath(dir), app_dir_name)
        if not os.path.isdir(dir):
            continue
        yield dir

def get_dir_set(global_key, home_key, app_dir_name):
    """
    Detects directories from environment variables named *global_key*
    and *home_key* (or their drop-in replacements from *XDGDefaults*),
    appending *app_dir_name*.
    
    The environment value behind *global_key* is splitted using
    *os.path.pathsep*, each element gets the *app_dir_name* appended using
    *os.path.join*.

    For *home_key*, just *app_dir_name* gets appended. If a fallback to the
    XDGDefaults dict is neccessary, os.path.expanduser is applied on it,
    so that ~ gets replaced by the users home directory.
    """
    global_dirs = _split_dirs(os.environ.get(global_key, XDGDefaults[global_key]), app_dir_name)
    
    home_dir = os.path.join(os.environ.get(home_key, os.path.expanduser(XDGDefaults[home_key])), app_dir_name)
    return global_dirs, home_dir

def require_dirs(app_dir_name):
    """
    Returns a tuple whose entries represent the recommended data
    and config paths respectively.
    
    The returned tuple has the following structure:
    ``(data_dirs, data_home, config_dirs, config_home)``
    with data_dirs and config_dirs being lists and data_home and config_home
    being strings.
    
    data_dirs and config_dirs may be empty if the directories they were
    supposed to point to did not exist.
    
    data_home and config_home will always be set, directories are created
    as neccessary.
    """
    data_dirs, data_home = get_dir_set("XDG_DATA_DIRS", "XDG_DATA_HOME", app_dir_name)
    if not os.path.isdir(data_home):
        os.makedirs(data_home)

    config_dirs, config_home = get_dir_set("XDG_CONFIG_DIRS", "XDG_CONFIG_HOME", app_dir_name)
    if not os.path.isdir(config_home):
        os.makedirs(config_home)
    
    return data_dirs, data_home, config_dirs, config_home
