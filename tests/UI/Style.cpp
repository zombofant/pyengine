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

TEST_CASE("UI/Style/construction",
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
