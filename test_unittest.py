# File name: test_unittest.py
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
import unittest

class TestUnittest(unittest.TestCase):
    @unittest.expectedFailure
    def test_expectedFailure(self):
        self.assertTrue(False)

    @unittest.skip("Because we can")
    def test_skip(self):
        pass

    @unittest.expectedFailure
    def test_unexpectedSuccess(self):
        pass

    def test_pass(self):
        pass

    def test_error(self):
        raise Exception()

    def test_failure(self):
        self.assertTrue(False)
        
class TestUnittest2(unittest.TestCase):
    def runTest(self):
        self.assertTrue(False)

# Only for testing the unittest framework output. Remove del to test
# output for all cases.
del TestUnittest
del TestUnittest2
