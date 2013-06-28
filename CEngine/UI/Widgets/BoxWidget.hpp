#ifndef _PYE_UI_WIDGET_BOX_H
#define _PYE_UI_WIDGET_BOX_H

#include "CEngine/UI/CSS/Style.hpp"

#include "WidgetBase.hpp"

namespace PyEngine {

class AbstractBoxWidget: public ParentWidget
{
public:
    struct SpacingInfo
    {
        WidgetPtr widget;
        coord_int_t margin;
        unsigned int flex;
        coord_int_t size;
    };

    struct FlexInfo
    {
        coord_int_t space_sum;
        coord_int_t flex_sum;
        coord_int_t static_width_sum;
    };

public:
    AbstractBoxWidget() = default;

protected:
    virtual coord_int_t get_aa_box_min_edge(const CSSBox &box) = 0;
    virtual coord_int_t get_aa_box_max_edge(const CSSBox &box) = 0;
    virtual coord_int_t get_fa_box_min_edge(const CSSBox &box) = 0;
    virtual coord_int_t get_fa_box_max_edge(const CSSBox &box) = 0;
    virtual coord_int_t get_aa_size(const Rect &rect) = 0;
    virtual coord_int_t get_aa_dimension(
        const coord_dimensions_t &dim) = 0;
    virtual void set_aa_position(
        Rect &rect,
        const coord_int_pair_t &values) = 0;
    virtual void set_fa_position(
        Rect &rect,
        const coord_int_pair_t &values) = 0;

    static FlexInfo _get_space_flex_info(
        const std::vector<SpacingInfo> &spacing_infos);

    void _get_spacing_list(
        std::vector<SpacingInfo> &results,
        coord_int_t base_spacing);

    void _do_box_align(
        const std::vector<SpacingInfo> &spacing_list,
        coord_int_t my_aa_pos,
        coord_int_t my_fa_pos,
        coord_int_t my_aa_size,
        coord_int_t my_fa_size,
        coord_int_t my_fa_padding_a,
        coord_int_t my_fa_padding_b);
};

class AbstractHBox: public AbstractBoxWidget
{
public:
    AbstractHBox() = default;

protected:
    coord_int_t get_aa_box_min_edge(const CSSBox &box) override;
    coord_int_t get_aa_box_max_edge(const CSSBox &box) override;
    coord_int_t get_fa_box_min_edge(const CSSBox &box) override;
    coord_int_t get_fa_box_max_edge(const CSSBox &box) override;
    coord_int_t get_aa_size(const Rect &rect) override;
    coord_int_t get_aa_dimension(
        const coord_dimensions_t &dim) override;
    void set_fa_position(
        Rect &rect,
        const coord_int_pair_t &values) override;
    void set_aa_position(
        Rect &rect,
        const coord_int_pair_t &values) override;

public:
    void do_align() override;
};

class AbstractVBox: public AbstractBoxWidget
{
public:
    AbstractVBox() = default;

protected:
    coord_int_t get_aa_box_min_edge(const CSSBox &box) override;
    coord_int_t get_aa_box_max_edge(const CSSBox &box) override;
    coord_int_t get_fa_box_min_edge(const CSSBox &box) override;
    coord_int_t get_fa_box_max_edge(const CSSBox &box) override;
    coord_int_t get_aa_size(const Rect &rect) override;
    coord_int_t get_aa_dimension(
        const coord_dimensions_t &dim) override;
    void set_fa_position(
        Rect &rect,
        const coord_int_pair_t &values) override;
    void set_aa_position(
        Rect &rect,
        const coord_int_pair_t &values) override;

public:
    void do_align() override;
};

class HBox: public AbstractHBox
{
    HBox() = default;
};

class VBox: public AbstractVBox
{
    VBox() = default;
};

}

#endif
