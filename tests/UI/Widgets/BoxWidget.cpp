/**********************************************************************
File name: BoxWidget.cpp
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

#include <CEngine/UI/Widgets/BoxWidget.hpp>

#include "tests/UI/test_utils.hpp"

using namespace PyEngine::UI;

TEST_CASE("UI/Widgets/BoxWidget/VBox/align",
          "Test alignment in vboxes")
{
    RootPtr root = setup_test_env();
    ThemePtr theme = root->get_theme();

    std::unique_ptr<FlexRule> rule(new FlexRule());
    rule->set(1);

    theme->add_rule(
        SelectorPtr(new ChildOf(SelectorPtr(new Is("vbox")))),
        std::move(rule)
    );

    ParentPtr vbox(new VBox());
    WidgetPtr child1(new Widget());
    WidgetPtr child2(new Widget());

    root->desktop_layer()->add(vbox);
    vbox->add(child1);
    vbox->add(child2);

    vbox->absolute_rect() = Rect(0, 0, 256, 256);
    vbox->do_align();

    CHECK(child1->absolute_rect() == Rect(0, 0, 256, 128));
    CHECK(child2->absolute_rect() == Rect(0, 128, 256, 128));
}

TEST_CASE("UI/Widgets/BoxWidget/HBox/align",
          "Test alignment in hboxes")
{
    RootPtr root = setup_test_env();
    ThemePtr theme = root->get_theme();

    std::unique_ptr<FlexRule> rule(new FlexRule());
    rule->set(1);

    theme->add_rule(
        SelectorPtr(new ChildOf(SelectorPtr(new Is("hbox")))),
        std::move(rule)
    );

    ParentPtr hbox(new HBox());
    WidgetPtr child1(new Widget());
    WidgetPtr child2(new Widget());

    root->desktop_layer()->add(hbox);
    hbox->add(child1);
    hbox->add(child2);

    hbox->absolute_rect() = Rect(0, 0, 256, 256);
    hbox->do_align();

    CHECK(child1->absolute_rect() == Rect(0, 0, 128, 256));
    CHECK(child2->absolute_rect() == Rect(128, 0, 128, 256));
}
