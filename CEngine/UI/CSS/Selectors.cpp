/**********************************************************************
File name: Selectors.cpp
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
#include "Selectors.hpp"

#include "CEngine/UI/Widgets/WidgetBase.hpp"

namespace PyEngine { namespace UI {

/* PyEngine::UI::Specifity */

Specifity::Specifity():
    std::array<unsigned int, 4>{{0, 0, 0, 0}}
{

}

Specifity::Specifity(unsigned int a, unsigned int b,
                     unsigned int c, unsigned int d):
    std::array<unsigned int, 4>{{a, b, c, d}}
{

}

Specifity::Specifity(const Specifity& ref):
    std::array<unsigned int, 4>(ref)
{

}

Specifity& Specifity::operator=(const Specifity& ref)
{
    this->std::array<unsigned int, 4>::operator=(ref);
    return *this;
}

Specifity& Specifity::operator+=(const Specifity& other)
{
    for (size_type i = 0; i < size(); i++) {
        (*this)[i] += other[i];
    }
    return *this;
}

Specifity Specifity::operator+(const Specifity& other)
{
    Specifity result = Specifity(*this);
    result += other;
    return std::move(result);
}

/* PyEngine::UI::Selector */

Selector::Selector():
    _specifity(0, 0, 0, 0),
    _chained()
{

}

Selector::Selector(const SelectorPtr &chained):
    _specifity(chained->specifity()),
    _chained(chained)
{

}

Selector::~Selector()
{

}

bool Selector::operator==(const Selector& oth_) const
{
    if (bool(_chained) != bool(oth_._chained)) {
        return false;
    }
    if (_chained) {
        return *_chained == *oth_._chained;
    }
    return true;
}

const AbstractWidget *Selector::test_widget(const AbstractWidget *widget) const
{
    if (_chained) {
        widget = _chained->test_widget(widget);
    }
    if (!widget) {
        return widget;
    }
    return _test_widget(widget);
}

/* PyEngine::UI::ParentSelector */

ParentSelector::ParentSelector(const SelectorPtr &parent):
    Selector(),
    _parent(parent)
{
    _specifity += Specifity(0, 0, 0, 1);
}

ParentSelector::ParentSelector(const SelectorPtr &parent, const SelectorPtr &chained):
    Selector(chained),
    _parent(parent)
{
    _specifity += Specifity(0, 0, 0, 1);
}

bool ParentSelector::operator==(const Selector& oth_) const
{
    const ParentSelector* oth = dynamic_cast<const ParentSelector*>(&oth_);
    if (!oth) {
        return false;
    }

    return this->Selector::operator==(oth_) && (_parent == oth->_parent);
}

/* PyEngine::UI::ChildOf */

ChildOf::ChildOf(const SelectorPtr &parent):
    ParentSelector(parent)
{

}

ChildOf::ChildOf(const SelectorPtr &parent, const SelectorPtr &chained):
    ParentSelector(parent, chained)
{

}

const AbstractWidget *ChildOf::_test_widget(const AbstractWidget *widget) const
{
    ParentPtr p = widget->get_parent();
    while (p) {
        if (_parent->test_widget(p)) {
            return p;
        }
        p = p->get_parent();
    }
    return nullptr;
}

/* PyEngine::UI::DirectChildOf */

DirectChildOf::DirectChildOf(const SelectorPtr &parent):
    ParentSelector(parent)
{

}

DirectChildOf::DirectChildOf(const SelectorPtr &parent, const SelectorPtr &chained):
    ParentSelector(parent, chained)
{

}

const AbstractWidget *DirectChildOf::_test_widget(const AbstractWidget *widget) const
{
    ParentPtr p = widget->get_parent();
    if (_parent->test_widget(p)) {
        return p;
    }
    return nullptr;
}

/* PyEngine::UI::Is */

Is::Is(const std::string& element_name):
    Selector(),
    _element_name(element_name)
{

}

Is::Is(const std::string& element_name, const SelectorPtr &chained):
    Selector(chained),
    _element_name(element_name)
{

}

const AbstractWidget *Is::_test_widget(const AbstractWidget *widget) const
{
    if (widget->is_element(_element_name)) {
        return widget;
    } else {
        return nullptr;
    }
}

/* PyEngine::UI::State */

State::State(CSSStateFlag flag):
    Selector(),
    _states({flag})
{

}

State::State(CSSStateFlag flag, const SelectorPtr &chained):
    Selector(chained),
    _states({flag})
{
    State* chained_state = dynamic_cast<State*>(chained.get());
    if (chained_state) {
        _states |= chained_state->_states;
        _chained = chained_state->_chained;
    }
}

const AbstractWidget *State::_test_widget(const AbstractWidget *widget) const
{
    if ((widget->state() & _states) != _states) {
        return nullptr;
    }
    return widget;
}

}
}
