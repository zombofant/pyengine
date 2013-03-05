from __future__ import unicode_literals, print_function, division
from our_future import *

__all__ = ["Space"]

import CSS.Minilanguage

from WidgetBase import Widget

class Space(Widget):
    def render(self):
        pass

CSS.Minilanguage.ElementNames().registerWidgetClass(Space)
