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
from OpenGL.GL import GL_TRIANGLES
from pyglet.graphics import vertex_list_indexed
import sys

# Try to load pyglet. If it fails, we provide a fallback since this
# class will be used by the dedicated server application which does
# not have a dependency on pyglet.
try:
    from pyglet.graphics import vertex_list_indexed
except ImportError:
    pass

class Model(object):
    """
    The Model class stores 3D model data like vertices, normals,
    faces and texture coordinates, rotation, position, scale and so on.
    It also provides methods to modify and render the model using OpenGL.
    """

    def __init__(self):
        self._vertexList = None
        self._clear()

    def _clear(self):
        if self._vertexList is not None:
            del self._vertexList
            self._vertexList = None
        self._faces = []

    def _setVertexData(self, indices, vertices, normals=None, texCoords=None):
        """
        Set the vertex data. At least indices and vertices have to be passed
        in order to construct the vertex list. Please not that a complete new
        vertex list is created when calling this method. This means all
        previous vertex data will be dropped.
        Each vertex at a given position in the list of vertices corresponds to
        the normal/texCoord at this position in the respective list (if not None)
        """
        self._clear()
        size = len(vertices) // 3
        data = [('v3f/static', list(vertices))]
        if normals is not None:
            data.append(('n3f/static', list(normals)))
        if texCoords is not None:
            data.append(('t2f/static', list(texCoords)))
        self._vertexList = vertex_list_indexed(size, list(indices), *data)

    def draw(self):
        if self._vertexList is None: return
        self._vertexList.draw(GL_TRIANGLES)

    @property
    def vertices(self):
        if self._vertexList is None: return None
        return self._vertexList.vertices

    @property
    def indices(self):
        if self._vertexList is None: return None
        return self._vertexList.indices



class ModelLoader(object):

    def __init__(self):
        super(ModelLoader, self).__init__()

    def load(self, iterable):
        """
        This method has to be implemented by all ModelLoader subclasses and
        returns an iterable containing faces.
        """

class OBJModelLoader(ModelLoader):
    """
    The OBJModelLoader provides a ModelLoader that is capable of loading
    wavefront obj formatted geometric data.
    """

    def __init__(self):
        super(OBJModelLoader, self).__init__()

    def _packVertexData(self, faces, vertices, normals, texcoords):
        """
        Pack data in lists as expected by Model._setVertexData() and call it.
        """
        size = len(vertices) // 3
        packed_normals, packed_texcoords = [None]*size*3, [None]*size*2
        indices = []
        for comps in [elems for f in faces for elems in f]:
            vpos = comps[0]
            indices.append(vpos)
            if comps[1] is not None:
                tpos = comps[1]
                packed_texcoords[vpos*2:vpos*2+2] = texcoords[tpos*2:tpos*2+2]
            if comps[2] is not None:
                npos = comps[2]
                packed_normals[vpos*3:vpos*3+3] = normals[npos*3:npos*3+3]
        if None in packed_texcoords: packed_texcoords = None
        if None in packed_normals: packed_normals = None
        return (indices, vertices, packed_normals, packed_texcoords)

    def load(self, iterable):
        """
        The actual geometry data loader.
        Note: All faces in the obj data are expected to be triangles.
        """
        vertices, normals, texcoords, faces = [], [], [], []
        for line in iterable:
            if len(line) < 1 : continue
            if line[0] == '#' : continue
            parts = line.strip().split(' ')
            if parts[0] == 'v':
                vertices.extend([float(x) for x in parts[1:]])
            elif parts[0] == 'vn':
                normals.extend([float(x) for x in parts[1:]])
            elif parts[0] == 'vt':
                texcoords.extend([float(x) for x in parts[1:]])
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
                #print("FIXME: Unhandled obj data: %s" % line, file=sys.stderr)
                pass
        if len(faces) < 1:
            raise Exception('No faces found in geometric data!')
        return self._packVertexData(faces, vertices, normals, texcoords)

