#include <catch.hpp>

#include <CEngine/UI/Rules.hpp>

using namespace PyEngine;

TEST_CASE("UI/Rules/MarginRule"
          "Test appliance of margin rule")
{
    MarginRule rule;
    CHECK(!rule.has_effect());

    rule.set_all(1, 2, 3, 4);

    CHECK(rule.top() == 1);
    CHECK(rule.right() == 2);
    CHECK(rule.bottom() == 3);
    CHECK(rule.left() == 4);

    CHECK(rule.has_effect());

    Style style;
    rule.apply_to(style);

    CHECK(style.margin() == Margin(4, 1, 2, 3));
}

TEST_CASE("UI/Rules/PaddingRule"
          "Test appliance of padding rule")
{
    PaddingRule rule;
    CHECK(!rule.has_effect());

    rule.set_all(1, 2, 3, 4);

    CHECK(rule.top() == 1);
    CHECK(rule.right() == 2);
    CHECK(rule.bottom() == 3);
    CHECK(rule.left() == 4);

    CHECK(rule.has_effect());

    Style style;
    rule.apply_to(style);

    CHECK(style.padding() == Padding(4, 1, 2, 3));
}

TEST_CASE("UI/Rules/BoxSpacingXRule"
          "Test appliance of box spacing x rule")
{
    BoxSpacingXRule rule;
    CHECK(!rule.has_effect());

    rule.set(1);
    CHECK(rule.value() == 1);
    CHECK(rule.has_effect());

    Style style;
    rule.apply_to(style);

    CHECK(style.get_box_spacing_x() == 1);
}

TEST_CASE("UI/Rules/BoxSpacingYRule"
          "Test appliance of box spacing x rule")
{
    BoxSpacingYRule rule;
    CHECK(!rule.has_effect());

    rule.set(1);
    CHECK(rule.value() == 1);
    CHECK(rule.has_effect());

    Style style;
    rule.apply_to(style);

    CHECK(style.get_box_spacing_y() == 1);
}

TEST_CASE("UI/Rules/BackgroundRule"
          "Test appliance of box spacing x rule")
{
    Colour value = Colour(1, 1, 1, 1);

    BackgroundRule rule;
    CHECK(!rule.has_effect());

    rule.set(new Colour(value));
    CHECK(rule.has_effect());

    Style style;
    REQUIRE(*style.get_background().get() != value);
    rule.apply_to(style);

    CHECK(*style.get_background().get() == value);
}

TEST_CASE("UI/Rules/TextColourRule"
          "Test appliance of box spacing x rule")
{
    Colour value = Colour(1, 1, 1, 1);

    TextColourRule rule;
    CHECK(!rule.has_effect());

    rule.set(new Colour(value));
    CHECK(rule.has_effect());

    // must use DefaultStyle here, as the other one might have Inherit
    // in text colour, which would crash.
    Style style = DefaultStyle();
    REQUIRE(*style.get_text_colour().get() != value);
    rule.apply_to(style);

    CHECK(*style.get_text_colour().get() == value);
}

