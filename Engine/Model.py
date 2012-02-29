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
    The Model class stores 3D model data like vertices, normals,
    faces and texture coordinates, rotation, position, scale and so on.
    It also provides useful methods to work the vertex data like
    calculating the model's bounding box for example.
    See the class GL.RenderModel on how to render Models using OpenGL.
    """
    _dataTypes = ('faces', 'vertices', 'normals', 'texCoords', 'materials')

    def __init__(self, **args):
        """
        Construct a new Model instance.
        You may pass the parameters faces and materials in oder to initialize
        the model data during construction.
        Each argument has to pass an appropriate list (see the corresponding
        setters below for details).
        """
        super(Model, self).__init__()
        self.clear()
        self.setData(**args)

    def _copy(self, other):
        """
        Copy variables from another Model object.
        """
        assert isinstance(other, Model)
        for dtype in self._dataTypes:
            setattr(self, dtype, getattr(other, dtype))

    def _packFaces(self):
        """
        Pack faces into a convenient data structure.
        See the faces property below for details.
        """
        self._packedFaces = []
        for face in self.faces:
            fvertices, ftexcoords, fnormals = [], [], []
            for elems in face:
                vpos = elems[0]
                fvertices.extend(self.vertices[vpos*3:vpos*3+3])
                if elems[1] is not None:
                    tpos = elems[1]
                    ftexcoords.extend(self.texCoords[tpos*2:tpos*2+2])
                if elems[2] is not None:
                    npos = elems[2]
                    fnormals.extend(self.normals[npos*3:npos*3+3])
            if None in fnormals: fnormals = None
            if None in ftexcoords: ftexcoords = None
            self._packedFaces.append((fvertices, fnormals, ftexcoords))
        self._facesNeedPacking = False
        return self._packedFaces

    def _calcBoundingBox(self):
        raise NotImplementedError()

    def _calcCenterVertex(self):
        raise NotImplementedError()

    def clear(self):
        """
        Clear all model data.
        This resets all model data properties to None.
        """
        for dtype in self._dataTypes:
            self.__setattr__(dtype, None)
        self._packedFaces = []
        self._facesNeedPacking = False

    def setData(self, **args):
        arguments = dict(**args)
        for dtype in self._dataTypes:
            if dtype in arguments:
                self.__setattr__(dtype, arguments[dtype])

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
        self._vertices = value
        self._facesNeedPacking = True
        #self._calcBoundingBox()
 
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
        self._normals = value
        self._facesNeedPacking = True
 
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
        self._texCoords = value
        self._facesNeedPacking = True
 
    @property
    def materials(self):
        """
        Return the list of materials.
        See setter method for a description of the list format.
        May be an empty list of no materials have been set.
        """
        return self._materials

    @materials.setter
    def materials(self, value):
        """
        Set the list of materials.
        Every entry consists of a list containing two elements. The name of
        the material and the face index at which the material has to be
        activated during rendering.
        """
        if value is None: value = []
        self._materials = value

    @property
    def faces(self):
        """
        Return a list of faces this model has.
        See setter for a description of the list format.
        This list may be empty if no vertices or indices have been set.
        """
        return self._faces

    @faces.setter
    def faces(self, value):
        """
        Set list of faces.
        This data structure contains most information about a models geometry. A
        face consists of a list of three elements. The list of vertices (3
        elements per vertex), the list of normals (3 per vertex) and the list of
        texture coordinates (2 per vertex).
        """
        if value is None: value = []
        self._faces = value
        self._facesNeedPacking = True

    @property
    def packedFaces(self):
        """
        Return a list of packed face data.
        """
        if self._facesNeedPacking:
            self._packFaces()
        return self._packedFaces

