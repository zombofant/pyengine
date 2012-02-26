# File name: Model.py
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
import sys

class Model(object):
    """
    The ModelData class stores 3D model data like vertices, normals,
    faces and texture coordinates, rotation, position, scale and so on.
    """

    def __init__(self, **kwargs):
        super(Model, self).__init__()
        self._dataTypes = ['indices', 'vertices', 'normals', 'texCoords']
        self.clear()
        if len(kwargs) > 0:
            self.setData(**kwargs)

    def clear(self):
        for dtype in self._dataTypes:
            self.__setattr__(dtype, [])

    def setData(self, **data):
        dataDict = dict(**data)
        for dtype in self._dataTypes:
            if dtype in dataDict:
                self.__setattr__(dtype, dataDict[dtype])

    @property
    def indices(self):
        if len(self._indices) < 1: return None
        return self._indices

    @indices.setter
    def indices(self, value):
        if value is None: value = []
        value = [int(x) for x in value]
        self._indices = value

    @property
    def vertices(self):
        if len(self._vertices) < 1: return None
        return self._vertices

    @vertices.setter
    def vertices(self, value):
        if value is None: value = []
        value = [float(x) for x in value]
        self._vertices = value

    @property
    def normals(self):
        if len(self._normals) < 1: return None
        return self._normals

    @normals.setter
    def normals(self, value):
        if value is None: value = []
        value = [float(x) for x in value]
        self._normals = value

    @property
    def texCoords(self):
        if len(self._texCoords) < 1: return None
        return self._texCoords

    @texCoords.setter
    def texCoords(self, value):
        if value is None: value = []
        value = [float(x) for x in value]
        self._texCoords = value

    @property
    def faces(self):
        faces, vertices = [], []
        for j in range(0, len(self._indices)):
            pos = self._indices[j] * 3
            comps = self._vertices[pos:pos+3]
            vertices.append(comps)
            if (j+1) % 3 == 0:
                faces.append(vertices)
                vertices = []
        return faces

