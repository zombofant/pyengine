/**********************************************************************
File name: Matrices.cpp
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

#include <CEngine/Misc/SetOperations.hpp>

using namespace PyEngine;

typedef std::set<int> set_t;

TEST_CASE("Misc/SetOperations/set_difference"
          "Test creation of a set difference to an output iterator")
{
    set_t a({1,2,3});
    set_t b({1,4});

    set_t c;
    set_difference(a, b, std::inserter(c, c.begin()));

    set_t ref({2,3});

    CHECK(c == ref);
    CHECK(a != c);
    CHECK(b != c);
}
