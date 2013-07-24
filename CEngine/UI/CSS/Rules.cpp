/**********************************************************************
File name: Rules.cpp
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
#include "Rules.hpp"

namespace PyEngine { namespace UI {

/* PyEngine::UI::MarginRule */

void MarginRule::apply_to(Style &to) const
{
    apply_to_box(to.margin());
}

/* PyEngine::UI::PaddingRule */

void PaddingRule::apply_to(Style &to) const
{
    apply_to_box(to.padding());
}

/* PyEngine::UI::BackgroundRule */

void BackgroundRule::apply_to(Style &to) const
{
    if (has_effect()) {
        to.set_background(value());
    }
}

/* PyEngine::UI::TextColourRule */

void TextColourRule::apply_to(Style &to) const
{
    if (has_effect()) {
        to.set_text_colour(value());
    }
}

/* PyEngine::UI::BoxSpacingXRule */

void BoxSpacingXRule::apply_to(Style &to) const
{
    if (has_effect()) {
        to.set_box_spacing_x(value());
    }
}

/* PyEngine::UI::BoxSpacingYRule */

void BoxSpacingYRule::apply_to(Style &to) const
{
    if (has_effect()) {
        to.set_box_spacing_y(value());
    }
}

/* PyEngine::UI::FlexRule */

void FlexRule::apply_to(Style &to) const
{
    if (has_effect()) {
        to.set_flex(value());
    }
}

}
}
