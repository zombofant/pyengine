#include <catch.hpp>

#include <CEngine/UI/Style.hpp>

using namespace PyEngine;

TEST_CASE("UI/Style/DefaultStyle",
          "Test settings of default style")
{
    Style style = DefaultStyle();

    FillPtr text_colour = style.get_text_colour();
    REQUIRE(text_colour.get() != 0);

    CHECK(*text_colour == Colour(0, 0, 0, 1));
    CHECK(style.get_text_align() == PANGO_ALIGN_LEFT);
    CHECK(style.get_font_weight() == PANGO_WEIGHT_NORMAL);
    CHECK(style.get_font_family() == std::string("sans"));
    CHECK(style.get_font_size() == (unsigned int)12*PANGO_SCALE);
    CHECK(style.get_ellipsize() == PANGO_ELLIPSIZE_END);
    CHECK(style.get_vertical_align() == CSS_VERTICAL_ALIGN_TOP);
}

TEST_CASE("UI/Style/Style/construction",
          "Test settings of default-constructed style")
{
    Style style;

    CHECK(*style.get_background().get() == Transparent());
    CHECK(style.padding() == Padding(0));
    CHECK(style.margin() == Margin(0));
    CHECK(style.border() == Border(0, CSSFillPtr(new Transparent())));
    CHECK(style.get_box_spacing_x() == 0);
    CHECK(style.get_box_spacing_y() == 0);
    CHECK(style.get_width() == Auto());
    CHECK(style.get_height() == Auto());
    CHECK(style.get_flex() == 0u);
    CHECK(style.get_text_colour().is_inherit());
    CHECK(style.get_text_align().is_inherit());
    CHECK(style.get_font_weight().is_inherit());
    CHECK(style.get_font_family().is_inherit());
    CHECK(style.get_font_size().is_inherit());
    CHECK(style.get_ellipsize().is_inherit());
    CHECK(style.get_vertical_align().is_inherit());
    CHECK(style.get_shear_x() == 0);
    CHECK(style.get_shear_y() == 0);
}

TEST_CASE("UI/Style/Style/deinherit"
          "Test deinheritance of style")
{
    Style inheriting;
    inheriting.set_background(Inherit);
    inheriting.set_box_spacing_x(Inherit);
    inheriting.set_box_spacing_y(Inherit);
    inheriting.set_flex(Inherit);
    inheriting.set_text_colour(Inherit);
    inheriting.set_text_align(Inherit);
    inheriting.set_font_weight(Inherit);
    inheriting.set_font_family(Inherit);
    inheriting.set_font_size(Inherit);
    inheriting.set_ellipsize(Inherit);
    inheriting.set_vertical_align(Inherit);
    inheriting.set_shear_x(Inherit);
    inheriting.set_shear_y(Inherit);
    inheriting.padding() = Padding(Inherit);
    inheriting.margin() = Margin(Inherit);
    inheriting.border() = Border(Inherit, Inherit);

    inheriting.deinherit_with(DefaultStyle());

    CHECK(inheriting == DefaultStyle());
}
