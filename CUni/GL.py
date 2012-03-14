# File name: GL.py
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
from _cuni_gl import *

def __testSizeRange(size, min, max, type):
    if size > max or size < min:
        raise ValueError("Size must be in [{0}..{1}] for {2}. Got {3}".format(min, max, type, size))
    return size

def __testIndexRange(idx, min, max, type):
    idx = int(idx)
    if idx > max or idx < min:
        raise ValueError("Index must be in [{0}..{1}] for {2}. Got {3}".format(min, max, type, idx))
    return idx

__VertexFormat = VertexFormat

def VertexFormat(fmtSpecifier):
    nVertex, nColour = 0, 0
    nTexCoord = [0, 0, 0, 0]
    nAttrib = [0, 0, 0, 0]
    hasNormal = False

    if not isinstance(fmtSpecifier, (unicode, str)):
        segmentIterable = fmtSpecifier
    else:
        segmentIterable = fmtSpecifier.split(";")
    segments = [segment.split(':') for segment in segmentIterable]
    for type, size in ((type.strip(), size.strip()) for type, size in segments):
        if len(type) == 0:
            raise ValueError("Invalid type specifier: {0}".format(type))
        size = int(size)

        attrib = type[0]
        if attrib == "v":
            nVertex = __testSizeRange(size, 2, 4, attrib)
        elif attrib == "c":
            nColour = __testSizeRange(size, 3, 4, attrib)
        elif attrib == "n":
            __testSizeRange(size, 3, 3, attrib)
            hasNormal = True
        elif attrib == "t":
            idx = __testIndexRange(type[1:], 0, len(nTexCoord), attrib)
            nTexCoord[idx] = __testSizeRange(size, 1, 4, type)
        elif attrib == "g" or attrib == "a":
            idx = __testIndexRange(type[1:], 0, len(nAttrib), attrib)
            nAttrib[idx] = __testSizeRange(size, 1, 4, type)

    args = [nVertex, nColour] + nTexCoord + [hasNormal] + nAttrib;
    return __VertexFormat(*args)
