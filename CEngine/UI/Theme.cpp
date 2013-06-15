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
