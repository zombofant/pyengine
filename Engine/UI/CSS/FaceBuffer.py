# File name: FaceBuffer.py
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

import itertools
import iterutils

class FaceBuffer(object):
    NullColours = (
        1., 1., 1., 1.,
        1., 1., 1., 1.,
        1., 1., 1., 1.
    )
    NullTexCoords = (
        0., 0.,
        0., 0.,
        0., 0.
    )
    
    def __init__(self):
        self._faces = {} # grouped by texture
    
    def add_face(self, vertices, colour=None, texture=None):
        """
        Adds a face to the FaceBuffer.

        *vertices* must be a 3-tuple of 2-tuples each representing
        the 2d coordinates of the face triangle.

        *colour* can be a 3-tuple of 4-tuples each representing the
        vertex colour or None for default values.

        *texture* can be a 2-tuple where the first element is a
        reference to the Texture2D instance used and the second element
        is a 3-tuple of 2-tuples representing the uv-coordinates at
        each vertex.

        If you want to add quads, you can use the *add_quad* method.

        Data is expected to be passed in CCW order.
        """
        lists = self._faces.setdefault(texture[0] if texture is not None else None, (("v2f", []),("c4f", []),("t2f", [])))
        lists[0][1].extend(map(float, itertools.chain.from_iterable(vertices)))
        lists[1][1].extend(map(float, itertools.chain.from_iterable(colour)) if colour is not None else self.NullColours)
        lists[2][1].extend(map(float, itertools.chain.from_iterable(texture[1])) if texture is not None else self.NullTexCoords)

    def add_faces(self, texture, vertices, colour=None, texcoords=None):
        """
        Adds multiple faces to the FaceBuffer.

        Expects *vertices* and *colour* to be iterables of their
        respective *add_face* argument.

        *texture* must be the Texture these faces are assigned to, while
        *texcoords* must be the list of 3-tuples of 2-tuples where
        the texture coordinates are stored in.

        *texture* can be None if no texture is assigned and texcoords
        is None.
        """
        lists = self._faces.setdefault(texture, (("v2f", []),("c4f", []),("t2f", [])))
        lists[0][1].extend(iterutils.yield_count(map(float, iterutils.flatten_two_levels(vertices)), self))
        # len gets set by iterutils.yield_count
        vertexcount = self.len >> 1
        lists[1][1].extend(
            map(float, iterutils.flatten_two_levels(colour)) if colour is not None else itertools.chain.from_iterable(itertools.repeat(self.NullColours[:4], vertexcount))
        )
        lists[2][1].extend(
            map(float, iterutils.flatten_two_levels(texcoords)) if texcoords is not None else itertools.chain.from_iterable(itertools.repeat(self.NullTexCoords[:2], vertexcount))
        )

    def add_quad(self, vertices, colour=None, texture=None):
        """
        Adds a quad to the FaceBuffer by splitting it in two triangles.

        The arguments are the same as for *add_face*, except that the
        3-tuples for vertex-specific data are 4-tuples.

        Data is expected to be passed in CCW order.

        This is rather inefficient as all tuples have to be
        reconstructed, so one should feel encouraged to use *add_face*
        directly.

        No assumptions should be made about where the split in the
        quad will be.
        """
        vertex0, vertex1, vertex2, vertex3 = vertices
        if colour is not None:
            colour0, colour1, colour2, colour3 = colour
            new_colour0 = (colour0, colour1, colour3)
            new_colour1 = (colour1, colour3, colour2)
        else:
            new_colour0 = None
            new_colour1 = None
        if texture is not None:
            texobj, (texture0, texture1, texture2, texture3) = texture
            new_texture0 = (texobj, (texture0, texture1, texture3))
            new_texture1 = (texobj, (texture1, texture3, texture2))
        else:
            new_texture0 = None
            new_texture1 = None
        
        self.add_face(
            (vertex0, vertex1, vertex3),
            new_colour0,
            new_texture0
        )
        self.add_face(
            (vertex1, vertex3, vertex2),
            new_colour1,
            new_texture1
        )

    def __iadd__(self, other):
        if not isinstance(other, FaceBuffer):
            return NotImplemented
        for key, value in other._both_faces.iteritems():
            self._faces.setdefault(key, []).extend(value)

    def get_geometry(self):
        """
        Returns the geometry saved so far.

        Returns a dict where each key represents a used texture. Each
        value is a tuple of lists, each element being a pyglet-compliant
        vertex data stream (i.e. beginning with a vertex attribute
        specifier, followed by data).
        
        The first list refers to v2f vertex data, the second to c4f
        colour data and the third to t2f texture coordinate data.

        This is a reference and no copy, so if the data is modified,
        it should be copied first.
        """
        return self._faces
