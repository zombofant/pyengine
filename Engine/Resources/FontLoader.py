# File name: FontLoader.py
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

from Base import ResourceLoader
from Manager import ResourceManager

class FontLoader(ResourceLoader):
    """
    Implement a loader for fonts resources.

    This uses pyglet as a backend and is not available if pyglet
    ImportErrors.

    This tries to do its best to serve the fontfamily requested, even
    if pyglet does this not. Pyglet returns a surrogate font by default,
    this is circumvented by checking for the availability beforehand
    and ValueError'ing if not available.
    """

    def __init__(self, **kwargs):
        try:
            super(FontLoader, self).__init__(
                [font.base.Font],
                ['ttf', 'otf'],
                **kwargs)
        except NameError:
            self._loader_not_available()

    def get_cache_token(self, vfspath, targetclass, fontfamily=None, size=10, italic=False, bold=False):
        if fontfamily is None:
            raise ValueError("Cannot request a font without specifying fontfamily keyword argument.")
        return (fontfamily, size, italic, bold)

    def load(self, filelike, targetclass, fontfamily=None, size=10, italic=False, bold=False):
        # font.add_file(filelike)
        # if not font.base.Font.have_font(fontfamily):
        #     raise ResourceNotFoundError("Cannot find font family {0}".format(fontfamily))
        # return font.load(fontfamily, size, bold, italic)
        # FIXME/pyglet: font loading
        pass

ResourceManager().register_resource_loader(FontLoader)

