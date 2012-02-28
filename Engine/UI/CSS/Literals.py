# File name: Literals.py
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

from Fill import Image, Colour
from Border import Border
from Box import BaseBox

def URLLiteral(self, url):
    # URLs in stylesheets are always image references. So resource
    # loading should happen here.
    return Image(url)


def BackgroundLiteral(*args):
    image = None
    colour = None
    args = list(args)
    if len(args) >= 1:
        first = args.pop(0)
        if isinstance(first, Colour):
            colour = first
        elif isinstance(first, Image):
            image = first
        else:
            raise TypeError("Invalid first argument to BackgroundLiteral: {0} {1}".format(type(first), first))
        
    if len(args) >= 1:
        second = args.pop(0)
        if isinstance(second, Image):
            raise TypeError("Image or Colour reference as second argument to a BackgroundLiteral is not supported.")
        elif isinstance(second, RepeatMode):
            if image is not None:
                image.RepeatX = second
        else:
            raise TypeError("Invalid second argument to BackgroundLiteral: {0} {1}".format(type(first), first))

    if len(args) >= 1:
        third = args.pop(0)
        if isinstance(third, RepeatMode) and isinstance(second, RepeatMode):
            if image is not None:
                image.RepeatY = third
        else:
            raise TypeError("Invalid third argument to BackgroundLiteral: {0} {1}".format(type(first), first))
    
    if len(args) > 0:
        raise TypeError("Too many arguments to BackgroundLiteral.")

    assert (image or colour) is not None
    if image is not None:
        return image
    else:
        return colour


def BorderLiteral(width, style, fill):
    style = style.lower()
    if not style == "solid":
        raise ValueError("BorderLiteral only accepts solid")
    if not isinstance(fill, Fill):
        raise TypeError("BorderLiteral needs a Fill as third argument")
    return Border(int(width), fill)


def BoxLiteral(*args):
    return BaseBox(*args)
