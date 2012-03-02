# File name: Rules.py
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

from Fill import Fill, Transparent, Colour
from Border import Border
from Box import Padding, Margin, BaseBox
import Literals

class Rule(object):
    def __init__(self, selectors, properties, **kwargs):
        super(Rule, self).__init__(**kwargs)
        self._selectors = selectors
        self._properties = properties

    def __eq__(self, other):
        if not isinstance(other, Rule):
            return NotImplemented
        return (self._selectors == other._selectors and
                self._properties == other._properties)

    def __ne__(self, other):
        r = self.__eq__(other)
        if r is NotImplemented:
            return r
        else:
            return not r

    def __unicode__(self):
        return """{0} {{
    {1}
}}""".format(
            "\n, ".join(map(unicode, self._selectors)),
            "\n    ".join(("{0}: {1};".format(name, " ".join(map(unicode, value))) for name, value in self._properties))
        )

    def __repr__(self):
        return "<{0} selectors={1}>".format(self.__class__.__name__, len(self._selectors))
