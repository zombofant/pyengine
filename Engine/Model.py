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
from GL.SceneGraph.Core import Geometry

class Model(Geometry):
    """
    The Model class stores 3D model data like vertices, normals,
    faces and texture coordinates, rotation, position, scale and so on.
    It also provides useful methods to work the vertex data like
    calculating the model's bounding box for example.
    See the class GL.RenderModel on how to render Models using OpenGL.
    """
    _dataTypes = ('Faces', 'Vertices', 'Normals', 'TexCoords', 'Materials')

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
        for face in self.Faces:
            fvertices, ftexcoords, fnormals = [], [], []
            for elems in face:
                vpos = elems[0]
                fvertices.extend(self.Vertices[vpos*3:vpos*3+3])
                if elems[1] is not None:
                    tpos = elems[1]
                    ftexcoords.extend(self.TexCoords[tpos*2:tpos*2+2])
                if elems[2] is not None:
                    npos = elems[2]
                    fnormals.extend(self.Normals[npos*3:npos*3+3])
            if None in fnormals: fnormals = None
            if None in ftexcoords: ftexcoords = None
            self._packedFaces.append((fvertices, fnormals, ftexcoords))
        self._facesNeedPacking = False
        return self._packedFaces

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
        arguments = dict((x[0].upper()+x[1:], y) for (x,y) in dict(**args).iteritems())
        for dtype in self._dataTypes:
            if dtype in arguments:
                self.__setattr__(dtype, arguments[dtype])

    @property
    def TexCoords(self):
        return self._texCoords

    @TexCoords.setter
    def TexCoords(self, value):
        self._texCoords = value
        self._facesNeedPacking = True
 
    @property
    def Materials(self):
        """
        Return the list of materials.
        See setter method for a description of the list format.
        May be an empty list of no materials have been set.
        """
        return self._materials

    @Materials.setter
    def Materials(self, value):
        """
        Set the list of materials.
        Every entry consists of a list containing two elements. The name of
        the material and the face index at which the material has to be
        activated during rendering.
        """
        if value is None: value = []
        self._materials = value

    @property
    def Faces(self):
        """
        Return a list of faces this model has.
        See setter for a description of the list format.
        This list may be empty if no vertices or indices have been set.
        """
        return self._faces

    @Faces.setter
    def Faces(self, value):
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
    def PackedFaces(self):
        """
        Return a list of packed face data.
        """
        if self._facesNeedPacking:
            self._packFaces()
        return self._packedFaces

