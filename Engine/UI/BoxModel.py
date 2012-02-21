# File name: BoxModel.py
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

class BaseBorder(object):
    def __init__(self, *args):
        if len(args) == 1:
            self._left, self._right, self._top, self._bottom = (args[0], )*4
        elif len(args) == 4:
            self._left, self._right, self._top, self._bottom = args
        else:
            raise ValueError("BaseBorder expects 1 or 4 arguments.")
            
    @property
    def Left(self):
        return self._left
    
    @Left.setter
    def Left(self, value):
        if self._left == value:
            return
        self._left = value
