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

// #include "RootWidget.hpp"

namespace PyEngine {

/* PyEngine::AbstractWidget */

AbstractWidget::AbstractWidget():
    _parent(),
    _root(),
    _alignment_invalidated(true),
    _computed_style_invalidated(true),
    _absolute_rect(0, 0, 0, 0),
    _visible(true)
{

}

AbstractWidget::~AbstractWidget()
{

}

void AbstractWidget::_parent_changed()
{
    ParentPtr parent = _parent.lock();
    if (parent) {
        parent->get_root();
    } else {
        set_root(RootPtr());
    }
}

void AbstractWidget::_root_changed()
{
    _state.reset({Hovered, Focused, Active});
    invalidate_computed_style();
}

void AbstractWidget::set_root(RootPtr root)
{
    if (_root.lock() == root) {
        return;
    }
    _root = root;
    _root_changed();
}

void AbstractWidget::set_parent(ParentPtr parent)
{
    if (parent == _parent.lock()) {
        return;
    }

    _parent = parent;
    _parent_changed();
}

void AbstractWidget::do_align()
{

}

const char* AbstractWidget::element_name() const
{
    return nullptr;
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

bool AbstractWidget::ev_key_down(unsigned int symbol, unsigned int modifiers)
{
    return false;
}

bool AbstractWidget::ev_key_up(unsigned int symbol, unsigned int modifiers)
{
    return false;
}

bool AbstractWidget::ev_mouse_click(int x, int y, unsigned int button,
                                    unsigned int modifiers, unsigned int nth)
{
    return false;
}

bool AbstractWidget::ev_mouse_down(int x, int y, unsigned int button,
                                    unsigned int modifiers)
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

bool AbstractWidget::ev_mouse_move(int x, int y, int dx, int dy,
                                   unsigned int button, unsigned int modifiers)
{
    return false;
}

bool AbstractWidget::ev_mouse_up(int x, int y, unsigned int button,
                                 unsigned int modifiers)
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

/* PyEngine::ParentWidget */

ParentWidget::ParentWidget():
    AbstractWidget(),
    std::enable_shared_from_this<ParentWidget>(),
    _children()
{

}

ParentWidget::~ParentWidget()
{

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
    return WidgetPtr();
}

void ParentWidget::_root_changed()
{
    this->AbstractWidget::_root_changed();
    for (auto child: *this) {
        child->_root_changed();
    }
}

void ParentWidget::add(WidgetPtr child)
{
    if (child->get_parent()) {
        WidgetError("Cannot add a child which is already bound to a parent.");
    }
    _children.push_back(child);
    child->set_parent(shared_from_this());
}

ParentWidget::iterator ParentWidget::begin()
{
    return _children.begin();
}

void ParentWidget::bring_to_front(WidgetPtr child)
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

ParentWidget::iterator ParentWidget::find(WidgetPtr child)
{
    for (auto it = begin(); it != end(); it++) {
        if (*it == child) {
            return it;
        }
    }
    return end();
}

ParentWidget::const_iterator ParentWidget::find(WidgetPtr child) const
{
    for (auto it = cbegin(); it != cend(); it++) {
        if (*it == child) {
            return it;
        }
    }
    return cend();
}

void ParentWidget::remove(WidgetPtr child)
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

    ParentPtr me = shared_from_this();
    if (me != child->get_parent()) {
        throw std::logic_error("Parenthood was not announced at child.");
    }

    _children.erase(it);
}

void ParentWidget::send_to_back(WidgetPtr child)
{
    auto child_it = find(child);
    if (child_it == end()) {
        throw WidgetError("Cannot bring non-child to front.");
    }

    _children.erase(child_it);
    _children.insert(begin(), child);
}

WidgetPtr ParentWidget::hittest(const Point& p)
{
    if (!_hittest(p)) {
        return WidgetPtr();
    }
    realign();
    WidgetPtr hit = _hittest_children(p);
    if (!hit) {
        return shared_from_this();
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
            chain.push_back(shared_from_this());
            return true;
        }
    }

    return false;
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

/* PyEngine::Widget */

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
    return (_hittest(p) ? shared_from_this() : WidgetPtr());
}

bool Widget::hittest_with_chain(const Point &p, HitChain &chain)
{
    if (_hittest(p)) {
        chain.push_back(shared_from_this());
        return true;
    } else {
        return false;
    }
}

}
