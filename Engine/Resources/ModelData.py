# File name: ModelData.py
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

from Base import Resource

class ModelData(Resource):
    """
    ModelData provides an interface to iterables pointing to 3D geometric
    data that has been loaded from resources (e.g. model files exported
    with blender).
    """

    def __init__(self, fileLike, loader=OBJModelLoader):
        super(ModelData, self).__init__()
        assert isinstance(loader, ModelLoader)
        self._data = loader.load(fileLike)

    def __getattr__(self, name):
        if name in self._data: return self._data[name]
        return None

class ModelDataLoader(object):
    """
    ModelDataLoader is the base class of all model data loaders.
    """

    def __init__(self):
        super(ModelLoader, self).__init__()
        assert self.__class__ != ModelDataLoader

    def load(self, iterable):
        """
        This method has to be implemented by all ModelLoader subclasses and
        returns a dictionary containing the data.
        """

class OBJModelDataLoader(ModelDataLoader):
    """
    The OBJModelLoader provides a ModelLoader that is capable of loading
    wavefront obj formatted geometric data.
    """

    def __init__(self):
        # FIXME: make singleton
        super(OBJModelDataLoader, self).__init__()

    def _packVertexData(self, faces, vertices, normals, texcoords):
        """
        Pack data in well formed lists as expected by ModelData.
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
        if None in packed_texcoords: packed_texcoords = []
        if None in packed_normals: packed_normals = []
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
        # pack data into desired format and return it
        data = self._packVertexData(faces, vertices, normals, texcoords)
        return {'indices': data[0], 'vertices': data[1],
            'normals': data[2], 'texCoords': data[3]}

