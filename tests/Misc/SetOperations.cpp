/**********************************************************************
File name: SetOperations.cpp
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

TEST_CASE("Misc/SetOperations/SetDifferenceIterator/invariants",
          "Test some invariants on the SetDifferenceIterator")
{
    typedef SetDifferenceIterator<typename set_t::iterator, set_t>
        iterator_t;

    iterator_t iter_a;
    iterator_t iter_b;

    CHECK(iter_a == iter_b);

    set_t non_empty({1, 2, 3});
    set_t empty({});

    iter_a = iterator_t(empty.begin(), empty.end(), non_empty);
    CHECK(iter_a == iterator_t());

    iter_a = iterator_t(non_empty.begin(), non_empty.end(), empty);
    CHECK(iter_a != iterator_t());
    CHECK(iter_a != iter_b);
    ++iter_a;
    CHECK(iter_a != iterator_t());
    ++iter_a;
    CHECK(iter_a != iterator_t());
    ++iter_a;
    CHECK(iter_a == iterator_t());

    iter_a = iterator_t(non_empty.begin(), non_empty.end(), non_empty);
    CHECK(iter_a == iterator_t());
}

TEST_CASE("Misc/SetOperations/SetDifferenceIterator/normal",
          "Test creation of a set difference using the SetDifferenceIterator")
{
    typedef SetDifferenceIterator<typename set_t::const_iterator, set_t>
        iterator_t;

    const set_t set_a({1, 2, 3});
    const set_t set_b({1, 4});

    set_t c;
    set_t ref({2,3});

    auto out = std::inserter(c, c.begin());

    for (iterator_t iter(set_a.begin(), set_a.end(), set_b);
         iter != iterator_t();
         ++iter)
    {
        *out++ = *iter;
    }

    CHECK(c == ref);
}

TEST_CASE("Misc/SetOperations/set_difference/normal",
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

TEST_CASE("Misc/SetOperations/set_difference/empty_result",
          "Test creation of a set difference to an output iterator, "
          "with empty result")
{
    set_t a({});
    set_t b({1,2,3});

    set_t c;
    set_difference(a, b, std::inserter(c, c.begin()));

    set_t ref({});

    CHECK(c == ref);
    CHECK(a == c);
    CHECK(b != c);
}

