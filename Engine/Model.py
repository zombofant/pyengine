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

    def __init__(self, faces=None, materials=None):
        """
        Construct a new Model instance.
        You may pass the parameters indices, vertices, normals and
        texCoords to initialize the model data during construction.
        Each argument has to pass an appropriate list (see the
        corresponding setters below for details).
        """
        super(Model, self).__init__()
        self.clear()
        if faces is not None:
            self.faces = faces
            if materials is not None:
                self.materials = materials

    def clear(self):
        """
        Clear all model data.
        This resets all model data properties to None.
        """
        self._faces = []

    @property
    def vertices(self):
        """
        The list of vertices.
        Every vertex consists of three components in 3D space.
        May be None if no vertices have been set.
        """
        return [x for f in self._faces for x in f[0]]

    @property
    def normals(self):
        """
        The list of normals.
        Since normals are vectors, they consist of three components.
        May be None if no normals have been set.
        """
        return [x for f in self._faces for x in f[1]]

    @property
    def texCoords(self):
        """
        Return the list of texture coordinates.
        Every texture coordinate consists of two components.
        May be None if no texture coords have been set.
        """
        return [x for f in self._faces for x in f[2]]

    @property
    def materials(self):
        return self._materials

    @materials.setter
    def materials(self, value):
        self._materials = value

    @property
    def faces(self):
        """
        Return a list of faces this model has.
        The returned list contains a list for every face where each face
        list contains lists for the three vertex components of every vertex
        in the face.
        This list is empty if no vertices or indices have been set.
        """
        return self._faces

    @faces.setter
    def faces(self, value):
        self._faces = value

