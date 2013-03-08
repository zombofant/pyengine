# File name: Literals.py
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

from Fill import Image, Colour, Transparent, Fill, Gradient
from Border import Border
from Box import BaseBox
from Constants import *

try:
    import Engine.CEngine.Pango as Pango
except ImportError:
    class Pango(object):
        class Alignment(object):
            Left = 0
            Right = 1
            Center = 2

        class Weight(object):
            Thin = 0
            UltraLight = 1
            Light = 2
            Book = 3
            Normal = 4
            Medium = 5
            SemiBold = 6
            Bold = 7
            UltraBold = 8
            Heavy = 9
            UltraHeavy = 10

def URLLiteral(self, url):
    return unicode(url)


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
        elif isinstance(first, Gradient):
            gradient = first
        elif first is Transparent:
            if len(args) > 1:
                raise TypeError("Too many arguments for transparent BackgroundLiteral")
            return first
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

    assert (image or colour or gradient) is not None
    if image is not None:
        return image
    elif colour is not None:
        return colour
    else:
        return gradient


def BorderLiteral(width, style, fill):
    style = style.lower()
    if not style == "solid":
        raise ValueError("BorderLiteral only accepts solid")
    if not isinstance(fill, Fill):
        raise TypeError("BorderLiteral needs a Fill as third argument")
    return Border(int(width), fill)


def BoxLiteral(*args):
    return BaseBox(*args)


def IntLiteral(value):
    return int(value)
