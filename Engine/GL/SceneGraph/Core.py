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

_IDENTITY = np.identity(4, dtype=np.float32)

class Transformation(object):

    def __init__(self):
        super(Transformation, self).__init__()
        self.reset()

    def updateMatrix(self):
        self._matrix = self._mTranslate*self._mRotate*self._mScale
        self._transposedMatrix = self._matrix.T.copy(order='C')

    def reset(self):
        self.setIdentity()

    def setIdentity(self):
        self._matrix = _IDENTITY
        self._transposedMatrix = _IDENTITY
        self._mTranslate = _IDENTITY
        self._mRotate = _IDENTITY
        self._mScale = _IDENTITY
        self._isIdentity = True
        self._isUnitScale = True

    def applyForward(self, mInput):
        pass

    def applyInverse(self, mInput):
        pass

    def product(self, transA, transB):
        if transA.IsIdentity or transB.IsIdentity:
            if transA.IsIdentity:
                self._matrix = transB.Matrix
                self._transposedMatrix = transB.TransposedMatrix
            else:
                self._matrix = transA.Matrix
                self._transpedMatrix = transA.TransposedMatrix
        else:
            self._matrix = transA.Matrix * transB.Matrix
            self._transposedMatrix = transA.TransposedMatrix * transB.TransposedMatrix
        self._isIdentity = False

    def scale(self, scaleVectorL):
        self._mScale = self._mScale * np.matrix([[scaleVectorL[0],0.,0.,0.],
            [0.,scaleVectorL[1],0.,0.],[0.,0.,scaleVectorL[2],0.],[0.,0.,0.,1.]],
            dtype=np.float32)
        self._isIdentity = False

    def translate(self, vectorL):
        self._mTranslate = self._mTranslate * np.matrix([[1.,0.,0.,vectorL[0]],
            [0.,1.,0.,vectorL[1]], [0.,0.,1.,vectorL[2]], [0.,0.,0.,1.]],
            dtype=np.float32)
        self._isIdentity = False

    def rotate(self, angle, vectorL, degrees=True):
        if degrees:
            angle = angle*np.pi/180.
        axisX, axisY, axisZ = vectorL[0], vectorL[1], vectorL[2]
        absol = np.sqrt(axisX*axisX+axisY*axisY+axisZ*axisZ)
        if absol == 0:
            return
        axisX /= absol
        axisY /= absol
        axisZ /= absol
        c = np.cos(angle)
        s = np.sin(angle)
        o = 1. - c
        self._mRotate = self._mRotate * np.matrix([
            [c+axisX*axisX*o, axisX*axisY*o-axisZ*s, axisX*axisZ*o+axisY*s, 0.],
            [axisY*axisX*o+axisZ*s, c+axisY*axisY*o, axisY*axisZ*o-axisX*s, 0.],
            [axisZ*axisX*o-axisY*s, axisZ*axisY*o+axisX*s, c+axisZ*axisZ*o, 0.],
            [0., 0., 0., 1.]], dtype=np.float32)
        self._isIdentity = False

    @property
    def Matrix(self):
        return self._matrix

    @property
    def TransposedMatrix(self):
        return self._transposedMatrix

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
        self._localTrans = Transformation()
        self._worldTrans = Transformation()

    def updateGeometry(self, deltaT, initiator=True):
        self.updateWorldData(deltaT, False)
        #updateWorldBounds()
        #if initiator:
        #    self.propagateBoundsToRoot()

    def updateBounds(self, initiator=True):
        raise NotImplementedError()

    def updateWorldData(self, deltaT, initiator=True):
        if self.parent is not None:
            self.LocalTrans.updateMatrix()
            self.WorldTrans.product(self.parent.WorldTrans, self.LocalTrans)

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
    def LocalTrans(self):
        return self._localTrans

    @LocalTrans.setter
    def LocalTransformation(self, value):
        self._localTrans = value

    @property
    def WorldTrans(self):
        return self._worldTrans

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
        GL.glLoadMatrixf(self.WorldTrans.TransposedMatrix)
        super(Node, self).draw()
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

