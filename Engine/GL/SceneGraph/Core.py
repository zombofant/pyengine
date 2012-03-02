# File name: Core.py
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
import OpenGL.GL as GL
import numpy as np
import scipy as sp

"""
The core classes of the SceneGraph management system.
"""

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
        return self._boundingVolume


class Transformation(object):

    def __init__(self):
        super(Transformation, self).__init__()
        self.reset()

    def reset(self):
        self.setIdentity()

    def setIdentity(self):
        self._mTransformation = np.identity(4, dtype=np.float32)
        self._mTransposed = self._mTransformation
        self._isIdentity = True
        self._isUnitScale = True

    def applyForward(self, mInput):
        pass

    def applyInverse(self, mInput):
        pass

    def product(self, transA, transB):
        self._mTransformation = transA.Transformation * transB.Transformation
        self._mTransposed = self._mTransformation.T.copy(order='C')

    def scale(self, scaleX, scaleY, scaleZ):
        self._mTransformation = self._mTransformation * np.matrix(
            [[scaleX, 0., 0., 0.], [0., scaleY, 0., 0.],
            [0., 0., scaleZ, 0.], [0., 0., 0., 1.]], dtype=np.float32)
        self._mTransposed = self._mTransformation.T.copy(order='C')

    def translate(self, x, y, z):
        self._mTransformation = self._mTransformation * np.matrix(
            [[1., 0., 0., x], [0., 1., 0., y],
            [0., 0., 1., z], [0., 0., 0., 1.]], dtype=np.float32)
        self._mTransposed = self._mTransformation.T.copy(order='C')

    def rotate(self, angle, axisX, axisY, axisZ, degrees=True):
        if degrees:
            angle = angle*np.pi/180.
        absol = np.sqrt(axisX*axisX+axisY*axisY+axisZ*axisZ)
        if absol > 0:
            axisX /= absol
            axisY /= absol
            axisZ /= absol
        else:
            return
        c = np.cos(angle)
        s = np.sin(angle)
        o = 1. - c
        self._mTransformation = self._mTransformation * np.matrix([
            [c+axisX*axisX*o, axisX*axisY*o-axisZ*s, axisX*axisZ*o+axisY*s, 0.],
            [axisY*axisX*o+axisZ*s, c+axisY*axisY*o, axisY*axisZ*o-axisX*s, 0.],
            [axisZ*axisX*o-axisY*s, axisZ*axisY*o+axisX*s, c+axisZ*axisZ*o, 0.],
            [0., 0., 0., 1.]], dtype=np.float32)
        self._mTransposed = self._mTransformation.T.copy(order='C')

    @property
    def Transformation(self):
        return self._mTransformation

    @property
    def Transposed(self):
        return self._mTransposed

    @property
    def IsIdentity(self):
        return self._isIdentity

    @property
    def IsUnitScale(self):
        return self._isUnitScale

class Spatial(object):

    def __init__(self):
        super(Spatial, self).__init__()
        self.parent = None
        self._localTransformation = Transformation()
        self._worldTransformation = Transformation()

    def updateGeometry(self, deltaT, initiator=True):
        self.updateWorldData(deltaT, False)
        #updateWorldBounds()
        #if initiator:
        #    self.propagateBoundsToRoot()

    def updateBounds(self, initiator=True):
        raise NotImplementedError()

    def updateWorldData(self, deltaT, initiator=True):
        if self.parent is not None:
            self.WorldTransformation.product(self.parent.WorldTransformation,
                self.LocalTransformation)

    def onDraw(self):
        pass

    def draw(self):
        pass

    @property
    def parent(self):
        return self._parent

    @parent.setter
    def parent(self, value):
        if value is not None:
            assert isinstance(value, Spatial)
        self._parent = value

    @property
    def LocalTransformation(self):
        return self._localTransformation

    @LocalTransformation.setter
    def LocalTransformation(self, value):
        self._localTransformation = value

    @property
    def WorldTransformation(self):
        return self._worldTransformation

class Node(Spatial):

    def __init__(self):
        super(Node, self).__init__()
        self._children = []
        self._numOfChildren = 0

    def addChild(self, child):
        if child not in self._children:
            child.parent = self
            self._children.append(child)
            self._numOfChildren += 1
            child.updateWorldData(0)

    def removeChild(self, child):
        self._children.remove(child)
        self._numOfChildren -= 1

    def updateWorldData(self, deltaT, initiator=True):
        super(Node, self).updateWorldData(deltaT, initiator)
        for child in self.children:
            child.updateGeometry(deltaT, False)

    def onDraw(self):
        for child in self.children:
            child.onDraw()

    def draw(self):
        super(Node, self).draw()
        GL.glLoadMatrixf(self.WorldTransformation.Transposed)
        for child in self.children:
            child.draw()

    @property
    def children(self):
        return self._children

    @property
    def numberOfChildren(self):
        return self._numOfChildren

class SceneGraph(object):

    def __init__(self):
        super(SceneGraph, self).__init__()
        self._rootNode = Node()

    def update(self, deltaT):
        self._rootNode.updateGeometry(deltaT)

    def renderScene(self, node=None):
        if node is None: node = self.rootNode
        node.onDraw()
        node.draw()

    @property
    def rootNode(self):
        return self._rootNode

