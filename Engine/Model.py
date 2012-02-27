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
    It also provides useful methods to work the vertex data like
    calculating the model's bounding box for example.
    See the class GL.RenderModel on how to render Models using OpenGL.
    """

    def __init__(self, **kwargs):
        """
        Construct a new Model instance.
        You may pass the parameters indices, vertices, normals and
        texCoords to initialize the model data during construction.
        Each argument has to pass an appropriate list (see the
        corresponding setters below for details).
        """
        super(Model, self).__init__()
        self._dataTypes = ['indices', 'vertices', 'normals', 'texCoords']
        self.clear()
        if len(kwargs) > 0:
            self.setData(**kwargs)

    def clear(self):
        """
        Clear all model data.
        This resets all model data properties to None.
        """
        for dtype in self._dataTypes:
            self.__setattr__(dtype, None)

    def setData(self, **data):
        """
        Set multiple data types at once.
        This works exactly like passing arguments to __init__.
        """
        dataDict = dict(**data)
        for dtype in self._dataTypes:
            if dtype in dataDict:
                self.__setattr__(dtype, dataDict[dtype])

    @property
    def indices(self):
        """
        Returns the list of indices.
        The list contains only ints pointing to the vertex at the
        location described by the int. Since we only work with
        triangulated faces, every three indices are describing a
        single face of the model.
        May be None if no indices have been set.
        """
        return self._indices

    @indices.setter
    def indices(self, value):
        """
        Set the indices used to create faces.
        This list has to contain three ints for every face the model
        is build with. The ints are indexes to the appropriate vertex
        in the vertices list. Since in 3D space every vertex has three
        components, if the index is - for example - 5, it points to
        the three elements in vertices at position 5*3.
        To unset this property, just supply a value of None.
        """
        if value is not None:
            value = [int(x) for x in value]
            if len(value) == 0: value = None
        self._indices = value

    @property
    def vertices(self):
        """
        The list of vertices.
        Every vertex consists of three components in 3D space.
        May be None if no vertices have been set.
        """
        return self._vertices

    @vertices.setter
    def vertices(self, value):
        """
        Set the list of vertices.
        Each vertex consists of three components.
        To unset this property, just supply a value of None.
        """
        if value is not None:
            value = [float(x) for x in value]
            if len(value) == 0: value = None
        self._vertices = value

    @property
    def normals(self):
        """
        The list of normals.
        Since normals are vectors, they consist of three components.
        May be None if no normals have been set.
        """
        return self._normals

    @normals.setter
    def normals(self, value):
        """
        Set the list of normals.
        Normals consist of three components.
        Every normal has to be placed at the same position as the
        corresponding vertex in vertices. This means the same normals
        may be found several time in the list if some vertices have
        the same normals.
        This list has to be of the same length as vertices.
        To unset this property, just supply a value of None.
        """
        if value is not None:
            value = [float(x) for x in value]
            if len(value) == 0: value = None
        self._normals = value

    @property
    def texCoords(self):
        """
        Return the list of texture coordinates.
        Every texture coordinate consists of two components.
        May be None if no texture coords have been set.
        """
        return self._texCoords

    @texCoords.setter
    def texCoords(self, value):
        """
        Set the list of texture coordinates.
        Every texture coordinate consists of two elements and has to be
        placed at the same position in the list as the corresponding
        vertex in vertices (this means if the vertex at position 3 in
        the vertices list corresponds to the texture coordinate at
        position 3 in the texture coordinate list (which is at index
        3*2 for the vertex at index 3*3).
        To unset this property, just supply a value of None.
        """
        if value is not None:
            value = [float(x) for x in value]
            if len(value) == 0: value = None
        self._texCoords = value

    @property
    def faces(self):
        """
        Return a list of faces this model has.
        The returned list contains a list for every face where each face
        list contains lists for the three vertex components of every vertex
        in the face.
        This list is empty if no vertices or indices have been set.
        """
        if None in (self._indices, self._vertices):
            return []
        faces, vertices = [], []
        for j in range(0, len(self._indices)):
            pos = self._indices[j] * 3
            comps = self._vertices[pos:pos+3]
            vertices.append(comps)
            if (j+1) % 3 == 0:
                faces.append(vertices)
                vertices = []
        return faces

