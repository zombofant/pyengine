# File name: WindowWidget.py
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

__all__ = ["Window"]

import CSS.Minilanguage

from WidgetBase import ParentWidget
from LabelWidget import LabelWidget
from BoxWidget import VBox, HBox
from DragController import DragMoveWidget

import Flags

class TitleBar(HBox):
    def __init__(self, parent, **kwargs):
        super(TitleBar, self).__init__(parent, **kwargs)
        self._caption = LabelWidget(self)

    def on_mouse_down(self, x, y, button, modifiers):
        if button <= 3:
            x += self.AbsoluteRect.Left
            y += self.AbsoluteRect.Top
            self.RootWidget.start_drag(
                DragMoveWidget,
                button,
                x, y,
                self.Parent)

class Window(VBox):
    def __init__(self, parent, on_modal_close=None, **kwargs):
        super(Window, self).__init__(parent, **kwargs)
        self.AbsoluteRect.Width = 256
        self.AbsoluteRect.Height = 128
        self._titlebar = TitleBar(self)
        self._flags.add(Flags.Focusable)
        self._on_close_hook = None
        self._has_focused_child = False
        self._modal_result = None
        self._on_modal_close = on_modal_close

    def _modal_close(self):
        self.close()
        if self._on_modal_close:
            self._on_modal_close(self)

    def on_activate(self):
        self.Parent.bring_to_front(self)

    def on_deactivate(self):
        pass

    def on_show_modal(self):
        pass

    def close(self):
        if self._on_close_hook:
            self._on_close_hook(self)
        self.Parent.remove(self)

    @property
    def Caption(self):
        return self._titlebar._caption.Text

    @Caption.setter
    def Caption(self, value):
        self._titlebar._caption.Text = value

    @property
    def HasFocusedChild(self):
        return self._has_focused_child

    @HasFocusedChild.setter
    def HasFocusedChild(self, value):
        if value == self._has_focused_child:
            return
        self._has_focused_child = value
        self.IsActive = value
        if value:
            self.on_activate()

    @property
    def ModalResult(self):
        return self._modal_result

    @ModalResult.setter
    def ModalResult(self, value):
        self._modal_result = value
        if value is not None:
            self._modal_close()


CSS.Minilanguage.ElementNames().register_widget_class(TitleBar)
CSS.Minilanguage.ElementNames().register_widget_class(Window)
