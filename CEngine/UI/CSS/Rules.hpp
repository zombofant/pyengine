/**********************************************************************
File name: Rules.hpp
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
#ifndef _PYE_UI_RULES_H
#define _PYE_UI_RULES_H

#include "BaseRules.hpp"

namespace PyEngine { namespace UI {

struct MarginRule: public BoxRule {
public:
    MarginRule() = default;

public:
    virtual void apply_to(Style &to) const override;
};

struct PaddingRule: public BoxRule {
public:
    PaddingRule() = default;

public:
    virtual void apply_to(Style &to) const override;
};

struct BackgroundRule: public FillRule {
public:
    BackgroundRule() = default;

public:
    virtual void apply_to(Style &to) const override;
};

struct TextColourRule: public FillRule {
public:
    TextColourRule() = default;

public:
    virtual void apply_to(Style &to) const override;
};

struct BoxSpacingXRule: public NonNegativeValueRule<css_coord_int_t> {
public:
    BoxSpacingXRule() = default;

public:
    virtual void apply_to(Style &to) const override;
};

struct BoxSpacingYRule: public NonNegativeValueRule<css_coord_int_t> {
public:
    BoxSpacingYRule() = default;

public:
    virtual void apply_to(Style &to) const override;
};

struct FlexRule: public NonNegativeValueRule<css_coord_int_t> {
public:
    FlexRule() = default;

public:
    virtual void apply_to(Style &to) const override;
};

}
}

#endif
