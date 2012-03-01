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
import scipy as sp

"""
The core classes of the SceneGraph management system.
"""

class Transformation(object):

    def __init__(self):
        super(Transformation, self).__init__()
        self.reset()

    def reset(self):
        self.setIdentity()
        self.setDefaultScale()
        self.setDefaultTransformation()

    def setIdentity(self):
        self._mRotate = sp.mat([[1,0,0], [0,1,0], [0,0,1]])
        self._isIdentity = True

    def setDefaultScale(self):
        self._mScale = sp.mat([1., 1., 1.])
        self._isDefaultScale = True

    def setDefaultTransformation(self):
        self._mTranslate = sp.mat([[1,0,0], [0,1,0], [0,0,1]])

    def applyForward(self, mInput):
        if self.isIdentity:
            return mInput
        mOutput = mInput
        for j in range(0,3):
            mOutput[j] *= self._mScale[j]
        mOutput = mRotate * mOutput
        mOutput += mTranslate
        return mOutput

    def applyInverse(self, mInput):
        if self.isIdentity:
            return mInput
        if self.isDefaultScale:
            return ((mInput - self._mTranslate)*self._mRotate)/self._mScale
        mOutput = (mInput - self._mTranslate)*self._mRotate
        sXY = self._mScale[0]*self._mscale[1]
        sXZ = self._mScale[0]*self._mscale[2]
        sYZ = self._mScale[1]*self._mScale[2]
        invDet = 1./(sXY*self._mScale[2])
        mOutput[0] *= invDet*sYZ
        mOutput[1] *= invDet*sXZ
        mOutput[2] *= invDet*sXY
        return mOutput

    def product(self, transformA, transformB):
        raise NotImplementedError()

    def inverse(self, transform):
        raise NotImplementedError()

    @property
    def isIdentity(self):
        return self._isIdentity

    @property
    def isDefaultScale(self):
        return self._isDefaultScale

    @property
    def rotate(self):
        return self._mRotate

    @rotate.setter
    def rotate(self, value):
        self._mRotate = value
        self._mIsIdentity = False

    @property
    def translate(self):
        return self._mTranslate

    @translate.setter
    def translate(self, value):
        self._mTranslate = value

    @property
    def scale(self):
        return self._mScale

    @rotate.setter
    def scale(self, value):
        self._mScale = value
        self._isDefaultScale = False

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
        return self._transWord

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

    def render(self, deltaT):
        self._rootNode.updateGeometry(deltaT)

    @property
    def rootNode(self):
        return self._rootNode

