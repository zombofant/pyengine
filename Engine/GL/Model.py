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

# Try to load pyglet. If it fails, we provide a fallback since this
# class will be used by the dedicated server application which does
# not have a dependency on pyglet.
try:
    from pyglet.graphics import vertex_list_indexed
    from OpenGL.GL import GL_TRIANGLES
except ImportError:
    raise NotImplemented('We need a fallback here')

class Model(object):
    """
    The Model class stores 3D model data like vertices, normals,
    faces and texture coordinates, rotation, position, scale and so on.
    It also provides methods to modify and render the model using OpenGL.
    """

    def __init__(self, indices=None, vertices=None, normals=None, texCoords=None):
        self._vertexList = None
        self._clear()
        if indices is not None and vertices is not None:
            self._setVertexData(indices, vertices, normals, texCoords)

    def _clear(self):
        if self._vertexList is not None:
            del self._vertexList
            self._vertexList = None
        self._faces = []

    def _setVertexData(self, indices, vertices, normals=None, texCoords=None):
        """
        Set the vertex data. At least indices and vertices have to be passed
        in order to construct the vertex list. Please not that a complete new
        vertex list is created when calling this method. This means all
        previous vertex data will be dropped.
        Each vertex at a given position in the list of vertices corresponds to
        the normal/texCoord at this position in the respective list (if not None)
        """
        self._clear()
        size = len(vertices) // 3
        data = [('v3f/static', list(vertices))]
        if normals is not None:
            data.append(('n3f/static', list(normals)))
        if texCoords is not None:
            data.append(('t2f/static', list(texCoords)))
        self._vertexList = vertex_list_indexed(size, list(indices), *data)

    def draw(self):
        if self._vertexList is None: return
        self._vertexList.draw(GL_TRIANGLES)

    @property
    def vertices(self):
        if self._vertexList is None: return None
        return self._vertexList.vertices

    @property
    def indices(self):
        if self._vertexList is None: return None
        return self._vertexList.indices

