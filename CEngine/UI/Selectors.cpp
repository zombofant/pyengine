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

#include "WidgetBase.hpp"

namespace PyEngine {

/* PyEngine::Specifity */

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
    for (int i = 0; i < 3; i++) {
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

/* PyEngine::Selector */

Selector::Selector():
    _specifity(0, 0, 0, 0),
    _chained()
{

}

Selector::Selector(SelectorPtr chained):
    _specifity(chained->specifity()),
    _chained(chained)
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

AbstractWidgetPtr Selector::test_widget(AbstractWidgetPtr widget) const
{
    if (_chained) {
        widget = _chained->test_widget(widget);
    }
    if (!widget) {
        return widget;
    }
    return _test_widget(widget);
}

/* PyEngine::ParentSelector */

ParentSelector::ParentSelector(SelectorPtr parent):
    Selector(),
    _parent(parent)
{
    _specifity += Specifity(0, 0, 0, 1);
}

ParentSelector::ParentSelector(SelectorPtr parent, SelectorPtr chained):
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

/* PyEngine::ChildOf */

ChildOf::ChildOf(SelectorPtr parent):
    ParentSelector(parent)
{

}

ChildOf::ChildOf(SelectorPtr parent, SelectorPtr chained):
    ParentSelector(parent, chained)
{

}

AbstractWidgetPtr ChildOf::_test_widget(AbstractWidgetPtr widget) const
{
    ParentPtr p = widget->get_parent();
    while (p) {
        if (_parent->test_widget(p)) {
            return p;
        }
        p = p->get_parent();
    }
    return AbstractWidgetPtr();
}

/* PyEngine::DirectChildOf */

DirectChildOf::DirectChildOf(SelectorPtr parent):
    ParentSelector(parent)
{

}

DirectChildOf::DirectChildOf(SelectorPtr parent, SelectorPtr chained):
    ParentSelector(parent, chained)
{

}

AbstractWidgetPtr DirectChildOf::_test_widget(AbstractWidgetPtr widget) const
{
    ParentPtr p = widget->get_parent();
    if (_parent->test_widget(p)) {
        return p;
    }
    return AbstractWidgetPtr();
}

/* PyEngine::Is */

Is::Is(const std::string& element_name):
    Selector(),
    _element_name(element_name)
{

}

AbstractWidgetPtr Is::_test_widget(AbstractWidgetPtr widget) const
{
    const char* name = widget->element_name();
    if (_element_name == name) {
        return widget;
    } else {
        return AbstractWidgetPtr();
    }
}

/* PyEngine::State */

State::State(CSSStateFlag flag):
    Selector(),
    _states({flag})
{

}

State::State(CSSStateFlag flag, SelectorPtr chained):
    Selector(chained),
    _states({flag})
{
    State* chained_state = dynamic_cast<State*>(chained.get());
    if (chained_state) {
        _states |= chained_state->_states;
        _chained = chained_state->_chained;
    }
}

AbstractWidgetPtr State::_test_widget(AbstractWidgetPtr widget) const
{
    if ((widget->state() & _states) != _states) {
        return AbstractWidgetPtr();
    }
    return widget;
}

}
