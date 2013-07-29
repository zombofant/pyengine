/**********************************************************************
File name: WidgetBase.cpp
This file is part of: Pythonic Engine

LICENSE

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations under
the License.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public license (the  "GPL License"), in which case  the
provisions of GPL License are applicable instead of those above.

FEEDBACK & QUESTIONS

For feedback and questions about pyengine please e-mail one of the
authors named in the AUTHORS file.
**********************************************************************/
#include "WidgetBase.hpp"

#include "CEngine/UI/CSS/Theme.hpp"

#include "RootWidget.hpp"

namespace PyEngine { namespace UI {

/* PyEngine::UI::WidgetError */

WidgetError::WidgetError(const std::string &what_arg):
    std::invalid_argument(what_arg)
{

}

WidgetError::WidgetError(const char *what_arg):
    std::invalid_argument(what_arg)
{

}

/* PyEngine::UI::AbstractWidget */

AbstractWidget::AbstractWidget():
    _parent(),
    _root(),
    _alignment_invalidated(true),
    _computed_style_invalidated(true),
    _absolute_rect(0, 0, 0, 0),
    _visible(true),
    _state(),
    _flags(),
    _computed_style(),
    _css_classes()
{

}

AbstractWidget::~AbstractWidget()
{

}

void AbstractWidget::_parent_changed()
{
    if (_parent) {
        set_root(_parent->get_root());
    } else {
        set_root(nullptr);
    }
    invalidate_alignment();
}

void AbstractWidget::_root_changed()
{
    _state.reset({Hovered, Focused, Active});
    invalidate_computed_style();
    invalidate_context();
}

void AbstractWidget::set_root(RootPtr root)
{
    if (_root == root) {
        return;
    }
    _root = root;
    _root_changed();
}

void AbstractWidget::invalidate_alignment()
{
    _alignment_invalidated = true;
}

void AbstractWidget::invalidate_context()
{

}

void AbstractWidget::invalidate_computed_style()
{
    _computed_style_invalidated = true;
    invalidate_rect();
}

void AbstractWidget::invalidate_rect()
{
    if (_root) {
        _root->invalidate_rect(absolute_rect());
    }
}

RootPtr AbstractWidget::get_root()
{
    return _root;
}

ThemePtr AbstractWidget::get_theme()
{
    if (_root) {
        return _root->get_theme();
    }
    return nullptr;
}

void AbstractWidget::set_parent(ParentPtr parent)
{
    if (parent == _parent) {
        return;
    }

    _parent = parent;
    _parent_changed();
}

Style& AbstractWidget::computed_style()
{
    if (_computed_style_invalidated) {
        ThemePtr theme = get_theme();
        Style new_style;
        if (theme) {
            new_style = *theme->get_widget_style(*this).get();
        }
        /* TODO: custom style rules */
        ParentPtr parent = get_parent();
        if (parent) {
            new_style.deinherit_with(parent->computed_style());
        } else {
            new_style.deinherit_with(DefaultStyle());
        }

        StyleDiff diff = _computed_style.calc_diff(new_style);
        if (diff.any()) {
            _computed_style = new_style;
            if (diff.test(SD_LAYOUT)) {
                invalidate_alignment();
                if (parent) {
                    parent->invalidate_alignment();
                }
            }
            if (diff.test(SD_VISUAL)) {
                invalidate();
            }
        }

        _computed_style_invalidated = false;
    }

    return _computed_style;
}

void AbstractWidget::do_align()
{

}

cairo_t* AbstractWidget::get_cairo_context()
{
    if (_parent) {
        return _parent->get_cairo_context();
    }
    return nullptr;
}

PangoContext* AbstractWidget::get_pango_context()
{
    if (_parent) {
        return _parent->get_pango_context();
    }
    return nullptr;
}

bool AbstractWidget::is_element(const std::string &name) const
{
    return (name == "*");
}

coord_dimensions_t AbstractWidget::get_dimensions()
{
    Style &mystyle = computed_style();
    CSSBox border_box = mystyle.border().get_box();
    coord_int_t width, height;
    if (!Auto::is_auto(mystyle.get_width())) {
        width = mystyle.get_width()
              + mystyle.padding().get_horizontal()
              + border_box.get_horizontal();
    } else {
        width = Auto();
    }

    if (!Auto::is_auto(mystyle.get_height())) {
        height = mystyle.get_height()
               + mystyle.padding().get_vertical()
               + border_box.get_vertical();
    } else {
        height = Auto();
    }

    return std::make_pair(width, height);
}

void AbstractWidget::invalidate()
{
    RootPtr root = get_root();
    if (root) {
        root->invalidate_rect(absolute_rect());
    }
}

void AbstractWidget::realign()
{
    if (_alignment_invalidated) {
        do_align();
    }
    _alignment_invalidated = false;
}

void AbstractWidget::render()
{
    computed_style().in_cairo(get_cairo_context(), absolute_rect());
}

bool AbstractWidget::ev_activate()
{
    return false;
}

bool AbstractWidget::ev_caret_motion(
    CaretMotionDirection direction,
    CaretMotionStep step,
    bool select)
{
    return false;
}

bool AbstractWidget::ev_deactivate()
{
    return false;
}

bool AbstractWidget::ev_key_down(Key::Key key, KeyModifiers modifiers)
{
    return false;
}

bool AbstractWidget::ev_key_up(Key::Key key, KeyModifiers modifiers)
{
    return false;
}

bool AbstractWidget::ev_mouse_click(int x, int y, MouseButton button,
                                    KeyModifiers modifiers, unsigned int nth)
{
    return false;
}

bool AbstractWidget::ev_mouse_down(int x, int y, MouseButton button,
                                    KeyModifiers modifiers)
{
    return false;
}

bool AbstractWidget::ev_mouse_enter()
{
    return false;
}

bool AbstractWidget::ev_mouse_leave()
{
    return false;
}

bool AbstractWidget::ev_mouse_move(
    int x, int y,
    int dx, int dy,
    MouseButtons buttons,
    KeyModifiers modifiers)
{
    return false;
}

bool AbstractWidget::ev_mouse_up(
    int x, int y,
    MouseButton button,
    KeyModifiers modifiers)
{
    return false;
}

bool AbstractWidget::ev_resize()
{
    return false;
}

bool AbstractWidget::ev_scroll(int scrollx, int scrolly)
{
    return false;
}

bool AbstractWidget::ev_text_input(const char* buf)
{
    return false;
}

/* PyEngine::UI::ParentWidget */

ParentWidget::ParentWidget():
    AbstractWidget(),
    _children()
{

}

ParentWidget::~ParentWidget()
{
    for (auto& child: _children) {
        delete child;
    }
}

bool ParentWidget::_hittest(const Point &p) const
{
    return _absolute_rect.contains(p);
}

WidgetPtr ParentWidget::_hittest_children(const Point &p) const
{
    for (auto it = _children.crbegin();
         it != _children.crend();
         it++)
    {
        const WidgetPtr child = *it;
        if (!child->_visible) {
            continue;
        }
        WidgetPtr hit = child->hittest(p);
        if (hit) {
            return hit;
        }
    }
    return nullptr;
}

void ParentWidget::_root_changed()
{
    this->AbstractWidget::_root_changed();
    RootPtr root = get_root();
    for (auto child: *this) {
        child->set_root(root);
        child->_root_changed();
    }
}

void ParentWidget::add(const WidgetPtr &child)
{
    if (child->get_parent()) {
        throw WidgetError("Cannot add a child which is already bound to a parent.");
    }
    _children.push_back(child);
    child->set_parent(this);
}

ParentWidget::iterator ParentWidget::begin()
{
    return _children.begin();
}

void ParentWidget::bring_to_front(const WidgetPtr &child)
{
    auto child_it = find(child);
    if (child_it == end()) {
        throw WidgetError("Cannot bring non-child to front.");
    }

    _children.erase(child_it);
    /* don't forget that the back-front sematics are exchanged here,
       as we usually want to bring stuff to front and new widgets
       should also be rendered first. */
    _children.push_back(child);
}

ParentWidget::const_iterator ParentWidget::cbegin() const
{
    return _children.cbegin();
}

ParentWidget::const_iterator ParentWidget::cend() const
{
    return _children.cend();
}

ParentWidget::iterator ParentWidget::end()
{
    return _children.end();
}

ParentWidget::iterator ParentWidget::find(const WidgetPtr &child)
{
    for (auto it = begin(); it != end(); it++) {
        if (*it == child) {
            return it;
        }
    }
    return end();
}

ParentWidget::const_iterator ParentWidget::find(const WidgetPtr &child) const
{
    for (auto it = cbegin(); it != cend(); it++) {
        if (*it == child) {
            return it;
        }
    }
    return cend();
}

void ParentWidget::remove(const WidgetPtr &child)
{
    auto it = begin();
    for (;
         it != end();
         it++)
    {
        if (*it == child) {
            break;
        }
    }
    if (it == end()) {
        throw WidgetError("Attempt to remove a widget which is not a child.");
    }

    if (this != child->get_parent()) {
        throw std::logic_error("Parenthood was not announced at child.");
    }

    _children.erase(it);
    child->set_parent(nullptr);
}

void ParentWidget::send_to_back(const WidgetPtr &child)
{
    auto child_it = find(child);
    if (child_it == end()) {
        throw WidgetError("Cannot bring non-child to front.");
    }

    _children.erase(child_it);
    _children.insert(begin(), child);
}

size_t ParentWidget::size() const
{
    return _children.size();
}

WidgetPtr ParentWidget::hittest(const Point& p)
{
    if (!_hittest(p)) {
        return nullptr;
    }
    realign();
    WidgetPtr hit = _hittest_children(p);
    if (!hit) {
        return this;
    } else {
        return std::move(hit);
    }
}

bool ParentWidget::hittest_with_chain(const Point &p, HitChain &chain)
{
    if (!_hittest(p)) {
        return false;
    }

    for (auto &child: *this)
    {
        if (!child->_visible) {
            continue;
        }
        if (child->hittest_with_chain(p, chain)) {
            break;
        }
    }

    chain.push_back(this);
    return true;
}

void ParentWidget::invalidate_context()
{
    AbstractWidget::invalidate_context();
    for (auto child: *this) {
        child->invalidate_context();
    }
}

void ParentWidget::realign()
{
    this->AbstractWidget::realign();
    for (auto child: *this) {
        child->realign();
    }
}

void ParentWidget::render()
{
    this->AbstractWidget::render();
    for (auto it = _children.crbegin();
         it != _children.crend();
         it++)
    {
        (*it)->render();
    }
}

/* PyEngine::UI::Widget */

Widget::Widget():
    AbstractWidget()
{

}

Widget::~Widget()
{

}

bool Widget::_hittest(const Point &p) const
{
    return _absolute_rect.contains(p);
}

WidgetPtr Widget::hittest(const Point &p)
{
    return (_hittest(p) ? this : nullptr);
}

bool Widget::hittest_with_chain(const Point &p, HitChain &chain)
{
    if (_hittest(p)) {
        chain.push_back(this);
        return true;
    } else {
        return false;
    }
}

}
}
