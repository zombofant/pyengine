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
from OpenGL.GL import *
import sys

"""
The Model class stores 3D model data like vertices, normals,
faces and texture coordinates, rotation, position, scale and so on.
It also provides methods to modify and render the model using OpenGL.
"""
class Model(object):

    def __init__(self):
        self._clear()

    def _clear(self):
        self._vertices, self._faces, self_normals, self._faces = [], [], [], []
        self._posX, self._posY, self._posZ = 0., 0., 0.
        self._angleX, self._angleY, self._angleZ = 0., 0., 0.
        self._scaleX, self._scaleY, self._scaleZ = 1., 1., 1.

    def _transform(self):
        glTranslatef(self._posX, self._posY, self._posZ)
        glRotatef(self._angleX, 1., 0., 0.)
        glRotatef(self._angleY, 0., 1., 0.)
        glRotatef(self._angleZ, 0., 0., 1.)
        glScalef(self._scaleX, self._scaleY, self._scaleZ)

    def render(self):
        # FIXME: implement VBOs
        glPushMatrix()
        self._transform()
        for face in self.faces:
            glBegin(GL_POLYGON)
            glColor3f(1,0,0)
            for ids in face:
                if ids[1] is not None:
                    texCoord = self.texCoords[ids[1]]
                    glTexCoord2f(texCoord[0], texCoord[1])
                if ids[2] is not None:
                    normal = self.normals[ids[2]]
                    glNormal3f(normal[0], normal[1], normal[2])
                vertex = self.vertices[ids[0]]
                glVertex3f(vertex[0], vertex[1], vertex[2])
            glEnd()
        glPopMatrix()

    @property
    def position(self):
        return [self._posX, self._posY, self._posZ]

    @position.setter
    def position(self, value):
        self._posX, self._posY, self._posZ = value

    @property
    def rotation(self):
        return [self._angleX, self._angleY, self._angleZ]

    @rotation.setter
    def rotation(self, value):
        self._angleX, self._angleY, self._angleZ = value

    @property
    def vertices(self):
        return self._vertices

    @property
    def normals(self):
        return self._normals

    @property
    def texcoords(self):
        return self._texcoords

    @property
    def faces(self):
        return self._faces


"""
The OBJModel provides a Model interface with support for loading
3d geometry data from wavefront obj formatted files.
See http://www.martinreddy.net/gfx/3d/OBJ.spec for a format specification.
"""
class OBJModel(Model):

    def __init__(self, filename=None):
        super(OBJModel, self).__init__()
        if filename is not None:
            self.loadFromFile(filename)

    def loadFromFile(self, filename):
        with open(str(filename), 'r') as objf:
            self.loadFromIterable(objf)

    def loadFromIterable(self, iterable):
        vertices, normals, texcoords, faces = [], [], [], []
        for line in iterable:
            if line[0] == '#' : continue
            parts = line.strip().split(' ')
            if parts[0] == 'v':
                vertices.append([float(x) for x in parts[1:]])
            elif parts[0] == 'vn':
                normals.append([float(x) for x in parts[1:]])
            elif parts[0] == 'vt':
                texcoords.append([float(x) for x in parts[1:]])
            elif parts[0] == 'f':
                face = []
                for fcomps in (part.split('/') for part in parts[1:]):
                    fcomp = []
                    for j in range(0,3):
                        if len(fcomps) <= j or fcomps[j] == '':
                            fcomp.append(None)
                        else:
                            fcomp.append(int(fcomps[j])-1)
                    face.append(fcomp)
                faces.append(face)
            else:
                print("FIXME: Unhandled obj data: %s" % line, file=sys.stderr)
        self._vertices = vertices
        self._normals = normals
        self._texcoords = texcoords
        self._faces = faces

