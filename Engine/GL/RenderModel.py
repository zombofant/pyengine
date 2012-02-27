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
from OpenGL.GL import GL_TRIANGLES
from Engine.Resources.Manager import ResourceManager

import pyglet

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
        self._batch = pyglet.graphics.Batch()
        if model is not None:
            self._copyFromModel(model)

    def _copyFromModel(self, model):
        assert isinstance(model, Model)
        if model.faces is not None:
            self._faces = model.faces
        if model.materials is not None:
            self._materials = model.materials
        self.update()

    def _clear(self):
        pass

    def update(self):
        """
        Update the vertex list. At least indices and vertices have to be
        available in order to construct a vertex list. Please not that a
        complete new vertex list is created when calling this method. This
        means all previous vertex data will be dropped.
        """
        if self.faces is None: return
        self._clear()
        pos, nextMatSwitchIndex, matCount = 0, 0, 0
        materials = self._materials
        group = None
        if materials is None: materials = [['(null)',0]]
        for material in materials:
            matCount += 1
            nextMatSwitchIndex = material[1]
            if material[0] == '(null)':
                group = None
            else:
                #load material
                mat = ResourceManager().require('/data/materials/%s.mtl' % material[0])
                #set material group
                group = mat.stateGroup
            if matCount >= len(materials):
                nextMatSwitchIndex = len(self.faces)
            if pos == nextMatSwitchIndex:
                continue
            vertices, normals, texCoords = [], [], []
            for face in self.faces[pos:nextMatSwitchIndex]:
                vertices.extend([x for y in face[0:1] for x in y])
                normals.extend([x for y in face[1:2] for x in y])
                texCoords.extend([x for y in face[2:3] for x in y])
            size = (nextMatSwitchIndex - pos) * 3
            data = [('v3f/static', vertices)]
            if normals is not None:
                data.append(('n3f/static', normals))
            if texCoords is not None:
                data.append(('t2f/static', texCoords))
            self._batch.add(size, GL_TRIANGLES, group, *data)
            pos = nextMatSwitchIndex

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

    def draw(self):
        """
        Draw the RenderModel using OpenGL.
        Call this in your render-loop to render the underlying model.
        """
        self._batch.draw()

