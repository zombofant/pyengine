# File name: test_ModelLoader.py
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
from Engine.Model import Model
from test_ResourceVFS import *
from ModelLoader import OBJModelLoader

class OBJModelLoaderTest(unittest.TestCase):
    _test_model = unicode("""
        # a comment
        o Cube
        v 1.000000 -1.000000 -1.000000
        v 1.000000 -1.000000 1.000000
        v -1.000000 -1.000000 1.000000
        v -1.000000 -1.000000 -1.000000
        v 1.000000 1.000000 -0.999999
        v 0.999999 1.000000 1.000001
        v -1.000000 1.000000 1.000000
        v -1.000000 1.000000 -1.000000
        vn 0.000000 -1.000000 0.000000
        vn 0.000000 1.000000 0.000000
        vn 1.000000 0.000000 0.000000
        vn -0.000000 -0.000000 1.000000
        vn -1.000000 -0.000000 -0.000000
        vn 0.000000 0.000000 -1.000000
        s off
        f 1//1 2//1 3//1
        f 1//1 3//1 4//1
        f 5//2 8//2 7//2
        f 5//2 7//2 6//2
        f 1//3 5//3 6//3
        f 1//3 6//3 2//3
        f 2//4 6//4 7//4
        f 2//4 7//4 3//4
        f 3//5 7//5 8//5
        f 3//5 8//5 4//5
        f 5//6 1//6 4//6
        f 5//6 4//6 8//6
        """).split('\n')

    def test_load_model(self):
        loader = OBJModelLoader()
        loaded_instance = loader.load(self._test_model, targetclass=Model)
        self.assertIsInstance(loaded_instance, Model)
        del loader, loaded_instance

    def test_load_render_model(self):
        try:
            import cuni
            from Engine.GL.RenderModel import RenderModel
        except ImportError:
            self.skipTest('RenderModel class not available')
        RenderModel = Engine.GL.RenderModel
        loader = OBJModelLoader()
        # FIXME create virtual material file and test it
        loaded_instance = loader.load(self._test_model, targetclass=RenderModel)
        self.assertIsInstance(loaded_instance, RenderModel)
        del loader, loaded_instance

