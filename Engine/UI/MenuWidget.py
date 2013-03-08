# File name: MenuWidget.py
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

from operator import attrgetter
import copy

import CSS.Minilanguage

from WidgetBase import ParentWidget, Widget
from Label import Label
from LabelWidget import LabelledWidget
from BoxWidget import AbstractHBox, AbstractVBox

import Flags
import Label

class AbstractMenu(object):
    def __init__(self, **kwargs):
        super(AbstractMenu, self).__init__(**kwargs)
        self._visible_submenu = None
        self._shown_by = None
        self.ShowingSubMenu = False

    def add_button(self, caption, onclick=None, **kwargs):
        button = MenuButton(self, caption=caption, onclick=onclick, **kwargs)
        return button

    def add_button_and_menu(self, caption, **kwargs):
        button = self.add_button(caption, **kwargs)
        button.SubMenu = Menu()
        return button, button.SubMenu

    def _setup_submenu(self, button, submenu):
        pass

    def show_submenu(self, button, menu):
        if self._visible_submenu == menu:
            return

        if self._visible_submenu is not None:
            self._visible_submenu._shown_by = None
            self._visible_submenu.kill()
        self._visible_submenu = menu

        if menu:
            menu._shown_by = self
            self.RootWidget.PopupLayer.add(menu)
            width, height = menu.get_dimensions()
            menu.AbsoluteRect.Width = width
            menu.AbsoluteRect.Height = height
            self._setup_submenu(button, menu)
            menu.invalidate()

            self.ShowingSubMenu = True

    def hide_submenu(self):
        self.show_submenu(None, None)
        self.ShowingSubMenu = False

    def kill(self):
        self.hide_submenu()
        self.invalidate()
        self.Parent.remove(self)

    def kill_upwards(self):
        if self._shown_by:
            self._shown_by.kill_upwards()
        else:
            self.kill()

    @property
    def VisibleSubMenu(self):
        return self._visible_submenu

class MenuBar(AbstractHBox, AbstractMenu):
    def _setup_submenu(self, button, submenu):
        substyle = submenu.ComputedStyle
        buttonrect = button.AbsoluteRect
        x = buttonrect.Left + substyle.Margin.Left
        y = buttonrect.Top + buttonrect.Height + substyle.Margin.Top
        submenu.AbsoluteRect.Y = y
        submenu.AbsoluteRect.X = x

        self.RootWidget.PopupLayer.CurrentRootMenu = self

    def hide_submenu(self):
        super(MenuBar, self).hide_submenu()
        self.RootWidget.PopupLayer.CurrentRootMenu = None

    def kill(self):
        self.hide_submenu()

class Menu(AbstractVBox, AbstractMenu):
    def __init__(self, parent=None, **kwargs):
        super(Menu, self).__init__(parent, **kwargs)

    def add_separator(self, **kwargs):
        separator = MenuSeparator(self, **kwargs)
        return separator

    def get_dimensions(self):
        mystyle = self.ComputedStyle

        borderbox = mystyle.Border.get_box()
        horiz_spacing = mystyle.Padding.Horizontal

        max_label_width = max([btn.get_label_width()
                             for btn in self
                             if isinstance(btn, MenuButton)])

        maxwidth = 0
        for item in self:
            item._max_label_width_hint = max_label_width
            width, height = item.get_dimensions()

            # FIXME: will break with auto-margins
            item_margin = item.ComputedStyle.Margin
            if width is not None:
                width += item_margin.Horizontal + horiz_spacing
                maxwidth = max(maxwidth, width)

        total_space, total_flex, total_height = self.get_space_flex_width(
            self._get_spacing_list(mystyle.BoxSpacingY)
            )

        assert total_flex == 0
        total_height += total_space
        total_height += borderbox.Vertical

        maxwidth += borderbox.Horizontal
        return maxwidth, total_height

    def _setup_submenu(self, button, submenu):
        substyle = submenu.ComputedStyle
        buttonrect = button.AbsoluteRect
        x = buttonrect.Left + buttonrect.Width + substyle.Margin.Left
        y = buttonrect.Top + substyle.Margin.Top

        submenu.AbsoluteRect.Y = y
        submenu.AbsoluteRect.X = x

class MenuItem(Widget):
    pass

class MenuButton(LabelledWidget, MenuItem):
    def __init__(self, parent,
                 caption="",
                 onclick=None,
                 hotkey_string="",
                 **kwargs):
        self._hotkey_label = Label.Label(self)
        super(MenuButton, self).__init__(parent, **kwargs)
        self._label.Text = caption
        self._hotkey_label.Text = hotkey_string
        self._onclick = onclick
        self._max_label_width_hint = 0
        self.SubMenu = None

    def _invalidate_computed_style(self):
        super(MenuButton, self)._invalidate_computed_style()
        self._hotkey_label.invalidate_layout()

    def invalidate_context(self):
        super(MenuButton, self).invalidate_context()
        self._hotkey_label.invalidate_context()

    def do_align(self):
        mystyle = self.ComputedStyle

        _label_rect = copy.copy(self.AbsoluteRect)
        _label_rect.shrink(mystyle.Padding)
        _label_rect.shrink(mystyle.Border.get_box())

        hotkeyW, hotkeyH = self._hotkey_label.get_dimensions()

        if hotkeyW > 0:
            _label_rect.Width = _label_rect.Width - (hotkeyW + mystyle.BoxSpacingX)

        self._label.Width = _label_rect.Width
        self._label.Height = _label_rect.Height
        self._label_rect = _label_rect

        hotkey_rect = copy.copy(self._label_rect)
        hotkey_rect.X = hotkey_rect.X + hotkey_rect.Width + mystyle.BoxSpacingX
        self._hotkey_rect = hotkey_rect

        self._hotkey_label.Height = None
        self._hotkey_label.Width = None

    def get_label_width(self):
        return self._label.get_dimensions()[0]

    def get_dimensions(self):
        width, height = self._label.get_dimensions()
        width = max(width, self._max_label_width_hint)
        hotkeyW, hotkeyH = self._hotkey_label.get_dimensions()

        mystyle = self.ComputedStyle
        borderbox = mystyle.Border.get_box()
        width += mystyle.Padding.Horizontal + borderbox.Horizontal
        height += mystyle.Padding.Vertical + borderbox.Vertical
        if hotkeyW > 0:
            width += hotkeyW + mystyle.BoxSpacingX

        return width, height

    def on_mouse_enter(self):
        if self.Parent.ShowingSubMenu or isinstance(self.Parent, Menu):
            self.Parent.show_submenu(self, self.SubMenu)

    def on_mouse_click(self, x, y, button, modifiers, nth):
        if self.SubMenu:
            if self.Parent.VisibleSubMenu != self.SubMenu:
                self.Parent.show_submenu(self, self.SubMenu)
            elif isinstance(self.Parent, MenuBar):
                self.Parent.kill()
        else:
            self.Parent.kill_upwards()

        if self._onclick is not None:
            self._onclick(self)

    def render(self):
        super(MenuButton, self).render()
        self._hotkey_label.render(self._hotkey_rect)

class MenuSeparator(MenuItem):
    def __init__(self, parent, **kwargs):
        super(MenuSeparator, self).__init__(parent, **kwargs)

CSS.Minilanguage.ElementNames().register_widget_class(MenuBar)
CSS.Minilanguage.ElementNames().register_widget_class(Menu)
CSS.Minilanguage.ElementNames().register_widget_class(MenuButton)
CSS.Minilanguage.ElementNames().register_widget_class(MenuSeparator)
