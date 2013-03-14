# File name: LayerWidget.py
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

__all__ = ["DesktopLayer", "WindowLayer", "PopupLayer"]

import CSS.Minilanguage

from WidgetBase import ParentWidget
from ScreenWidget import ScreenWidget

class LayerWidget(ParentWidget):
    def hit_test(self, p):
        self.realign()
        if not p in self.AbsoluteRect:
            return None
        return self._hit_test(p)

    def hit_test_with_chain(self, p):
        self.realign()
        if not p in self.AbsoluteRect:
            return False
        return self._hit_test_with_chain(p)

class DesktopLayer(LayerWidget):
    def do_align(self):
        for child in self:
            child.AbsoluteRect = self.AbsoluteRect

class WindowLayer(LayerWidget):
    def __init__(self, parent, **kwargs):
        super(WindowLayer, self).__init__(parent, **kwargs)

    def do_align(self):
        rect = self.AbsoluteRect
        for item in self:
            if isinstance(item, ModalWindowLayer):
                item.AbsoluteRect = rect

class PopupLayer(LayerWidget):
    def __init__(self, parent, **kwargs):
        super(PopupLayer, self).__init__(parent, **kwargs)
        self._current_root_menu = None

    def hit_test(self, p):
        if self._current_root_menu:
            hit = ParentWidget.hit_test(self, p)
            if hit is self and p in self._current_root_menu.AbsoluteRect:
                return self._current_root_menu.hit_test(p)
            return hit
        else:
            return super(PopupLayer, self).hit_test(p)

    def on_mouse_click(self, x, y, button, modifiers, nth):
        if self._current_root_menu:
            self._current_root_menu.hide_submenu()
            self._current_root_menu = None

    @property
    def CurrentRootMenu(self):
        return self._current_root_menu

    @CurrentRootMenu.setter
    def CurrentRootMenu(self, value):
        self._current_root_menu = value

class ModalWindowLayer(ParentWidget):
    # opaque layer widget which eats all other events
    def __init__(self, parent, window, **kwargs):
        super(ModalWindowLayer, self).__init__(parent, **kwargs)
        self._window = window
        window._on_close_hook = self._handle_window_close

        self.add(window)
        if self.Parent:
            self.RootWidget.focus(self._window)

    def _parent_changed(self):
        super(ModalWindowLayer, self)._parent_changed()
        if self.Parent:
            self.AbsoluteRect = self.Parent.AbsoluteRect
            self.invalidate()

    def _handle_window_close(self, sender):
        self.invalidate()
        self.Parent.remove(self)

CSS.Minilanguage.ElementNames().register_widget_class(DesktopLayer)
CSS.Minilanguage.ElementNames().register_widget_class(WindowLayer)
CSS.Minilanguage.ElementNames().register_widget_class(PopupLayer)
CSS.Minilanguage.ElementNames().register_widget_class(ModalWindowLayer, "ModalLayer")
