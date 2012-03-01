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
import numpy as np
import scipy as sp

"""
The core classes of the SceneGraph management system.
"""

class Transformation(object):

    def __init__(self):
        super(Transformation, self).__init__()
        self._mScale, self._mTranslate, self._mRotate = 0, 0, 0
        self.reset()

    def _updateTransformation(self):
        m = self._mScale*np.identity(4,dtype=np.float32)*self._mTranslate*self._mRotate
        self._mTransformation = m.T.copy(order='C_CONTIGUOUS')

    def reset(self):
        self.setIdentity()
        self.setDefaultScale()

    def setIdentity(self):
        self._mRotate = np.identity(4, dtype=np.float32)
        self._mTranslate = np.identity(4, dtype=np.float32)
        self._isIdentity = True

    def setDefaultScale(self):
        self._mScale = np.identity(4, dtype=np.float32)
        self._isDefaultScale = True

    def applyForward(self, mInput):
        pass

    def applyInverse(self, mInput):
        pass

    def product(self, matrixA, matrixB):
        self._mTransformation = matrixA * matrixB

    def scale(self, scaleX, scaleY, scaleZ):
        self._mScale = np.matrix([[scaleX, 0., 0., 0.], [0., scaleY, 0., 0.],
            [0., 0., scaleZ, 0.], [0., 0., 0., 1.]], dtype=np.float32)
        self._updateTransformation()

    def translate(self, x, y, z):
        self._mTranslate = np.matrix([[1., 0., 0., x], [0., 1., 0., y],
            [0., 0., 1., z], [0., 0., 0., 1.]], dtype=np.float32)
        self._updateTransformation()

    def rotate(self, angle, axisX, axisY, axisZ):
        c = np.cos(angle)
        s = np.sin(angle)
        self._mRotate = np.matrix([
            [c+axisX*axisX*(1.-c),axisX*axisY*(1-c)-axisZ*s, axisX*axisZ*(1-c)+axisY*s, 0.],
            [axisY*axisX*(1-c)+axisZ*s, c+axisY*axisY*(1-c), axisY*axisZ*(1-c)-axisX*s, 0.],
            [axisZ*axisX*(1-c)-axisY*s, axisZ*axisY*(1-c)+axisX*s, c+axisZ*axisZ*(1-c), 0.],
            [0., 0., 0., 1.]], dtype=np.float32)
        self._updateTransformation()

    @property
    def transformation(self):
        return self._mTransformation

    @property
    def isIdentity(self):
        return self._isIdentity

    @property
    def isDefaultScale(self):
        return self._isDefaultScale

class Spatial(object):

    def __init__(self):
        super(Spatial, self).__init__()
        self.parent = None
        self.transLocal = Transformation()
        self.transWorld = Transformation()

    def updateGeometry(self, deltaT, initiator=True):
        self.updateWorldData(deltaT, False)
        #updateWorldBounds()
        #if initiator:
        #    self.propagateBoundsToRoot()

    def updateBounds(self, initiator=True):
        raise NotImplementedError()

    def updateWorldData(self, deltaT, initiator=True):
        if self.parent is not None:
            self.transWorld.product(self.parent.transWorld, self.transLocal)

    def onDraw(self):
        pass

    def draw(self):
        pass

    def scale(self, x, y, z):
        self.transLocal.scale(x, y, z)

    def translate(self, x, y, z):
        self.transLocal.translate(x, y, z)

    def rotate(self, angle, axisX, axisY, axisZ):
        self.transLocal.rotate(angle, axisX, axisY, axisZ)

    @property
    def parent(self):
        return self._parent

    @parent.setter
    def parent(self, value):
        if value is not None:
            assert isinstance(value, Spatial)
        self._parent = value

    @property
    def transLocal(self):
        return self._transLocal

    @transLocal.setter
    def transLocal(self, value):
        self._transLocal = value

    @property
    def transWorld(self):
        return self._transWorld

    @transWorld.setter
    def transWorld(self, value):
        self._transWorld = value

class Node(Spatial):

    def __init__(self):
        super(Node, self).__init__()
        self._children = []
        self._numOfChildren = 0

    def addChild(self, child):
        if child not in self._children:
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

