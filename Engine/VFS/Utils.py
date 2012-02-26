# File name: Utils.py
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

import platform

def absolutify(path):
    """
    Attempts to create an absolute path from a segment of a relative
    path.

    If the path somehow leaves its own scope (like /../ does) a
    ValueError is raised.

    If the path misses a leading slash, it is added.
    """
    segments = path.split('/')
    if len(segments[0]) == 0:
        del segments[0]
    i = 0
    while i < len(segments):
        segment = segments[i]
        if segment == '.':
            del segments[i]
            continue
        if segment == '..':
            if i > 0:
                del segments[i]
                i -= 1
                del segments[i]
                continue
            else:
                raise ValueError("Path leaves its root.")
        i += 1
    return '/'+'/'.join(segments)

def validateVFSPath(path):
    """
    Checks whether a path is a valid VFS path.

    Relative paths, a path not starting with / or a path containing //
    will be rejected with a ValueError.
    """
    nodes = path.split('/')
    if len(nodes[0]) > 0:
        raise ValueError("Invalid VFS path {0!r}: paths must start with /".format(path))
    for node in nodes[1:]:
        if node == "." or node == "..":
            raise ValueError("Invalid VFS path {0!r}: relative paths are not allowed".format(path))
        if len(node) == 0:
            raise ValueError("Invalid VFS path {0!r}: \"//\" must not occur in a path".format(path))
    

def normalizeVFSPath(path):
    """
    Removes trailing slashes.
    """

    if len(path) > 1 and path[-1] == '/':
        path = path[:-1]
    return path

def isWriteFlag(flag):
    """
    Checks whether an :func:`open()` flag means opening a file for
    writing.

    Returns *True* if this flag combination would mean a write access,
    *False* otherwise.
    """

    # FIXME: Verify that this is sufficient
    return flag.startswith("w") or flag.startswith("a") or "+" in flag
    
    
