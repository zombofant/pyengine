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
        self._visibleSubMenu = None
        self._shownBy = None
        self.ShowingSubMenu = False

    def addButton(self, caption, onclick=None, **kwargs):
        button = MenuButton(self, caption=caption, onclick=onclick, **kwargs)
        return button

    def addButtonAndMenu(self, caption, **kwargs):
        button = self.addButton(caption, **kwargs)
        button.SubMenu = Menu()
        return button, button.SubMenu

    def _setupSubMenu(self, button, submenu):
        pass

    def showSubMenu(self, button, menu):
        if self._visibleSubMenu == menu:
            return

        if self._visibleSubMenu is not None:
            self._visibleSubMenu._shownBy = None
            self._visibleSubMenu.kill()
        self._visibleSubMenu = menu

        if menu:
            menu._shownBy = self
            self.RootWidget.PopupLayer.add(menu)
            width, height = menu.getDimensions()
            menu.AbsoluteRect.Width = width
            menu.AbsoluteRect.Height = height
            self._setupSubMenu(button, menu)
            menu.invalidate()

            self.ShowingSubMenu = True

    def hideSubMenu(self):
        self.showSubMenu(None, None)
        self.ShowingSubMenu = False

    def kill(self):
        self.hideSubMenu()
        self.invalidate()
        self.Parent.remove(self)

    def killUpwards(self):
        if self._shownBy:
            self._shownBy.killUpwards()
        else:
            self.kill()

    @property
    def VisibleSubMenu(self):
        return self._visibleSubMenu

class MenuBar(AbstractHBox, AbstractMenu):
    def _setupSubMenu(self, button, submenu):
        subStyle = submenu.ComputedStyle
        buttonRect = button.AbsoluteRect
        x = buttonRect.Left + subStyle.Margin.Left
        y = buttonRect.Top + buttonRect.Height + subStyle.Margin.Top
        submenu.AbsoluteRect.Y = y
        submenu.AbsoluteRect.X = x

        self.RootWidget.PopupLayer.CurrentRootMenu = self

    def hideSubMenu(self):
        super(MenuBar, self).hideSubMenu()
        self.RootWidget.PopupLayer.CurrentRootMenu = None

    def kill(self):
        self.hideSubMenu()

class Menu(AbstractVBox, AbstractMenu):
    def __init__(self, parent=None, **kwargs):
        super(Menu, self).__init__(parent, **kwargs)

    def addSeparator(self, **kwargs):
        separator = MenuSeparator(self, **kwargs)
        return separator

    def getDimensions(self):
        myStyle = self.ComputedStyle

        paddingLeft, paddingRight = myStyle.Padding.Left, myStyle.Padding.Right

        maxLabelWidth = max([btn.getLabelWidth()
                             for btn in self
                             if isinstance(btn, MenuButton)])

        maxWidth = 0
        for item in self:
            item._maxLabelWidthHint = maxLabelWidth
            width, height = item.getDimensions()

            # FIXME: will break with auto-margins
            itemMargin = item.ComputedStyle.Margin
            if width is not None:
                width += itemMargin.Horizontal + paddingLeft + paddingRight
                maxWidth = max(maxWidth, width)

        totalSpace, totalFlex, totalHeight = self.getSpaceFlexWidth(
            self._getSpacingList(myStyle.BoxSpacingY)
            )

        assert totalFlex == 0
        totalHeight += totalSpace
        totalHeight += myStyle.Border.getBox().Vertical
        return maxWidth, totalHeight

    def _setupSubMenu(self, button, submenu):
        subStyle = submenu.ComputedStyle
        buttonRect = button.AbsoluteRect
        x = buttonRect.Left + buttonRect.Width + subStyle.Margin.Left
        y = buttonRect.Top + subStyle.Margin.Top

        submenu.AbsoluteRect.Y = y
        submenu.AbsoluteRect.X = x

class MenuItem(Widget):
    pass

class MenuButton(LabelledWidget, MenuItem):
    def __init__(self, parent,
                 caption="",
                 onclick=None,
                 hotkeyString="",
                 **kwargs):
        self._hotkeyLabel = Label.Label(self)
        super(MenuButton, self).__init__(parent, **kwargs)
        self._label.Text = caption
        self._hotkeyLabel.Text = hotkeyString
        self._onclick = onclick
        self._maxLabelWidthHint = 0
        self.SubMenu = None

    def _invalidateComputedStyle(self):
        super(MenuButton, self)._invalidateComputedStyle()
        self._hotkeyLabel.invalidateLayout()

    def invalidateContext(self):
        super(MenuButton, self).invalidateContext()
        self._hotkeyLabel.invalidateContext()

    def doAlign(self):
        myStyle = self.ComputedStyle

        _label_rect = copy.copy(self.AbsoluteRect)
        _label_rect.shrink(myStyle.Padding)
        _label_rect.shrink(myStyle.Border.getBox())

        hotkeyW, hotkeyH = self._hotkeyLabel.getDimensions()

        if hotkeyW > 0:
            _label_rect.Width = _label_rect.Width - (hotkeyW + myStyle.BoxSpacingX)

        self._label.Width = _label_rect.Width
        self._label.Height = _label_rect.Height
        self._label_rect = _label_rect

        hotkey_rect = copy.copy(self._label_rect)
        hotkey_rect.X = hotkey_rect.X + hotkey_rect.Width + myStyle.BoxSpacingX
        self._hotkey_rect = hotkey_rect

        self._hotkeyLabel.Height = None
        self._hotkeyLabel.Width = None

    def getLabelWidth(self):
        return self._label.getDimensions()[0]

    def getDimensions(self):
        width, height = self._label.getDimensions()
        width = max(width, self._maxLabelWidthHint)
        hotkeyW, hotkeyH = self._hotkeyLabel.getDimensions()

        myStyle = self.ComputedStyle
        borderBox = myStyle.Border.getBox()
        width += myStyle.Padding.Horizontal + borderBox.Horizontal
        height += myStyle.Padding.Vertical + borderBox.Vertical
        if hotkeyW > 0:
            width += hotkeyW + myStyle.BoxSpacingX

        return width, height

    def onMouseEnter(self):
        if self.Parent.ShowingSubMenu or isinstance(self.Parent, Menu):
            self.Parent.showSubMenu(self, self.SubMenu)

    def onMouseClick(self, x, y, button, modifiers, nth):
        if self.SubMenu:
            if self.Parent.VisibleSubMenu != self.SubMenu:
                self.Parent.showSubMenu(self, self.SubMenu)
            elif isinstance(self.Parent, MenuBar):
                self.Parent.kill()
        else:
            self.Parent.killUpwards()

        if self._onclick is not None:
            self._onclick(self)

    def render(self):
        super(MenuButton, self).render()
        self._hotkeyLabel.render(self._hotkey_rect)

class MenuSeparator(MenuItem):
    def __init__(self, parent, **kwargs):
        super(MenuSeparator, self).__init__(parent, **kwargs)

CSS.Minilanguage.ElementNames().registerWidgetClass(MenuBar)
CSS.Minilanguage.ElementNames().registerWidgetClass(Menu)
CSS.Minilanguage.ElementNames().registerWidgetClass(MenuButton)
CSS.Minilanguage.ElementNames().registerWidgetClass(MenuSeparator)
