# File name: ModelLoader.py
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

from Engine.CEngine.Log import server, Severity
log = server.getChannel("resources.obj")

from Base import ResourceLoader
from Manager import ResourceManager
from Engine.Model import Model
from Engine.GL.RenderModel import RenderModel

import MaterialLoader

class OBJModelLoader(ResourceLoader):
    """
    The OBJModelLoader provides a ModelLoader that is capable of loading
    wavefront obj formatted 3D model data.
    """

    def __init__(self, **kwargs):
        supported = [Model]
        try: # add support for RenderModel if available
            supported.insert(0, RenderModel)
        except NameError:
            pass
        super(OBJModelLoader, self).__init__(
            supported,
            ['obj'],
            relative_path_prefix="/data/models",
            **kwargs)

    def load(self, filelike, targetclass=Model):
        """
        The actual geometry data loader.
        Note: All faces in the obj data are expected to be triangulated.
        """
        facecount = 0
        vertices, normals, texcoords, faces, materials = [], [], [], [], []
        for line in filelike:
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
                if len(face) == 3:
                    faces.append(face)
                else:
                    log.log(Severity.Warning, 'Face is not a triangle')
            elif parts[0] == 'usemtl':
                if len(parts) == 2:
                    materials.append([parts[1],len(faces)])
            else:
                log.log(Severity.Hint, "FIXME: Unhandled obj data: {0}".format(line.strip()))
                pass
        if len(faces) < 1:
            raise Exception('No faces found in geometric data!')
        args = dict(faces=faces, vertices=vertices, normals=normals,
            texcoords=texcoords, materials=materials)
        if targetclass is Model:
            return Model(**args)
        else:
            return RenderModel(**args)

# register loader with resource manager
ResourceManager().register_resource_loader(OBJModelLoader)

