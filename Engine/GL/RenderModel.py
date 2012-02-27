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

class RenderModel(Model):
    """
    This class extends the model class with GL methods in order to provide
    a convenient renderable representation of the model.
    """

    def __init__(self, model=None):
        """
        Initialize a RenderModel.
        You may pass a Model or RenderModel instance. The constructor will
        then copy all model data from the given instance.
        You may also create an instance by using the class method fromModel().
        """
        super(RenderModel, self).__init__()
        self._vertexList = None
        if model is not None:
            self._copyFromModel(model)

    def _copyFromModel(self, model):
        assert isinstance(model, Model)
        self._indices = model.indices
        self._vertices = model.vertices
        if model.normals is not None:
            self._normals = model.normals
        if model.texCoords is not None:
            self._texCoords = model.texCoords
        self._updateVertexList()

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
        if None in (self.indices, self.vertices): return
        self._clear()
        size = len(self.vertices) // 3
        data = [('v3f/static', self.vertices)]
        if self.normals is not None:
            data.append(('n3f/static', self.normals))
        if self.texCoords is not None:
            data.append(('t2f/static', self.texCoords))
        self._vertexList = vertex_list_indexed(size, self.indices, *data)

    @classmethod
    def fromModel(cls, model):
        """
        Takes a given model and constructs a new RenderModel instance from it.
        Returns the freshly created instance of RenderModel.
        """
        assert isinstance(model, Model)
        renderModel = RenderModel()
        renderModel._copyFromModel(model)
        return renderModel

    def update(self):
        """
        This stores the model data in video memory and must be called everytime
        the underlying model data has been changed. This allows to change
        multiple data values and then commit the changes by calling update().
        """
        self._updateVertexList()

    def draw(self):
        """
        Draw the RenderModel using OpenGL.
        Call this in your render-loop to render the underlying model.
        """
        if self._vertexList is None: return
        self._vertexList.draw(GL_TRIANGLES)

