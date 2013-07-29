/**********************************************************************
File name: Rules.hpp
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
#ifndef _PYE_UI_RULES_H
#define _PYE_UI_RULES_H

#include "BaseRules.hpp"

namespace PyEngine { namespace UI {

struct MarginRule: public BoxRule {
public:
    MarginRule() = default;

public:
    void apply_to(Style &to) const override;
};

struct PaddingRule: public BoxRule {
public:
    PaddingRule() = default;

public:
    void apply_to(Style &to) const override;
};

struct BackgroundRule: public FillRule {
public:
    BackgroundRule() = default;

public:
    void apply_to(Style &to) const override;
};

struct TextColourRule: public FillRule {
public:
    TextColourRule() = default;

public:
    void apply_to(Style &to) const override;
};

struct BoxSpacingXRule: public NonNegativeValueRule<css_coord_int_t> {
public:
    BoxSpacingXRule() = default;

public:
    void apply_to(Style &to) const override;
};

struct BoxSpacingYRule: public NonNegativeValueRule<css_coord_int_t> {
public:
    BoxSpacingYRule() = default;

public:
    void apply_to(Style &to) const override;
};

struct FlexRule: public NonNegativeValueRule<css_coord_int_t> {
public:
    FlexRule() = default;

public:
    void apply_to(Style &to) const override;
};

struct FontFamilyRule: public AbstractRule
{
public:
    typedef std::string value_type;

public:
    FontFamilyRule();

private:
    bool _is_set;
    std::string _value;

public:
    void apply_to(Style &to) const override;
    bool has_effect() const override;
    void set(const std::string &value);
    const std::string& value() const;
    void unset();
};

struct FontSizeRule: public NonNegativeValueRule<css_coord_int_t> {
public:
    FontSizeRule() = default;

public:
    void apply_to(Style &to) const override;
};

struct HeightRule: public NonNegativeValueRule<css_coord_int_t> {
public:
    HeightRule() = default;

public:
    void apply_to(Style &to) const override;

};

struct WidthRule: public NonNegativeValueRule<css_coord_int_t> {
public:
    WidthRule() = default;

public:
    void apply_to(Style &to) const override;

};

struct FontWeightRule: public NonNegativeValueRule<PangoWeight> {
public:
    FontWeightRule() = default;

public:
    void apply_to(Style &to) const override;

};

struct TextAlignRule: public PrimitiveRule<PangoAlignment> {
public:
    TextAlignRule() = default;

public:
    void apply_to(Style &to) const override;

};

struct BorderRule: public AbstractRule
{
public:
    BorderRule();

private:
    template <typename value_t>
    struct value_container {
        value_container():
            is_set(false),
            value()
        {

        };

        bool is_set;
        value_t value;

        void set(const value_t &new_value) {
            is_set = true;
            value = new_value;
        };

        void unset() {
            is_set = false;
        };
    };
    typedef value_container<css_coord_int_t> int_value;
    typedef value_container<FillPtr> fill_value;

    int_value _top_width, _left_width, _right_width, _bottom_width;
    fill_value _top_fill, _left_fill, _right_fill, _bottom_fill;
    int_value _tl_radius, _tr_radius, _bl_radius, _br_radius;

private:
    void apply_edge(const int_value &width, const fill_value &fill,
                    BorderEdge &edge) const;

public:
    void set_top_width(const css_coord_int_t &value);
    void set_left_width(const css_coord_int_t &value);
    void set_right_width(const css_coord_int_t &value);
    void set_bottom_width(const css_coord_int_t &value);
    void set_all_widths(const css_coord_int_t &value);

    void set_top_fill(const FillPtr &value);
    void set_left_fill(const FillPtr &value);
    void set_right_fill(const FillPtr &value);
    void set_bottom_fill(const FillPtr &value);
    void set_all_fills(const FillPtr &value);

    void set_top_left_radius(const css_coord_int_t &value);
    void set_top_right_radius(const css_coord_int_t &value);
    void set_bottom_left_radius(const css_coord_int_t &value);
    void set_bottom_right_radius(const css_coord_int_t &value);
    void set_all_radii(const css_coord_int_t &value);

public:
    void apply_to(Style &to) const override;
    bool has_effect() const override;

};

struct VerticalAlignRule: public PrimitiveRule<VerticalAlign>
{
public:
    VerticalAlignRule() = default;

public:
    void apply_to(Style &to) const override;

};

template <typename rule_t>
RulePtr simple_rule(const typename rule_t::value_type &value)
{
    rule_t *rule = new rule_t();
    rule->set(value);
    return RulePtr(rule);
}

}
}

#endif
