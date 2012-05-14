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

from CUni.SceneGraph import Leaf
from CUni.GL import VertexFormat, GeometryBuffer, GeometryBufferView
from Engine.Model import Model
from OpenGL.GL import *
from Engine.Resources.Manager import ResourceManager

class RenderModel(Model, Leaf):
    """
    This class extends the model class with GL methods in order to provide
    a convenient renderable representation of the model.
    """

    def __init__(self, **kwargs):
        """
        Initialize a RenderModel.
        """
        super(RenderModel, self).__init__(**kwargs)
        # FIXME/pyglet
        self._batch = []
        self._vFormat = VertexFormat("v:3;t0:2;n:3")
        self.update()
 
    def _copy(self, other):
        """
        Copy data from another (Render)Model instance.
        """
        super(RenderModel, self)._copy(other)
        self.update()

    @classmethod
    def fromModel(cls, model):
        """
        Takes a Model instance and returns a RenderModel instance as a
        renderable representation of the given model.
        """
        assert isinstance(model, Model)
        rmodel = RenderModel()
        rmodel._copy(model)
        return rmodel

    def update(self):
        """
        Take the faces of this model and put the vertex data into vertex
        lists in the object's batch. This prepares the data to be rendered
        using OpenGL.
        You have to call this method if the underlying vertex or face data
        has been changed in order to make the changes known to the renderer.
        """
        if len(self.PackedFaces) < 1: return
        pos, nextMatSwitchIndex, matCount = 0, 0, 0
        materials = self.Materials
        group = None
        self._batch = []
        if materials is None: materials = []
        materials.append(['(null)',0])
        for material in materials:
            matCount += 1
            nextMatSwitchIndex = material[1]
            if matCount >= len(materials):
                nextMatSwitchIndex = len(self.PackedFaces)
            if pos < nextMatSwitchIndex:
                vertices, normals, texCoords = [], [], []
                for face in self.PackedFaces[pos:nextMatSwitchIndex]:
                    before = len(vertices)
                    vertices.extend((x for y in face[0:1] for x in y))
                    normals.extend((x for y in face[1:2] for x in y))
                    texCoords.extend((x for y in face[2:3] for x in y))
                size = (nextMatSwitchIndex - pos) * 3
                buf = GeometryBuffer(self._vFormat, GL_STATIC_DRAW)
                indices = buf.allocateVertices(size)
                bufView = GeometryBufferView(buf, indices)
                bufView.Vertex.set(vertices)
                if len(normals) > 0:
                    bufView.Normal.set(normals)
                if len(texCoords) > 0:
                    bufView.TexCoord(0).set(texCoords)
                # FIXME
                #self._batch.add(size, GL_TRIANGLES, group, *data)
                self._batch.append(buf)
                pos = nextMatSwitchIndex
            if material[0] == '(null)':
                group = None
            else:
                mat_filename = '%s.mtl' % material[0]
                # FIXME
                #group = ResourceManager().require(mat_filename).stateGroup
 
    def draw(self):
        """
        Draw the RenderModel using OpenGL.
        Call this in your render-loop to render the underlying model.
        """
        super(RenderModel, self).draw()
        print("DRAW")
        for buf in self._batch:
            print("Model DRAW")
            buf.draw();

