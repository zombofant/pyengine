/**********************************************************************
File name: Border.hpp
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
#ifndef _PYE_UI_BORDER_H
#define _PYE_UI_BORDER_H

#include <array>
#include <ostream>

#include "CSS.hpp"
#include "Shapes.hpp"
#include "Fill.hpp"

namespace PyEngine { namespace UI {

class BorderComponent
{
public:
    virtual ~BorderComponent();
public:
    virtual CSSFillPtr get_fill() const = 0;
    virtual css_coord_int_t get_width() const = 0;
};

class Border;

class BorderEdge: public BorderComponent
{
public:
    BorderEdge();
    BorderEdge(css_coord_int_t width, CSSFillPtr fill);
    BorderEdge(const BorderComponent& ref);
    BorderEdge(const BorderEdge& ref);
    BorderEdge& operator=(const BorderComponent& ref);
    BorderEdge& operator=(const BorderEdge& ref);
    virtual ~BorderEdge();

private:
    css_coord_int_t _width;
    CSSFillPtr _fill;

public:
    virtual CSSFillPtr get_fill() const;
    virtual css_coord_int_t get_width() const;

    void set_fill(const CSSFillPtr& value);
    void set_width(const css_coord_int_t& value);

    friend class Border;

public:
    bool operator==(const BorderEdge& other) const;
    inline bool operator!=(const BorderEdge& other) const {
        return !(operator==(other));
    };

    void deinherit_with(const BorderEdge &other);
};

class Border: public BorderComponent
{
public:
    Border();
    Border(css_coord_int_t width, CSSFillPtr fill);
    Border(const BorderComponent& ref);
    Border(const Border& ref);
    Border& operator=(const BorderComponent& ref);
    Border& operator=(const Border& ref);
    virtual ~Border();

private:
    std::array<BorderEdge, 4> _edges;
    std::array<css_coord_int_t, 4> _corners;

public:
    virtual CSSFillPtr get_fill() const;
    virtual css_coord_int_t get_width() const;

    void set_fill(const CSSFillPtr& value);
    void set_radius(const css_coord_int_t& value);
    void set_width(const css_coord_int_t& value);

public:
    Box get_box() const;
    FloatBox get_half_box() const;

public:
    inline BorderEdge& left() {
        return _edges[0];
    };

    inline const BorderEdge& left() const {
        return _edges[0];
    };

    inline BorderEdge& top() {
        return _edges[1];
    };

    inline const BorderEdge& top() const {
        return _edges[1];
    };

    inline BorderEdge& right() {
        return _edges[2];
    };

    inline const BorderEdge& right() const {
        return _edges[2];
    };

    inline BorderEdge& bottom() {
        return _edges[3];
    };

    inline const BorderEdge& bottom() const {
        return _edges[3];
    };


    inline css_coord_int_t get_top_left_radius() const {
        return _corners[0];
    };
    inline css_coord_int_t get_top_right_radius() const {
        return _corners[1];
    };
    inline css_coord_int_t get_bottom_right_radius() const {
        return _corners[2];
    };
    inline css_coord_int_t get_bottom_left_radius() const {
        return _corners[3];
    };

    inline void set_top_left_radius(const css_coord_int_t& value) {
        _corners[0] = value;
    };
    inline void set_top_right_radius(const css_coord_int_t& value) {
        _corners[1] = value;
    };
    inline void set_bottom_right_radius(const css_coord_int_t& value) {
        _corners[2] = value;
    };
    inline void set_bottom_left_radius(const css_coord_int_t& value) {
        _corners[3] = value;
    };

    inline std::array<coord_int_t, 4> get_widths() const {
        return std::array<coord_int_t, 4>{{
            _edges[0].get_width(),
            _edges[1].get_width(),
            _edges[2].get_width(),
            _edges[3].get_width()
        }};
    };

    inline std::array<FillPtr, 4> get_fills() const {
        return std::array<FillPtr, 4>{{
            _edges[0].get_fill().get(),
            _edges[1].get_fill().get(),
            _edges[2].get_fill().get(),
            _edges[3].get_fill().get()
        }};
    };

public:
    bool operator==(const Border& other) const;
    inline bool operator!=(const Border& other) const {
        return !(operator==(other));
    };

    void deinherit_with(const Border &other);
};

}
}

namespace std {

inline ostream& operator<<(ostream &stream, const PyEngine::UI::BorderEdge &edge)
{
    return stream << "BorderEdge("
                  << "width=" << edge.get_width() << ", "
                  << "fill=" << edge.get_fill() << ")";
}

inline ostream& operator<<(ostream &stream, const PyEngine::UI::Border &border)
{
    return stream << "Border("
                  << "top=" << border.top() << ", "
                  << "left=" << border.left() << ", "
                  << "right=" << border.right() << ", "
                  << "bottom=" << border.bottom() << ", "
                  << "top_left_radius=" << border.get_top_left_radius() << ", "
                  << "top_right_radius=" << border.get_top_right_radius() << ", "
                  << "bottom_left_radius=" << border.get_bottom_left_radius() << ", "
                  << "bottom_right_radius=" << border.get_bottom_right_radius() << ")";
}

}

#endif
