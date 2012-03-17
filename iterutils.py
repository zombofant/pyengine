# File name: iterutils.py
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

import itertools

__all__ = ["interleave", "flattenTwoLevels", "yieldCount"]

def interleave(*iterators):
    """
    Takes an arbitary amount of iterators and yields a sequence like
    it0[0], it1[0], ... itn[0], it0[1], it1[1], ...

    Terminates when the first input iterator cannot yield a further
    element.
    (here it differs from the *roundrobin* example in the
    :mod:`itertools` documentation)
    """
    iters = list(map(iter, iterators))
    while True:
        for iterator in iters:
            yield next(iterator)
    
def flattenTwoLevels(iterable):
    """
    Flattens a two times nested iterable.

    [[[0, 1], [2, 3]], [[4, 5], [6, 7]], ...] => [0, 1, 2, 3, 4, 5, 6, 7, ...]
    """
    return itertools.chain.from_iterable(map(itertools.chain.from_iterable, iterable))

def yieldCount(iterable, storeTo):
    """
    Iterates over the given iterable and stores the amount of iterations
    made after all items are consumed in ``storeTo.len``.
    """
    count = 0
    for item in iterable:
        count += 1
        yield item
    storeTo.len = count
