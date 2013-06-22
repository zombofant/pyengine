/**********************************************************************
File name: Shapes.cpp
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

#include <CEngine/UI/CSS/Shapes.hpp>

using namespace PyEngine;

TEST_CASE("UI/CSS/Shapes/CSSBox",
          "Initialization of CSSBox")
{
    CSSBox box(1, 2, 3, 4);
    CHECK(box.get_left() == 1);
    CHECK(box.get_top() == 2);
    CHECK(box.get_right() == 3);
    CHECK(box.get_bottom() == 4);
}

TEST_CASE("UI/CSS/Shapes/Rect/NotARect",
          "Test NotARect semantics")
{
    Rect not_a_rect(NotARect);
    Rect not_a_rect2(NotARect);
    Rect r1(0, 0, 0, 0);

    CHECK(not_a_rect == NotARect);
    CHECK(not_a_rect2 == NotARect);
    CHECK(not_a_rect == not_a_rect2);
    CHECK(not_a_rect != r1);
}

TEST_CASE("UI/CSS/Shapes/Rect/operations",
          "Initialization and some rectangle operations")
{
    Rect r1(0, 0, 10, 10);
    Rect r2(0, 0, 5, 5);
    Rect r3(5, 0, 10, 5);

    CHECK(r1.is_a_rect() == true);
    CHECK(r2.is_a_rect() == true);
    CHECK(r3.is_a_rect() == true);

    CHECK(r1.get_width() == 10);
    CHECK(r1.get_height() == 10);

    CHECK(r2.get_width() == 5);
    CHECK(r2.get_height() == 5);

    CHECK(r3.get_width() == 5);
    CHECK(r3.get_height() == 5);

    CHECK(r1.contains(r2));
    CHECK(r1.contains(r3));

    CHECK(r1 != r2);
    CHECK(r2 != r3);
    CHECK(r3 != r1);

    CHECK((r1 & r2) == r2);
    CHECK((r1 & r3) == r3);
    CHECK((r2 & r3) == Rect(5, 0, 5, 5));

    CHECK((r1 | r2) == r1);
    CHECK((r1 | r3) == r1);
    CHECK((r2 | r3) == Rect(0, 0, 10, 5));

    CHECK((r1 | NotARect) == r1);
    CHECK((r1 & NotARect) == NotARect);
    CHECK((NotARect | r1) == r1);
    CHECK((NotARect & r1) == NotARect);
}
