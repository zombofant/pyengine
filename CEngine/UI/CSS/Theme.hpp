/**********************************************************************
File name: Theme.hpp
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
#ifndef _PYE_UI_THEME_H
#define _PYE_UI_THEME_H

#include <list>
#include <tuple>

#include "Selectors.hpp"
#include "Style.hpp"
#include "BaseRules.hpp"

#include "CEngine/UI/Widgets/WidgetBase.hpp"

namespace PyEngine { namespace UI {

class Theme
{
public:
    typedef std::tuple<Specifity, SelectorPtr, RulePtr> item_t;

public:
    Theme();

private:
    std::list<item_t> _rulesets;

public:
    void add_rule(SelectorPtr selector, RulePtr &&rule);

    std::unique_ptr<Style> get_widget_style(const AbstractWidget &widget) const;

    inline std::unique_ptr<Style> get_widget_style(const WidgetPtr &widget) const {
        return get_widget_style(*widget);
    };

};

typedef std::shared_ptr<Theme> ThemePtr;

}
}

#endif
