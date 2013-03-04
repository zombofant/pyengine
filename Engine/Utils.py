# File name: Utils.py
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

import math

def indent(text, indent):
    """
    Prepends *indent* to each line from *text* and returns the result.
    """
    return "\n".join((indent+line for line in text.split("\n")))

def lineNumbering(text):
    if isinstance(text, (unicode, str)):
        lines = list(text.split("\n"))
    else:
        lines = text
    amount = int(math.ceil(math.log(len(lines), 10)))
    format = "{{1:{0}d}}: {{0}}".format(amount)
    return "\n".join(format.format(line, number+1) for number, line in enumerate(lines))
