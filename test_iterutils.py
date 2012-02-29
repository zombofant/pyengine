# File name: test_iterutils.py
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

import iterutils

class Iterutils(unittest.TestCase):
    def test_interleaved(self):
        l1 = [10, 20, 30]
        l2 = [15, 25, 35, 45]
        l3 = [17, 27, 37]
        self.assertSequenceEqual(
            list(iterutils.interleave(l1, l2, l3)),
            [10, 15, 17, 20, 25, 27, 30, 35, 37]
        )
        
