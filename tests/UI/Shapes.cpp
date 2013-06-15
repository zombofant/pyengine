#include <catch.hpp>

#include <CEngine/UI/Shapes.hpp>

using namespace PyEngine;

TEST_CASE("UI/Shapes/CSSBox",
          "Initialization of CSSBox")
{
    CSSBox box(1, 2, 3, 4);
    CHECK(box.get_left() == 1);
    CHECK(box.get_top() == 2);
    CHECK(box.get_right() == 3);
    CHECK(box.get_bottom() == 4);
}
