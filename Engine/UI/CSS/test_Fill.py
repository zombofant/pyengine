# File name: test_Fill.py
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

import unittest

from Fill import Colour, FakeImage, Gradient, Stretch, Repeat
from Rect import Rect
from FaceBuffer import FaceBuffer

class FillTest(unittest.TestCase):
    def assertFaceBuffer(self, textureKey, vertices, colours=None, texCoords=None):
        geometry = self.buffer.getGeometry()
        vertexCount = len(vertices) // 2
        if colours is None:
            colours = FaceBuffer.NullColours[:4]*vertexCount
        if texCoords is None:
            texCoords = FaceBuffer.NullTexCoords[:4]*vertexCount
        self.assertIn(textureKey, geometry)
        bVertices, bColours, bTexCoords = geometry[textureKey]
        self.assertSequenceEqual(vertices, bVertices[1])
        self.assertSequenceEqual(colours, bColours[1])
        self.assertSequenceEqual(texCoords, bTexCoords[1])

    def setUp(self):
        self.buffer = FaceBuffer()

    def tearDown(self):
        del self.buffer

class ColourTest(unittest.TestCase):
    def test_init(self):
        colour = Colour()
        self.assertEqual(colour, Colour(0., 0., 0., 1.))

    def test_initInvalid(self):
        self.assertRaises(ValueError, Colour, -1., 0., 0., 1.)
        self.assertRaises(ValueError, Colour, 0., -1., 0., 1.)
        self.assertRaises(ValueError, Colour, 0., 0., -1., 1.)
        self.assertRaises(ValueError, Colour, 0., 0., 0., -1.)
        self.assertRaises(ValueError, Colour, 0., 0., 0., 2.)

class FakeImageFill(FillTest):
    def setUp(self):
        super(FakeImageFill, self).setUp()
        w, h = 256, 256
        rw, rh = 16., 16.
        self.uv = (0., 0., rw/w, rh/h)
        self.image = FakeImage(
            (w, h),
            rect=Rect(0, 0, 16, 16)
        )

    def tearDown(self):
        del self.image
        super(FakeImageFill, self).tearDown()
    
    def test_stretch_stretch(self):
        self.image.RepeatX = Stretch
        self.image.RepeatY = Stretch
        self.image.geometryForRect(Rect(0, 0, 32, 32), self.buffer)
        u1, v1, u2, v2 = self.uv
        self.assertFaceBuffer(None,
            [
                0., 0.,
                0., 32.,
                32., 0.,
                
                0., 32.,
                32., 32.,
                32., 0.
            ],
            None,
            [
                u1, v1,
                u1, v2,
                u2, v1,

                u1, v2,
                u2, v2,
                u2, v1
            ]
        )

    def test_repeat_stretch(self):
        self.image.RepeatX = Repeat
        self.image.RepeatY = Stretch
        self.image.geometryForRect(Rect(16, 16, 64, 32), self.buffer)
        u1, v1, u2, v2 = self.uv
        self.assertFaceBuffer(
            None,
            [
                16., 16.,
                16., 32.,
                32., 16.,

                32., 16.,
                16., 32.,
                32., 32.,


                32., 16.,
                32., 32.,
                48., 16.,

                48., 16.,
                32., 32.,
                48., 32.,


                48., 16.,
                48., 32.,
                64., 16.,

                64., 16.,
                48., 32.,
                64., 32.,
            ],
            None,
            [
                u1, v1,
                u1, v2,
                u2, v1,

                u2, v1,
                u1, v2,
                u2, v2,

                
                u1, v1,
                u1, v2,
                u2, v1,

                u2, v1,
                u1, v2,
                u2, v2,

                
                u1, v1,
                u1, v2,
                u2, v1,

                u2, v1,
                u1, v2,
                u2, v2,
            ]
        )

    def test_repeat_repeat(self):
        self.image.RepeatX = Repeat
        self.image.RepeatY = Repeat
        self.image.geometryForRect(Rect(0, 0, 32, 32), self.buffer)
        u1, v1, u2, v2 = self.uv
        self.assertFaceBuffer(
            None,
            [
                 0.,  0.,
                 0., 16.,
                16.,  0.,

                16.,  0.,
                 0., 16.,
                16., 16.,


                16.,  0.,
                16., 16.,
                32.,  0.,

                32.,  0.,
                16., 16.,
                32., 16.,


                 0., 16.,
                 0., 32.,
                16., 16.,

                16., 16.,
                 0., 32.,
                16., 32.,
                
                
                16., 16.,
                16., 32.,
                32., 16.,

                32., 16.,
                16., 32.,
                32., 32.,
            ],
            None,
            [
                u1, v1,
                u1, v2,
                u2, v1,

                u2, v1,
                u1, v2,
                u2, v2,

                
                u1, v1,
                u1, v2,
                u2, v1,

                u2, v1,
                u1, v2,
                u2, v2,

                
                u1, v1,
                u1, v2,
                u2, v1,

                u2, v1,
                u1, v2,
                u2, v2,

                
                u1, v1,
                u1, v2,
                u2, v1,

                u2, v1,
                u1, v2,
                u2, v2,
            ]
        )

    def test_strech_repeat_partly(self):
        self.image.RepeatX = Stretch
        self.image.RepeatY = Repeat
        self.image.geometryForRect(Rect(0, 0, 16, 24), self.buffer)
        u1, v1, u2, v2 = self.uv
        vhalf = v1 + (v2 - v1) / 2.
        self.assertFaceBuffer(
            None,
            [
                 0.,  0.,
                 0., 16.,
                16.,  0.,

                16.,  0.,
                 0., 16.,
                16., 16.,


                 0., 16.,
                 0., 24.,
                16., 16.,

                16., 16.,
                 0., 24.,
                16., 24.,
            ],
            None,
            [
                u1, v1,
                u1, v2,
                u2, v1,

                u2, v1,
                u1, v2,
                u2, v2,

                
                u1, v1,
                u1, vhalf,
                u2, v1,

                u2, v1,
                u1, vhalf,
                u2, vhalf,
            ]
        )
        
