# File name: Model.py
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
import sys

class Geometry(object):

    def __init__(self, **kwargs):
        super(Geometry, self).__init__(**kwargs)

    @property
    def Indices(self):
        return self._indices

    @Indices.setter
    def Indices(self, value):
        if value == []: value = None
        self._indices = value

    @property
    def Vertices(self):
        return self._vertices

    @Vertices.setter
    def Vertices(self, value):
        if value == []: value = None
        self._vertices = value

    @property
    def Normals(self):
        return self._normals

    @Normals.setter
    def Normals(self, value):
        if value == []: value = None
        self._normals = value

    @property
    def BoundingVolume(self):
        return self._bounding_volume

class Model(Geometry):
    """
    The Model class stores 3D model data like vertices, normals,
    faces and texture coordinates, rotation, position, scale and so on.
    It also provides useful methods to work the vertex data like
    calculating the model's bounding box for example.
    See the class GL.RenderModel on how to render Models using OpenGL.
    """
    _data_types = ('Faces', 'Vertices', 'Normals', 'TexCoords', 'Materials')

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
        self.set_data(**args)

    def _copy(self, other):
        """
        Copy variables from another Model object.
        """
        assert isinstance(other, Model)
        for dtype in self._data_types:
            setattr(self, dtype, getattr(other, dtype))

    def _pack_faces(self):
        """
        Pack faces into a convenient data structure.
        See the faces property below for details.
        """
        self._packed_faces = []
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
            self._packed_faces.append((fvertices, fnormals, ftexcoords))
        self._faces_need_packing = False
        return self._packed_faces

    def clear(self):
        """
        Clear all model data.
        This resets all model data properties to None.
        """
        for dtype in self._data_types:
            self.__setattr__(dtype, None)
        self._packed_faces = []
        self._faces_need_packing = False

    def set_data(self, **args):
        arguments = dict((x[0].upper()+x[1:], y) for (x,y) in dict(**args).iteritems())
        for dtype in self._data_types:
            if dtype in arguments:
                self.__setattr__(dtype, arguments[dtype])

    @property
    def TexCoords(self):
        return self._tex_coords

    @TexCoords.setter
    def TexCoords(self, value):
        self._tex_coords = value
        self._faces_need_packing = True
 
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
        self._faces_need_packing = True

    @property
    def PackedFaces(self):
        """
        Return a list of packed face data.
        """
        if self._faces_need_packing:
            self._pack_faces()
        return self._packed_faces

