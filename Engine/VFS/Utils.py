# File name: Utils.py
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

import platform

def absolutify(path):
    """
    Attempts to create an absolute path from a segment of a relative
    path.

    If the path somehow leaves its own scope (like /../ does) a
    ValueError is raised.

    If the path misses a leading slash, it is added. A trailing slash
    is not added.
    """
    segments = path.split('/')
    if len(segments[0]) == 0:
        del segments[0]
    i = 0
    while i < len(segments):
        segment = segments[i]
        if len(segment) == 0 or segment == '.':
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

def validate_vfs_path(path):
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


def normalize_vfs_path(path):
    """
    Removes trailing slashes.
    """

    if len(path) > 1 and path[-1] == '/':
        path = path[:-1]
    return path

def is_write_flag(flag):
    """
    Checks whether an :func:`open()` flag means opening a file for
    writing.

    Returns *True* if this flag combination would mean a write access,
    *False* otherwise.
    """

    # FIXME: Verify that this is sufficient
    return flag.startswith("w") or flag.startswith("a") or "+" in flag

def join(*segments):
    """
    Works like os.path.join, except that it does not use the platforms
    directory separator but always a /.
    """
    full_path = ""
    for segment in segments:
        if len(segment) == 0:
            continue
        if segment[0] == "/":
            full_path = absolutify(segment)
        else:
            segment = absolutify(segment)
            if len(full_path) == 0:
                full_path += segment[1:]
            else:
                full_path += segment
    return full_path

def splitext(path):
    """
    Splits off the file extension from the given *path*.

    Returns a tuple ``(path, ext)`` where path is everything before the
    last dot in the filename segment and ext is everything behind.

    This function is aware of directories.
    """
    path, filename = path.rsplit('/', 1)
    items = filename.rsplit('.', 1)
    if len(items) == 1:
        basename, = items
        ext = ""
    else:
        basename, ext = items
    return path+"/"+basename, ext
