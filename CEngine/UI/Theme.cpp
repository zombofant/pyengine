/**********************************************************************
File name: Theme.cpp
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
#include "Theme.hpp"

#include <forward_list>

namespace PyEngine {

/* PyEngine::Theme */

Theme::Theme():
    _rulesets()
{

}

void Theme::add_rule(SelectorPtr selector, RulePtr &&rule)
{
    _rulesets.emplace_back(
        selector->specifity(),
        selector,
        std::move(rule));
    _rulesets.sort();
}

Style* Theme::get_widget_style(const AbstractWidgetPtr &widget) const
{
    Style *style = new Style();
    for (auto& item: _rulesets) {
        Selector *selector = std::get<1>(item).get();

        if (selector->test_widget(widget)) {
            std::get<2>(item)->apply_to(*style);
        }
    }

    return style;
}

}
