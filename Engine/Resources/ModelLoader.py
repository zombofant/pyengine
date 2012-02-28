# File name: ModelLoader.py
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

from Base import ResourceLoader
from Manager import ResourceManager
from Engine.Model import Model

# At server side we probably cannot import GL classes. Therefore we
# provide a fallback for this case:
try:
    from Engine.GL.RenderModel import RenderModel
    from Engine.GL.Material import Material
except ImportError:
    pass

class OBJModelLoader(ResourceLoader):
    """
    The OBJModelLoader provides a ModelLoader that is capable of loading
    wavefront obj formatted 3D model data.
    """

    def __init__(self):
        super(OBJModelLoader, self).__init__()
        self._supportedTargetClasses = [Model]
        try: # add support for RenderModel if available
            self._supportedTargetClasses.append(RenderModel)
        except NameError:
            pass
        self._defaultTargetClass = Model
        self._resourceTypes = ['obj']

    def load(self, fileLike, targetClass=Model):
        """
        The actual geometry data loader.
        Note: All faces in the obj data are expected to be triangulated.
        """
        faceCount = 0
        vertices, normals, texcoords, faces, materials = [], [], [], [], []
        for line in fileLike:
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
            elif parts[0] == 'usemtl':
                if len(parts) == 2:
                    materials.append([parts[1],len(faces)])
            else:
                #print("FIXME: Unhandled obj data: %s" % line, file=sys.stderr)
                pass
        if len(faces) < 1:
            raise Exception('No faces found in geometric data!')
        model = Model(faces=faces, vertices=vertices, normals=normals,
            texCoords=texcoords, materials=materials)
        if targetClass is Model:
            return model
        else:
            return RenderModel(model)

class MaterialLoader(ResourceLoader):
    """
    The MaterialLoader loads model material data.
    It loads exactly one material from the given fileLike and returns
    a Material instance constructed from the data.
    """

    def __init__(self, **kwargs):
        super(MaterialLoader, self).__init__(**kwargs)
        # if the Material class is missing, the loader does nothing 
        try:
            self._supportedTargetClasses = [Material]
            self._defaultTargetClass = Material
            self._resourceTypes = ['mtl']
        except NameError:
            self._supportedTargetClasses, self._resourceTypes = [], []
            self._defaultTargetClass = None

    def load(self, fileLike, targetClass=Material):
        """
        The mtl loader.
        Load the material with name materialName from 
        """
        # FIXME: only parses face texture at the moment
        inMaterial = False
        textures = []
        for line in fileLike:
            if len(line) < 1 :
                if inMaterial: break
                continue
            if line[0] == '#' : continue
            parts = line.strip().split(' ')
            if parts[0] == 'newmtl':
                inMaterial = True
            if parts[0] == 'map_Kd':
                textures.append(parts[1])
        return Material(textures=textures)

# register loader with resource manager
ResourceManager().registerResourceLoader(OBJModelLoader())
ResourceManager().registerResourceLoader(MaterialLoader())

