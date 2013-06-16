/**********************************************************************
File name: Selectors.cpp
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

#include <CEngine/UI/Selectors.hpp>

using namespace PyEngine;

TEST_CASE("UI/Selectors/Specifity/comparision",
          "Comparision of Specifity objects")
{
    Specifity a(1, 2, 3, 4), b(0, 0, 0, 0), c(1, 0, 0, 0),
              d(1, 2, 3, 4);

    CHECK(a != b);
    CHECK(a == d);
    CHECK(b != c);
    CHECK(a != c);
    CHECK(b != d);
}

TEST_CASE("UI/Selectors/Specifity/addition",
          "Addition of Specifity objects")
{
    Specifity a(1, 2, 3, 4), b(0, 0, 0, 0), c(1, 1, 1, 1);

    a += b;
    CHECK(a == Specifity(1, 2, 3, 4));
    b += a;
    CHECK(b == a);

    a += c;
    CHECK(a == Specifity(2, 3, 4, 5));
    CHECK(b == Specifity(1, 2, 3, 4));
}
