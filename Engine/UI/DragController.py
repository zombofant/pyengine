# File name: DragController.py
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

import Engine.CEngine.key as key

class DragControllerBase(object):
    def __init__(self,
                 root_widget,
                 mouse_button,
                 **kwargs):
        super(DragControllerBase, self).__init__(**kwargs)
        self.root_widget = root_widget
        self.mouse_button = mouse_button

    def release(self):
        self.root_widget.stop_drag()

    def abort(self):
        self.release()

    def on_key_down(self, symbol, modifiers):
        if symbol == key.Escape:
            self.abort()

    def on_key_up(self, symbol, modifiers):
        pass

    def on_mouse_down(self):
        pass

    def on_mouse_move(self, x, y, buttons, modifiers):
        self.on_drag_move(x, y, modifiers)
        pass

    def on_mouse_up(self, x, y, button, modifiers):
        if button == self.mouse_button:
            self.release()

    def on_drag_move(self, x, y, modifiers):
        pass

class DragMoveWidget(DragControllerBase):
    def __init__(self,
                 root_widget,
                 mouse_button,
                 x, y,
                 widget,
                 **kwargs):
        super(DragMoveWidget, self).__init__(root_widget, mouse_button, **kwargs)
        self.start = (x, y)
        self.widget = widget
        widget_rect = widget.AbsoluteRect
        self.start_offs = x - widget_rect.X, y - widget_rect.Y

    def on_widget_move(self, x, y):
        widget_rect = self.widget.AbsoluteRect
        self.widget.invalidate()
        widget_rect.X = px
        widget_rect.Y = py
        self.widget.invalidate()

    def on_drag_move(self, x, y, modifiers):
        sx, sy = self.start_offs
        px, py = x - sx, y - sy
        self.on_widget_move(px, py)
