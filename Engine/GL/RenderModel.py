# File name: RenderModel.py
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

from Engine.Model import Model
from pyglet.graphics import vertex_list_indexed
from OpenGL.GL import GL_TRIANGLES

class RenderModel(object):
    """
    This class renders a Model.
    """

    def __init__(self, model=None):
        super(RenderModel, self).__init__()
        self._vertexList = None
        if model is not None:
            self.model = model

    def _clear(self):
        if self._vertexList is not None:
            del self._vertexList
            self._vertexList = None

    def _updateVertexList(self):
        """
        Update the vertex list. At least indices and vertices have to be
        available in order to construct a vertex list. Please not that a
        complete new vertex list is created when calling this method. This
        means all previous vertex data will be dropped.
        """
        model = self._model
        if None in (model.indices, model.vertices): return
        self._clear()
        size = len(model.vertices) // 3
        data = [('v3f/static', model.vertices)]
        if model.normals is not None:
            data.append(('n3f/static', model.normals))
        if model.texCoords is not None:
            data.append(('t2f/static', model.texCoords))
        self._vertexList = vertex_list_indexed(size, model.indices, *data)

    @property
    def model(self):
        return self._model

    @model.setter
    def model(self, model):
        assert isinstance(model, Model)
        self._model = model
        self._updateVertexList()

    def draw(self):
        if self._vertexList is None: return
        self._vertexList.draw(GL_TRIANGLES)

