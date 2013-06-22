/**********************************************************************
File name: EnumBitset.cpp
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

#include <CEngine/Misc/EnumBitset.hpp>

using namespace PyEngine;

enum Foo {
    FooA = 1,
    FooB = 2,
    FooC = 3
};

TEST_CASE("Misc/EnumBitset"
          "Test the enum bitset")
{
    EnumBitset<Foo, FooA, FooC> bitset;
    CHECK(bitset.none());
    CHECK(!bitset.any());
    CHECK(!bitset.all());
    bitset.set(FooA);

    CHECK(bitset.any());
    CHECK(!bitset.none());
    CHECK(bitset[FooA]);
    CHECK(bitset.test(FooA));

    bitset.flip(FooA);
    CHECK(!bitset.test(FooA));

    bitset.set();
    CHECK(bitset.all());
    CHECK(bitset.test(FooC));

    bitset.flip(FooB);
    CHECK(!bitset.test(FooB));

    bitset.flip(FooA).set(FooB).reset(FooC);
    CHECK(!bitset.test(FooA));
    CHECK(bitset.test(FooB));
    CHECK(!bitset.test(FooC));
}

TEST_CASE("Misc/EnumBitset/initializer_lists"
          "Test the enum bitset with initializer_lists")
{
    EnumBitset<Foo, FooA, FooC> bitset({FooB});
    CHECK(bitset.test(FooB));

    bitset.reset().set({FooA, FooB}).flip({FooB, FooC});
    CHECK(bitset.test(FooA));
    CHECK(!bitset.test(FooB));
    CHECK(bitset.test(FooC));
}
