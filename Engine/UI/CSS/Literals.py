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

from Values import Image, Colour, RepeatMode
from Properties import Border, BaseBox, BackgroundImage, BackgroundColour, BackgroundGradient

class URLLiteral(object):
    def __init__(self, url):
        self._url = url


def BackgroundLiteral(*args):
    image = None
    colour = None
    args = list(args)
    if len(args) >= 1:
        first = args.pop(0)
        if isinstance(first, URLLiteral):
            image = Image(first._url)
        elif isinstance(first, Colour):
            colour = first
        elif isinstance(first, Image):
            image = first
        else:
            raise TypeError("Invalid first argument to BackgroundLiteral: {0} {1}".format(type(first), first))
        
    if len(args) >= 1:
        second = args.pop(0)
        if isinstance(second, (URLLiteral, Colour, Image)):
            raise TypeError("Image or Colour reference as second argument to a BackgroundLiteral is not supported.")
        elif isinstance(second, RepeatMode):
            if image is not None:
                image._repeatX = second
        else:
            raise TypeError("Invalid second argument to BackgroundLiteral: {0} {1}".format(type(first), first))

    if len(args) >= 1:
        third = args.pop(0)
        if isinstance(third, RepeatMode) and isinstance(second, RepeatMode):
            if image is not None:
                image._repeatY = third
        else:
            raise TypeError("Invalid third argument to BackgroundLiteral: {0} {1}".format(type(first), first))
    
    if len(args) > 0:
        raise TypeError("Too many arguments to BackgroundLiteral.")

    assert (image or colour) is not None
    if image is not None:
        return BackgroundImage(image)
    else:
        return BackgroundColour(colour)


def BorderLiteral(width, style, colour):
    self.width = width
    style = style.lower()
    if not style == "solid":
        raise ValueError("BorderLiteral only accepts solid")
    self.style = style
    if not isinstance(colour, Colour):
        raise TypeError("BorderLiteral needs a colour as third argument")
    self.colour = colour
    return Border(width, style, colour)


def BoxLiteral(*args):
    return BaseBox(*args)
