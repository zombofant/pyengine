# File name: GL.py
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
from _cuni_gl import *

def __test_size_range(size, min, max, type):
    if size > max or size < min:
        raise ValueError("Size must be in [{0}..{1}] for {2}. Got {3}".format(min, max, type, size))
    return size

def __test_index_range(idx, min, max, type):
    if len(idx) == 0:
        raise ValueError("{0} needs an index.".format(type))
    idx = int(idx)
    if idx > max or idx < min:
        raise ValueError("Index must be in [{0}..{1}] for {2}. Got {3}".format(min, max, type, idx))
    return idx

__VertexFormat = VertexFormat

def VertexFormat(fmtspecifier):
    nvertex, ncolour = 0, 0
    ntexcoord = [0, 0, 0, 0]
    nattrib = [0, 0, 0, 0]
    has_normal = False

    if not isinstance(fmtspecifier, (unicode, str)):
        segmentiter = fmtspecifier
    else:
        segmentiter = fmtspecifier.split(";")
    segments = [segment.split(':') for segment in segmentiter]
    for type, size in ((type.strip(), size.strip()) for type, size in segments):
        if len(type) + len(size) == 0:
            continue
        if len(type) == 0:
            raise ValueError("Invalid type specifier: {0}".format(type))
        size = int(size)

        attrib = type[0]
        if attrib == "v":
            nvertex = __test_size_range(size, 2, 4, attrib)
        elif attrib == "c":
            ncolour = __test_size_range(size, 3, 4, attrib)
        elif attrib == "n":
            __test_size_range(size, 3, 3, attrib)
            has_normal = True
        elif attrib == "t":
            idx = __test_index_range(type[1:], 0, len(ntexcoord), attrib)
            ntexcoord[idx] = __test_size_range(size, 1, 4, type)
        elif attrib == "g" or attrib == "a":
            idx = __test_index_range(type[1:], 0, len(nattrib), attrib)
            nattrib[idx] = __test_size_range(size, 1, 4, type)

    args = [nvertex, ncolour] + ntexcoord + [has_normal] + nattrib;
    return __VertexFormat(*args)
