# File name: test_Fill.py
# This file is part of: pyengine
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
# For feedback and questions about pyengine please e-mail one of the
# authors named in the AUTHORS file.
########################################################################
from __future__ import unicode_literals, print_function, division
from our_future import *

import unittest

from Fill import Colour, FakeImage, Gradient, Stretch, Repeat
from Rect import Rect
from FaceBuffer import FaceBuffer

class FillTest(unittest.TestCase):
    def assertFaceBuffer(self, texture_key, vertices, colours=None, texcoords=None):
        geometry = self.buffer.get_geometry()
        vertexcount = len(vertices) // 2
        if colours is None:
            colours = FaceBuffer.NullColours[:4]*vertexcount
        if texcoords is None:
            texcoords = FaceBuffer.NullTexCoords[:4]*vertexcount
        self.assertIn(texture_key, geometry)
        bvertices, bcolours, btexcoords = geometry[texture_key]
        self.assertSequenceEqual(vertices, bvertices[1])
        self.assertSequenceEqual(colours, bcolours[1])
        self.assertSequenceEqual(texcoords, btexcoords[1])

    def setUp(self):
        self.buffer = FaceBuffer()

    def tearDown(self):
        del self.buffer

class ColourTest(unittest.TestCase):
    def test_init(self):
        colour = Colour()
        self.assertEqual(colour, Colour(0., 0., 0., 1.))

    def test_init_invalid(self):
        self.assertRaises(ValueError, Colour, -1., 0., 0., 1.)
        self.assertRaises(ValueError, Colour, 0., -1., 0., 1.)
        self.assertRaises(ValueError, Colour, 0., 0., -1., 1.)
        self.assertRaises(ValueError, Colour, 0., 0., 0., -1.)
        self.assertRaises(ValueError, Colour, 0., 0., 0., 2.)
