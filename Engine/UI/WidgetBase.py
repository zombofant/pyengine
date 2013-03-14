# File name: WidgetBase.py
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

__all__ = ["AbstractWidget", "ParentWidget", "Widget"]

import copy

from CSS.Rect import Rect
from CSS.Rules import Rule
from CSS.FaceBuffer import FaceBuffer
from CSS.ClassSet import ClassSet
from Style import Style, BaseStyle

from OpenGL.GL import GL_TRIANGLES, glEnable, GL_TEXTURE_2D
from Engine.GL.Texture import Texture2D

try:
    import Engine.CEngine.GL as CGL
except ImportError:
    pass

class AbstractWidget(object):
    """
    Abstract base class for widgets. Do not derive from this if you are not
    creating a root widget. For root widgets, derive from *AbstractWidget*
    and *WidgetContainer*.

    See *Widget* and *ParentWidget* for other widgets.
    """

    def __init__(self, disabled=False, **kwargs):
        super(AbstractWidget, self).__init__(**kwargs)
        self.Visible = True
        self._parent = None
        self._relative_rect = Rect(0, 0, onchange=self.on_resize)
        self._absolute_rect = Rect(0, 0, onchange=self.on_resize)
        self._stylerule = None
        self._computed_style_invalidated = True
        self._alignment_invalidated = True
        self._styleclasses = ClassSet()
        self._rootwidget = None
        self._cairo = None
        self._pango = None
        self._is_hovered = False
        self._is_active = False
        self._is_focused = False
        self._is_enabled = not disabled
        self._computed_style = Style()

    def _invalidate_computed_style(self):
        self._computed_style_invalidated = True

    def _invalidate_alignment(self):
        self._alignment_invalidated = True

    def _theme_changed(self):
        self._invalidate_computed_style()
        self.ComputedStyle

    def invalidate_context(self):
        pass

    def realign(self):
        if self._alignment_invalidated or self._computed_style_invalidated:
            # the below is neccessary for widgets which do no alignment
            # nor rendering -- otherwise we'll realign them on every
            # frame!
            self.ComputedStyle
            self.do_align()
            self._alignment_invalidated = False

    def do_align(self):
        pass

    def render(self):
        self.ComputedStyle.in_cairo(self._cairo, self.AbsoluteRect)

    def on_activate(self):
        pass

    def on_deactivate(self):
        pass

    def on_key_down(self, symbol, modifiers):
        return False

    def on_key_up(self, symbol, modifiers):
        return False

    def on_drag_over(self, x, y, other):
        return False

    def on_mouse_down(self, x, y, button, modifiers):
        return False

    def on_mouse_click(self, x, y, button, modifiers, nth):
        return False

    def on_mouse_move(self, x, y, dx, dy, buttons, modifiers):
        return False

    def on_mouse_up(self, x, y, button, modifiers):
        return False

    def on_mouse_enter(self):
        pass

    def on_mouse_leave(self):
        pass

    def on_resize(self):
        self._invalidate_alignment()

    def on_scroll(self, scrollX, scrollY):
        return False

    def on_text_input(self, text):
        return False

    def on_caret_motion(self, direction, step):
        return False

    def on_caret_motion_select(self, direction, step):
        return False

    def invalidate(self):
        root = self.RootWidget
        if not root:
            return
        root.invalidate_rect(self.AbsoluteRect)

    def get_dimensions(self):
        mystyle = self.ComputedStyle
        borderbox = mystyle.Border.get_box()
        if mystyle.Width is not None:
            width = mystyle.Width + mystyle.Padding.Horizontal + \
                borderbox.Horizontal
        else:
            width = None
        if mystyle.Height is not None:
            height = mystyle.Height + mystyle.Padding.Vertical + \
                borderbox.Vertical
        else:
            height = None

        return width, height

    @property
    def StyleRule(self):
        return self._stylerule

    @StyleRule.setter
    def StyleRule(self, value):
        if value is not None and not isinstance(value, Rule):
            raise TypeError("Widget StyleRules must be CSS Rules")
        self._stylerule = value
        self._invalidate_computed_style()

    @property
    def ComputedStyle(self):
        if self._computed_style_invalidated:
            if self.Theme:
                style = copy.copy(self.Theme.get_widget_style(self))
            else:
                style = Style()
            style += self._stylerule
            if self.Parent:
                style.solve_inheritance(self.Parent.ComputedStyle)
            else:
                style.solve_inheritance(BaseStyle())
            diff = self._computed_style.diff(style)
            if diff:
                self._computed_style = style
                if Style.Layout in diff:
                    self._invalidate_alignment()
                    if self.Parent:
                        self.Parent._invalidate_alignment()
                if Style.Visual in diff:
                    # force redrawing of the current rect
                    self.invalidate()
            self._computed_style_invalidated = False
        return self._computed_style

    @property
    def RelativeRect(self):
        return self._relative_rect

    @RelativeRect.setter
    def RelativeRect(self, value):
        if value != self._relative_rect:
            self._relative_rect.assign(value)
            self._invalidate_alignment()

    @property
    def AbsoluteRect(self):
        return self._absolute_rect

    @AbsoluteRect.setter
    def AbsoluteRect(self, value):
        if value != self._absolute_rect:
            # the old rect must be invalidated too
            self.invalidate()
            self._absolute_rect.assign(value)
            self._invalidate_alignment()

    @property
    def StyleClasses(self):
        return self._styleclasses

    @property
    def CSSState(self):
        return (self._is_hovered, self._is_active, self._is_focused, not self._is_enabled)

    @property
    def IsHovered(self):
        return self._is_hovered

    @IsHovered.setter
    def IsHovered(self, value):
        value = bool(value)
        if value == self._is_hovered:
            return
        self._is_hovered = value
        self._invalidate_computed_style()

    @property
    def IsFocused(self):
        return self._is_focused

    @IsFocused.setter
    def IsFocused(self, value):
        value = bool(value)
        if value == self._is_focused:
            return
        self._is_focused = value
        self._invalidate_computed_style()
        if value:
            self.on_activate()
        else:
            self.on_deactivate()

    @property
    def IsActive(self):
        return self._is_active

    @IsHovered.setter
    def IsActive(self, value):
        value = bool(value)
        if value == self._is_active:
            return
        self._is_active = value
        self._invalidate_computed_style()

    @property
    def IsEnabled(self):
        return self._is_enabled

    @IsHovered.setter
    def IsEnabled(self, value):
        value = bool(value)
        if value == self._is_enabled:
            return
        self._is_enabled = value
        self._invalidate_computed_style()

    @property
    def ClientRect(self):
        rect = copy.copy(self.AbsoluteRect)
        rect.shrink(self.ComputedStyle.Border.get_box())
        return rect

    @property
    def RootWidget(self):
        return self._rootwidget

    @property
    def Theme(self):
        if self._rootwidget:
            return self._rootwidget.Theme
        else:
            return None

    def iter_upwards(self):
        widget = self
        while widget:
            yield widget
            widget = widget.Parent

class Widget(AbstractWidget):
    """
    Base class for non-parent widgets. Use this for any widget which will
    not contain other widgets.

    See *ParentWidget* for parents, *AbstractWidget* for root widgets.
    """

    def __init__(self, parent, **kwargs):
        super(Widget, self).__init__(**kwargs)
        self._parent = None
        self._flags = set()
        self._rootwidget = None
        self._cairo = None
        self._pango = None
        if parent is not None:
            parent.add(self)

    def _require_parent(self):
        if self._parent is None:
            raise ValueError("This operation on {0} requires it to have a parent.".format(self))

    def _parent_changed(self):
        if self._parent is not None:
            self._rootwidget = self._parent.RootWidget
        else:
            self._rootwidget = None
        if self._rootwidget is not None:
            self._cairo = self._rootwidget._cairo
            self._pango = self._rootwidget._pango
        else:
            self._cairo = None
            self._pango = None
        self.IsFocused = False
        self.IsHovered = False
        self.IsActive = False
        self.invalidate_context()
        self._theme_changed()

    def hit_test_with_chain(self, p):
        return [self] if p in self.AbsoluteRect else False

    def hit_test(self, p):
        return self if p in self.AbsoluteRect else None

    def client_to_absolute(self, p):
        return (p[0] + self.AbsoluteRect.X, p[1] + self.AbsoluteRect.Y)

    def client_to_parent(self, p):
        return (p[0] + self._left, p[1] + self._top)

    def parent_to_client(self, p):
        return (p[0] - self._left, p[1] - self._top)

    def update(self, deltaT):
        pass

    @property
    def Parent(self):
        return self._parent

    @property
    def Flags(self):
        return self._flags


class WidgetContainer(object):
    """
    Abstraction of a container which can contain widgets. It is list based
    (thus ordered) and implements a basic typecheck.

    Do not derive from this if you are not implementing a root widget. For
    root widgets, see *AbstractWidget*, for normal widgets which may contain
    other widgets see *ParentWidget*.
    """

    def __init__(self, **kwargs):
        super(WidgetContainer, self).__init__(**kwargs)
        self._child_classes = Widget
        self._children = []

    def __contains__(self, child):
        return child in self._children

    def __delitem__(self, key):
        l = self._children[key]
        if isinstance(key, slice):
            for child in l:
                child._parent = None
                child._parent_changed()
        else:
            l._parent = None
            l._parent_changed()
        del self._children[key]

    def __getitem__(self, key):
        return self._children.__getitem__(key)

    def __iter__(self):
        return iter(self._children)

    def __len__(self):
        return len(self._children)

    def __reversed__(self):
        return reversed(self._children)

    def _check_potential_child(self, child):
        if not isinstance(child, self._child_classes):
            raise TypeError("Got {0}, but {1} only supports {2} as children.".format(type(child), self, self._child_classes))
        if child.Parent is not None:
            raise ValueError("A widget cannot be added multiple times (neither to the same nor to different parents).")

    def _hit_test(self, p):
        for child in self:
            if not child.Visible:
                continue
            hit = child.hit_test(p)
            if hit is not None:
                return hit
        return None

    def _hit_test_with_chain(self, p):
        for child in self:
            if not child.Visible:
                continue
            hit = child.hit_test_with_chain(p)
            if hit is not False:
                return hit
        return False

    def _new_child(self, widget):
        pass

    def add(self, child):
        assert not (child in self._children and not child.Parent == self and not isinstance(child, RootWidget))
        self._check_potential_child(child)
        self._children.append(child)
        child._parent = self
        child._parent_changed()
        self._new_child(child)
        self._invalidate_alignment()

    def index(self, child):
        return self._children.index(child)

    def remove(self, child):
        self._children.remove(child)
        child._parent = None
        child._parent_changed()
        self._invalidate_alignment()

    def tree_depth_first(self):
        yield self
        for child in self:
            if isinstance(child, WidgetContainer):
                for node in child.tree_depth_first():
                    yield node
            else:
                yield child

    def update_rendering_context(self):
        for widget in self.tree_depth_first():
            widget._cairo = widget._rootwidget._cairo
            widget._pango = widget._rootwidget._pango
            widget.invalidate_context()


class ParentWidget(Widget, WidgetContainer):
    """
    Base class for widgets which contain other widgets. This derives from
    Widget and WidgetContainer, so all of the benefits apply here.
    """

    def __init__(self, parent, **kwargs):
        super(ParentWidget, self).__init__(parent)

    def _new_child(self, widget):
        pass

    def _parent_changed(self):
        super(ParentWidget, self)._parent_changed()
        for child in self:
            child._parent_changed()

    def _invalidate_computed_style(self):
        super(ParentWidget, self)._invalidate_computed_style()
        for child in self:
            child._invalidate_computed_style()

    def add(self, widget):
        super(ParentWidget, self).add(widget)
        self._new_child(widget)

    def bring_to_front_by_index(self, key):
        child = self._children[key]
        del self._children[key]
        self._children.insert(0, child)
        self.invalidate()

    def bring_to_front(self, widget):
        self.bring_to_front_by_index(self._children.index(widget))

    def hit_test_with_chain(self, p):
        self.realign()
        if not p in self.AbsoluteRect:
            return False
        chain = self._hit_test_with_chain(p)
        if chain is not False:
            chain.append(self)
            return chain
        else:
            return [self]

    def hit_test(self, p):
        self.realign()
        if not p in self.AbsoluteRect:
            return None
        return self._hit_test(p) or self

    def realign(self):
        super(ParentWidget, self).realign()
        for child in self:
            child.realign()

    def render(self):
        super(ParentWidget, self).render()
        for child in reversed(self):
            if child.Visible and child.AbsoluteRect.Area > 0:
                child.render()

    def send_to_back(self, key):
        child = self._children[key]
        del self._children[key]
        self._children.append(child)
        self.invalidate()

    def update(self, time_delta):
        for child in self:
            child.update(timedelta)
