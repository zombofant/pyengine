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
#include <catch.hpp>

#include <CEngine/UI/CSS/Theme.hpp>

#include "tests/UI/test_utils.hpp"

#include <CEngine/UI/Widgets/BoxWidget.hpp>
#include <CEngine/UI/CSS/Rules.hpp>

using namespace PyEngine::UI;

TEST_CASE("UI/CSS/Theme/Theme/theming",
          "Test theming")
{
    RootPtr root = setup_test_env();
    ThemePtr theme = root->get_theme();

    std::unique_ptr<FlexRule> rule1(new FlexRule());
    rule1->set(1);


    std::unique_ptr<TextColourRule> rule2(new TextColourRule());
    rule2->set(FillPtr(new Colour(1, 0, 0, 1)));

    theme->add_rule(
        SelectorPtr(new ChildOf(SelectorPtr(new Is("vbox")))),
        std::move(rule1)
    );

    theme->add_rule(
        SelectorPtr(new ChildOf(SelectorPtr(new Is("root")))),
        std::move(rule2)
    );

    ParentPtr vbox(new VBox());
    WidgetPtr child1(new Widget());
    WidgetPtr child2(new Widget());

    root->desktop_layer()->add(vbox);
    vbox->add(child1);
    vbox->add(child2);

    Style &vbox_style = vbox->computed_style();
    Style ref = DefaultStyle();
    ref.set_text_colour(FillPtr(new Colour(1, 0, 0, 1)));
    CHECK(vbox_style == ref);

    ref.set_flex(1);
    CHECK(child1->computed_style() == ref);
}
