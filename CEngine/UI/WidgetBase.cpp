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
    std::enable_shared_from_this<AbstractWidget>(),
    _parent(),
    _root(),
    _alignment_invalidated(true),
    _computed_style_invalidated(true),
    _absolute_rect(0, 0, 0, 0),
    _visible(true),
    _enabled(true),
    _active(false),
    _hovered(false),
    _focused(false)
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
    _focused = false;
    _hovered = false;
    _active = false;
    _invalidate_computed_style();
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

/* PyEngine::WidgetContainer */

WidgetContainer::WidgetContainer():
    std::enable_shared_from_this<WidgetContainer>(),
    _children()
{

}

WidgetContainer::~WidgetContainer()
{

}

WidgetPtr WidgetContainer::_hittest(const Point& p) const
{
    for (auto it = _children.crbegin();
         it != _children.crend();
         it++)
    {
        WidgetPtr hit = (*it)->hittest(p);
        if (hit) {
            return hit;
        }
    }
    return WidgetPtr();
}

void WidgetContainer::add(WidgetPtr child)
{
    if (child->get_parent()) {
        WidgetError("Cannot add a child which is already bound to a parent.");
    }
    _children.push_back(child);
    child->set_parent(shared_from_this());
}

WidgetContainer::iterator WidgetContainer::begin()
{
    return _children.begin();
}

WidgetContainer::const_iterator WidgetContainer::cbegin() const
{
    return _children.cbegin();
}

WidgetContainer::const_iterator WidgetContainer::cend() const
{
    return _children.cend();
}

WidgetContainer::iterator WidgetContainer::end()
{
    return _children.end();
}

WidgetContainer::iterator WidgetContainer::find(WidgetPtr child)
{
    for (auto it = begin(); it != end(); it++) {
        if (*it == child) {
            return it;
        }
    }
    return end();
}

WidgetContainer::const_iterator WidgetContainer::find(WidgetPtr child) const
{
    for (auto it = cbegin(); it != cend(); it++) {
        if (*it == child) {
            return it;
        }
    }
    return cend();
}

void WidgetContainer::remove(WidgetPtr child)
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

/* PyEngine::ParentWidget */

ParentWidget::ParentWidget():
    AbstractWidget(),
    WidgetContainer()
{

}

ParentWidget::~ParentWidget()
{

}

void ParentWidget::_root_changed()
{
    this->AbstractWidget::_root_changed();
    for (auto child: *this) {
        child->_root_changed();
    }
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
    realign();
    WidgetPtr hit = _hittest(p);
    if (!hit) {
        return this->AbstractWidget::hittest(p);
    } else {
        return std::move(hit);
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

/* PyEngine::Widget */

Widget::Widget():
    AbstractWidget()
{

}

Widget::~Widget()
{

}

WidgetPtr Widget::hittest(const Point& p)
{
    return (_absolute_rect.contains(p) ? shared_from_this() : WidgetPtr());
}

}
