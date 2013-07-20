/**********************************************************************
File name: BoxWidget.cpp
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
#include "BoxWidget.hpp"

namespace PyEngine {

/* PyEngine::AbstractBoxWidget */

AbstractBoxWidget::FlexInfo
    AbstractBoxWidget::_get_space_flex_info(
        const std::vector<AbstractBoxWidget::SpacingInfo> &spacing_infos)
{
    FlexInfo info{0, 0, 0};
    for (auto& spacing: spacing_infos) {
        info.space_sum += spacing.margin;
        if (spacing.size < 0) {
            info.flex_sum += spacing.flex;
        } else {
            info.static_width_sum += spacing.size;
        }
    }
    return info;
}

void AbstractBoxWidget::_get_spacing_list(
    std::vector<SpacingInfo> &results,
    coord_int_t base_spacing)
{
    Style &my_style = computed_style();

    results.reserve(size()+1);

    auto it = cbegin();
    WidgetPtr left = nullptr, curr = nullptr;
    bool term = false;
    for (; (!term) && ((it != cend()) || (curr)); ++it)
    {
        if (it == cend()) {
            term = true;
            curr = nullptr;
        } else {
            curr = *it;
        }
        if (curr && !curr->get_visible()) {
            continue;
        }

        coord_int_t left_space = (
            left ? get_aa_box_max_edge(left->computed_style().margin())
                 : get_aa_box_min_edge(my_style.padding()));

        if (left_space < 0) {
            left_space += base_spacing;
        } else if (left && curr) {
            left_space = std::max(base_spacing, left_space);
        }

        SpacingInfo info;
        info.widget = curr;
        if (curr) {
            Style &curr_style = curr->computed_style();
            coord_int_t my_left_margin = get_aa_box_min_edge(curr_style.margin());
            coord_int_t my_right_margin = get_aa_box_max_edge(curr_style.margin());
            info.flex = curr_style.get_flex();
            info.size = get_aa_dimension(curr->get_dimensions());

            if (my_left_margin < 0) {
                info.margin = left_space + my_left_margin;
            } else {
                info.margin = std::max(my_left_margin, left_space);
            }
        } else {
            info.margin = left_space + get_aa_box_max_edge(my_style.padding());
            info.flex = 0;
            info.size = -1;
        }

        results.push_back(info);
        left = curr;
    }
}

void AbstractBoxWidget::_do_box_align(
    const std::vector<AbstractBoxWidget::SpacingInfo> &spacing_list,
    coord_int_t my_aa_pos,
    coord_int_t my_fa_pos,
    coord_int_t my_aa_size,
    coord_int_t my_fa_size,
    coord_int_t my_fa_padding_a,
    coord_int_t my_fa_padding_b)
{
    FlexInfo total = _get_space_flex_info(spacing_list);
    coord_float_t widget_width_per_flex = 1;
    if (total.flex_sum > 0) {
        widget_width_per_flex =
            (my_aa_size - (total.space_sum+total.static_width_sum)) / total.flex_sum;
    }

    if (ceil(widget_width_per_flex) <= 0) {
        for (auto& widget: *this) {
            widget->absolute_rect() = Rect(0, 0, 0, 0);
        }
        /* TODO: log warning here */
        return;
    }

    coord_int_t fa_pos_a = my_fa_pos;
    coord_int_t fa_pos_b = fa_pos_a + my_fa_size;
    coord_float_t aa_pos_a = spacing_list[0].margin + my_aa_pos;

    for (auto& spacing: spacing_list) {
        if (!spacing.widget) {
            continue;
        }
        aa_pos_a += spacing.margin;

        coord_float_t widget_width = (
            spacing.size < 0 ? spacing.flex * widget_width_per_flex
                             : spacing.size);

        Margin &widget_margin = spacing.widget->computed_style().margin();

        coord_int_t fa_spacing_a = my_fa_padding_a + get_fa_box_min_edge(widget_margin);
        coord_int_t fa_spacing_b = my_fa_padding_b + get_fa_box_max_edge(widget_margin);

        if (fa_pos_a + fa_spacing_a < fa_pos_b - fa_spacing_b) {
            Rect &widget_rect = spacing.widget->absolute_rect();
            set_fa_position(
                widget_rect,
                std::make_pair(fa_pos_a + fa_spacing_a, fa_pos_b - fa_spacing_b)
                );
            set_aa_position(
                widget_rect,
                std::make_pair(round(aa_pos_a), round(aa_pos_a+widget_width))
                );
            if (spacing.widget->get_client_rect().area() < 0) {
                spacing.widget->absolute_rect() = Rect(0, 0, 0, 0);
            }
        } else {
            spacing.widget->absolute_rect() = Rect(0, 0, 0, 0);
        }

        aa_pos_a += widget_width;
    }
}

/* PyEngine::AbstractHBox */

coord_int_t AbstractHBox::get_aa_box_min_edge(const CSSBox &box)
{
    return box.get_left();
}

coord_int_t AbstractHBox::get_aa_box_max_edge(const CSSBox &box)
{
    return box.get_right();
}

coord_int_t AbstractHBox::get_fa_box_min_edge(const CSSBox &box)
{
    return box.get_top();
}

coord_int_t AbstractHBox::get_fa_box_max_edge(const CSSBox &box)
{
    return box.get_bottom();
}

coord_int_t AbstractHBox::get_aa_size(const Rect &rect)
{
    return rect.get_width();
}

coord_int_t AbstractHBox::get_aa_dimension(
    const coord_dimensions_t &dim)
{
    return dim.first;
}

void AbstractHBox::set_aa_position(
    Rect &rect,
    const coord_int_pair_t &values)
{
    rect.set_x(values.first);
    rect.set_right(values.second);
}

void AbstractHBox::set_fa_position(
    Rect &rect,
    const coord_int_pair_t &values)
{
    rect.set_y(values.first);
    rect.set_bottom(values.second);
}

void AbstractHBox::do_align()
{
    if (size() == 0) {
        return;
    }
    Style &mystyle = computed_style();
    std::vector<SpacingInfo> spacing_list;
    _get_spacing_list(spacing_list, mystyle.get_box_spacing_x());

    Rect rect = absolute_rect();
    rect.shrink(mystyle.border().get_box());
    _do_box_align(
        spacing_list,
        rect.get_left(),
        rect.get_top(),
        rect.get_width(),
        rect.get_height(),
        mystyle.padding().get_top(),
        mystyle.padding().get_bottom()
    );
}

/* PyEngine::AbstractVBox */

coord_int_t AbstractVBox::get_aa_box_min_edge(const CSSBox &box)
{
    return box.get_top();
}

coord_int_t AbstractVBox::get_aa_box_max_edge(const CSSBox &box)
{
    return box.get_bottom();
}

coord_int_t AbstractVBox::get_fa_box_min_edge(const CSSBox &box)
{
    return box.get_left();
}

coord_int_t AbstractVBox::get_fa_box_max_edge(const CSSBox &box)
{
    return box.get_right();
}

coord_int_t AbstractVBox::get_aa_size(const Rect &rect)
{
    return rect.get_height();
}

coord_int_t AbstractVBox::get_aa_dimension(
    const coord_dimensions_t &dim)
{
    return dim.first;
}

void AbstractVBox::set_aa_position(
    Rect &rect,
    const coord_int_pair_t &values)
{
    rect.set_y(values.first);
    rect.set_bottom(values.second);
}

void AbstractVBox::set_fa_position(
    Rect &rect,
    const coord_int_pair_t &values)
{
    rect.set_x(values.first);
    rect.set_right(values.second);
}

void AbstractVBox::do_align()
{
    if (size() == 0) {
        return;
    }
    Style &mystyle = computed_style();
    std::vector<SpacingInfo> spacing_list;
    _get_spacing_list(spacing_list, mystyle.get_box_spacing_y());

    Rect rect = absolute_rect();
    rect.shrink(mystyle.border().get_box());
    _do_box_align(
        spacing_list,
        rect.get_top(),
        rect.get_left(),
        rect.get_height(),
        rect.get_width(),
        mystyle.padding().get_left(),
        mystyle.padding().get_right()
    );
}

/* PyEngine::HBox */

bool HBox::is_element(const std::string &name) const
{
    return (name == "hbox") || AbstractHBox::is_element(name);
}

/* PyEngine::VBox */

bool VBox::is_element(const std::string &name) const
{
    return (name == "vbox") || AbstractVBox::is_element(name);
}

}
