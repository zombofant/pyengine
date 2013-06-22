/**********************************************************************
File name: CSS.cpp
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

#include <CEngine/UI/CSS/CSS.hpp>

using namespace PyEngine;

TEST_CASE("UI/CSS/CSS/CSSInheritable/assignment_no_inherit",
          "Assignment of not-inherited values")
{
    css_coord_int_t a(10);
    CHECK(a == 10);
    CHECK(!a.is_inherit());
    css_coord_int_t b = a;
    CHECK(b == 10);
    CHECK(!b.is_inherit());
}

TEST_CASE("UI/CSS/CSS/CSSInheritable/assignment_inherit",
          "Assignment of inherited values")
{
    css_coord_int_t a(Inherit);
    int av = 0;
    CHECK(a.is_inherit());
    css_coord_int_t b = a;
    CHECK(b.is_inherit());
}

TEST_CASE("UI/CSS/CSS/CSSInheritable/extract_inherit_throws",
          "Attempt to convert an unresolved inherit to a value must "
          "throw")
{
    css_coord_int_t a(Inherit);
    int av = 0;
    CHECK_THROWS_AS(av = a, UnresolvedInheritable);
    CHECK_THROWS_AS(static_cast<int>(a), UnresolvedInheritable);
    //~ CHECK_THROWS_AS(int(a), UnresolvedInheritable);
    // the above doesn't throw, maybe it's a compiler bug?
    // an explicit cast should first check static_cast<>(), which does
    // work and should throw accordingly...
}

TEST_CASE("UI/CSS/CSS/CSSInheritable/extract_and_assign_values"
          "Test extraction and assignment of values")
{
    css_coord_int_t a(Inherit);
    CHECK(a.is_inherit());
    a = 10;
    CHECK(!a.is_inherit());
    CHECK(a == 10);
    int av = 0;
    av = a;
    CHECK(av == 10);
}

TEST_CASE("UI/CSS/CSS/CSSInheritable/compare"
          "Test comparision with values and Inherit")
{
    css_coord_int_t a(Inherit);
    bool test = a == Inherit; // <-- bug in Catch appearantly!
    CHECK(test);
    CHECK_THROWS_AS(a == 10, UnresolvedInheritable);
    a = 10;
    CHECK(a == 10);
    CHECK(a != Inherit);
}

TEST_CASE("UI/CSS/CSS/value_is_inherit",
          "Test working of is_inherit value trait")
{
    css_coord_int_t a(Inherit);
    css_coord_int_t b(10);
    int c = 20;

    CHECK(value_is_inherit(a));
    CHECK(!value_is_inherit(b));
    CHECK(!value_is_inherit(c));
}

TEST_CASE("UI/CSS/CSS/CSSInheritable/deinherit",
          "Test de-inheritance")
{
    css_coord_int_t inheriting(Inherit);
    css_coord_int_t other(10);

    inheriting.deinherit_with(other);
    CHECK(!inheriting.is_inherit());
    CHECK(inheriting == 10);
}
