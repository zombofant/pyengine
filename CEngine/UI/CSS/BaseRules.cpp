/**********************************************************************
File name: BaseRules.cpp
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
#include "BaseRules.hpp"

namespace PyEngine {

/* PyEngine::AbstractRule */

AbstractRule::~AbstractRule()
{

}

/* PyEngine::RuleGroup */

void RuleGroup::apply_to(Style &to) const
{
    for (auto& rulep: _rules) {
        rulep->apply_to(to);
    }
}

bool RuleGroup::has_effect() const
{
    for (auto& rulep: _rules) {
        if (rulep->has_effect()) {
            return true;
        }
    }
    return false;
}

void RuleGroup::add(std::unique_ptr<AbstractRule> &&rule)
{
    _rules.push_back(std::move(rule));
}

/* PyEngine::BoxRule */

BoxRule::BoxRule():
    AbstractRule(),
    _set_top(false),
    _set_right(false),
    _set_bottom(false),
    _set_left(false),
    _top(0),
    _right(0),
    _bottom(0),
    _left(0)
{

}

void BoxRule::apply_to_box(CSSBox &to) const
{
    if (_set_top) {
        to.set_top(_top);
    }
    if (_set_right) {
        to.set_right(_right);
    }
    if (_set_bottom) {
        to.set_bottom(_bottom);
    }
    if (_set_left) {
        to.set_left(_left);
    }
}

void BoxRule::set_all(const css_coord_int_t &v)
{
    _set_bottom = true;
    _bottom = v;

    _set_left = true;
    _left = v;

    _set_right = true;
    _right = v;

    _set_top = true;
    _top = v;
}

void BoxRule::set_all(const css_coord_int_t &top,
                      const css_coord_int_t &right,
                      const css_coord_int_t &bottom,
                      const css_coord_int_t &left)
{
    _set_bottom = true;
    _bottom = bottom;

    _set_left = true;
    _left = left;

    _set_right = true;
    _right = right;

    _set_top = true;
    _top = top;
}

void BoxRule::set_bottom(const css_coord_int_t &ref)
{
    _set_bottom = true;
    _bottom = ref;
}

void BoxRule::set_left(const css_coord_int_t &ref)
{
    _set_left = true;
    _left = ref;
}

void BoxRule::set_right(const css_coord_int_t &ref)
{
    _set_right = true;
    _right = ref;
}

void BoxRule::set_top(const css_coord_int_t &ref)
{
    _set_top = true;
    _top = ref;
}

void BoxRule::unset_bottom()
{
    _set_bottom = false;
}

void BoxRule::unset_left()
{
    _set_left = false;
}

void BoxRule::unset_right()
{
    _set_right = false;
}

void BoxRule::unset_top()
{
    _set_top = false;
}

bool BoxRule::has_effect() const
{
    return (_set_top || _set_right || _set_bottom || _set_left);
}

/* PyEngine::FillRule */

FillRule::FillRule():
    AbstractRule(),
    _fill(nullptr)
{

}

void FillRule::set(const CSSFillPtr& value)
{
    _fill = value;
}

void FillRule::unset()
{
    _fill = nullptr;
}

bool FillRule::has_effect() const
{
    return (_fill.is_inherit() || (_fill != nullptr));
}

}
