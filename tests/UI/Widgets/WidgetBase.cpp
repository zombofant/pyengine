/**********************************************************************
File name: WidgetBase.cpp
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

#include <CEngine/UI/Widgets/WidgetBase.hpp>

using namespace PyEngine;

TEST_CASE("UI/Widgets/WidgetBase/Widget/init"
          "Widget initialization")
{
    Widget widget;
    CHECK(widget.get_parent().get() == 0);
    CHECK(widget.get_root().get() == 0);
    CHECK(widget.get_theme().get() == 0);
    CHECK(widget.computed_style() == DefaultStyle());
}

TEST_CASE("UI/Widgets/WidgetBase/ParentWidget/children",
          "Test handling of children")
{
    ParentPtr parent(new ParentWidget());
    WidgetPtr c1(new Widget()), c2(new Widget()), c3(new Widget());

    CHECK(parent->size() == 0);

    parent->add(c1);
    CHECK(c1->get_parent() == parent);

    parent->add(c2);
    CHECK(c2->get_parent() == parent);

    parent->add(c3);
    CHECK(c3->get_parent() == parent);

    CHECK(parent->size() == 3);

    CHECK_THROWS_AS(parent->add(c1), WidgetError);

    parent->remove(c2);
    CHECK(c2->get_parent().get() == 0);
    CHECK(parent->size() == 2);
    CHECK_THROWS_AS(parent->remove(c2), WidgetError);

    CHECK(parent->find(c1) != parent->end());
    CHECK(parent->find(c2) == parent->end());
}

TEST_CASE("UI/Widgets/WidgetBase/iter_upwards",
          "Test iterating the tree upwards")
{
    ParentPtr p1(new ParentWidget()), p2(new ParentWidget());
    WidgetPtr l1(new Widget()), l2(new Widget());

    p1->add(p2);
    p1->add(l1);
    p2->add(l2);

    std::vector<WidgetPtr> chain;
    iter_upwards(l2, std::back_inserter(chain));
    REQUIRE(chain.size() >= 3);
    CHECK(chain.size() == 3);
    CHECK(chain[0] == l2);
    CHECK(chain[1] == p2);
    CHECK(chain[2] == p1);

    chain.clear();
    iter_upwards(l1, std::back_inserter(chain));
    REQUIRE(chain.size() >= 2);
    CHECK(chain.size() == 2);
    CHECK(chain[0] == l1);
    CHECK(chain[1] == p1);

    chain.clear();
    iter_upwards(p1, std::back_inserter(chain));
    REQUIRE(chain.size() >= 1);
    CHECK(chain.size() == 1);
    CHECK(chain[0] == p1);
}
