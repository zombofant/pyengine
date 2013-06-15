/**********************************************************************
File name: Shapes.hpp
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
#ifndef _PYE_UI_SHAPES_H
#define _PYE_UI_SHAPES_H


#include <ostream>
#include <stdexcept>
#include <cmath>

#include "CSS.hpp"

namespace PyEngine {

class BoxError: public std::invalid_argument
{
public:
    BoxError(const std::string& what_arg);
    BoxError(const char* what_arg);
};

class RectError: public std::invalid_argument
{
public:
    RectError(const std::string& what_arg);
    RectError(const char* what_arg);
};

template <typename _coord_t>
class GenericPoint
{
public:
    typedef _coord_t coord_t;
public:
    GenericPoint():
        _x(0), _y(0)
    {
    };
    GenericPoint(coord_t x, coord_t y):
        _x(x), _y(y)
    {
    };
    GenericPoint(const GenericPoint<coord_t>& ref):
        _x(ref._x), _y(ref._y)
    {
    };
    GenericPoint<coord_t>& operator= (const GenericPoint<coord_t>& ref)
    {
        _x = ref._x;
        _y = ref._y;
        return *this;
    };
protected:
    coord_t _x, _y;
public:
    inline void set_x(const coord_t value) {
        _x = value;
    };
    inline void set_y(const coord_t value) {
        _y = value;
    };

    inline coord_t get_x() const {
        return _x;
    };
    inline coord_t get_y() const {
        return _y;
    };
public:
    bool operator== (const GenericPoint<coord_t>& b) const {
        return ((_x == b._x) &&
                (_y == b._y));
    };
    bool operator!= (const GenericPoint<coord_t>& b) const {
        return !(operator==(b));
    };
};

class Point: public GenericPoint<coord_int_t>
{
public:
    Point();
    Point(coord_int_t x, coord_int_t y);
    Point(const Point& ref);
    Point& operator=(const Point& ref);
};

template <typename coord_t>
class GenericBox
{
public:
    GenericBox():
        _left(0), _top(0), _right(0), _bottom(0)
    {

    };

    explicit GenericBox(coord_t value):
        _left(value),
        _top(value),
        _right(value),
        _bottom(value)
    {

    };
    GenericBox(coord_t left, coord_t top,
               coord_t right, coord_t bottom):
        _left(left),
        _top(top),
        _right(right),
        _bottom(bottom)
    {

    };
    GenericBox(const GenericBox<coord_t>& ref):
        _left(ref._left),
        _top(ref._top),
        _right(ref._right),
        _bottom(ref._bottom)
    {

    };
    GenericBox<coord_t>& operator= (const GenericBox<coord_t>& ref)
    {
        _left = ref._left;
        _right = ref._right;
        _top = ref._top;
        _bottom = ref._bottom;
        return *this;
    };
protected:
    coord_t _left, _top, _right, _bottom;
protected:
    virtual void _check_value(const coord_t value) const {
        if (Auto::is_auto(value)) {
            throw BoxError("Non-margin boxes do not support Auto value.");
        }
        if (value < 0) {
            throw BoxError("Non-margin boxes do not allow negative values.");
        }
    };
public:
    inline void set_left(const coord_t value) {
        _check_value(value);
        _left = value;
    };
    inline void set_top(const coord_t value) {
        _check_value(value);
        _top = value;
    };
    inline void set_right(const coord_t value) {
        _check_value(value);
        _right = value;
    };
    inline void set_bottom(const coord_t value) {
        _check_value(value);
        _bottom = value;
    };

    inline coord_t get_left() const {
        return _left;
    };
    inline coord_t get_top() const {
        return _top;
    };
    inline coord_t get_right() const {
        return _right;
    };
    inline coord_t get_bottom() const {
        return _bottom;
    };

    inline coord_t get_horizontal() const {
        return get_left() + get_right();
    };
    inline coord_t get_vertical() const {
        return get_top() + get_bottom();
    };
public:
    inline bool operator==(const GenericBox<coord_t>& b) const {
        return ((_left == b._left) &&
                (_top == b._top) &&
                (_right == b._right) &&
                (_bottom == b._bottom));
    };
    inline bool operator!=(const GenericBox<coord_t>& b) const {
        return !(operator==(b));
    };
};

class Box: public GenericBox<coord_int_t>
{
public:
    Box();
    explicit Box(coord_int_t value);
    Box(coord_int_t left, coord_int_t right,
        coord_int_t top, coord_int_t bottom);
    Box(const Box& ref);
    Box& operator=(const Box& ref);
};

class FloatBox: public GenericBox<coord_float_t>
{
public:
    FloatBox();
    explicit FloatBox(coord_float_t value);
    FloatBox(coord_float_t left, coord_float_t right,
             coord_float_t top, coord_float_t bottom);
    FloatBox(const FloatBox& ref);
    FloatBox(const Box& ref);
    FloatBox& operator=(const Box& ref);
    FloatBox& operator=(const FloatBox& ref);
public:
    inline bool operator==(const Box& b) const;
    inline bool operator!=(const Box& b) const;
};

template <typename _coord_t>
class GenericRect
{
public:
    typedef _coord_t coord_t;
public:
    GenericRect():
        _left(0), _top(0), _width(-1), _height(-1)
    {
    };

    GenericRect(coord_t left, coord_t top):
        _left(left),
        _top(top),
        _width(0),
        _height(0)
    {
    };

    GenericRect(coord_t left, coord_t top, coord_t right, coord_t bottom):
        _left(left),
        _top(top),
        _width(0),
        _height(0)
    {
        set_right(right);
        set_bottom(bottom);
    };

    GenericRect(const GenericRect<coord_t>& ref):
        _left(ref._left),
        _top(ref._top),
        _width(ref._width),
        _height(ref._height)
    {
    };

    GenericRect<coord_t>& operator=(const GenericRect<coord_t>& ref)
    {
        _left = ref._left;
        _top = ref._top;
        _width = ref._width;
        _height = ref._height;
        return *this;
    };

    ~GenericRect() {};
protected:
    coord_t _left, _top;
    coord_t _width, _height;
protected:
    inline void check_dimension(const coord_t value,
                                const std::string which) {
        if (value < 0) {
            throw RectError(which + " must be non-negative.");
        }
    };
public:
    inline bool is_a_rect() const {
        return (_width >= 0) && (_height >= 0);
    };

    inline void set_x(const coord_t value) {
        _left = value;
    };
    inline void set_y(const coord_t value) {
        _top = value;
    };
    inline void set_width(const coord_t value) {
        _width = value;
    };
    inline void set_height(const coord_t value) {
        _height = value;
    };

    inline void set_left(const coord_t value) {
        if (value > get_right()) {
            throw RectError("left must be less than or equal to right.");
        }
        _left = value;
    };
    inline void set_top(const coord_t value) {
        if (value > get_bottom()) {
            throw RectError("top must be less than or equal to bottom.");
        }
        _top = value;
    };
    inline void set_right(const coord_t value) {
        if (value < _left) {
            throw RectError("right must be greater than or equal to left.");
        }
        _width = value - _left;
    };
    inline void set_bottom(const coord_t value) {
        if (value < _top) {
            throw RectError("bottom must be greater than or equal to top.");
        }
        _height = value - _top;
    };

    inline coord_t get_x() const {
        return _left;
    };
    inline coord_t get_y() const {
        return _top;
    };
    inline coord_t get_width() const {
        return _width;
    };
    inline coord_t get_height() const {
        return _height;
    };

    inline coord_t get_left() const {
        return _left;
    };
    inline coord_t get_top() const {
        return _top;
    };
    inline coord_t get_right() const {
        return _left + _width;
    };
    inline coord_t get_bottom() const {
        return _top + _height;
    };
public:
    coord_t area() const {
        if (!is_a_rect()) {
            return 0;
        }
        return _width * _height;
    };

private:
    /* SFINAE rules! */
    template <typename rect_t>
    inline auto contains_impl(const rect_t& b, long v = 0) const
        -> decltype(b.is_a_rect())
    {
        if (!b.is_a_rect()) {
            return true;
        }
        if (!is_a_rect()) {
            return false;
        }

        const coord_t me_right = get_right();
        const coord_t me_bottom = get_bottom();

        const coord_t b_right = b.get_right();
        const coord_t b_bottom = b.get_bottom();

        return ((_left <= b._left) &&
                (_top <= b._top) &&
                (me_right >= b_right) &&
                (me_bottom >= b_bottom));
    }

    template <typename point_t>
    inline bool contains_impl(const point_t& p, int v = 0) const
    {

        const coord_t me_right = get_right();
        const coord_t me_bottom = get_bottom();

        const coord_t p_x = p.get_x();
        const coord_t p_y = p.get_y();

        return (_left <= p_x && p_x < me_right &&
                _top <= p_y && p_y < me_bottom);
    }
public:
    template <typename other_t>
    bool contains(const other_t& b) const {
        return contains_impl<other_t>(b, 0L);
    }


    void translate(const coord_t dx, const coord_t dy) {
        _left += dx;
        _top += dy;
    };

    template <typename point_t>
    void translate(const Point& point) {
        _left += point.get_x();
        _top += point.get_y();
    }
public:
    template <typename rect_t>
    inline bool operator==(const rect_t& b) const {
        return ((_left == b._left) &&
                (_top == b._top) &&
                (_width == b._width) &&
                (_height == b._height));
    }

    template <typename rect_t>
    inline bool operator!=(const rect_t& b) const {
        return !(operator==(b));
    }

    template <typename rect_t>
    GenericRect<coord_t>& operator&=(const rect_t& b) {
        static_assert(std::is_convertible<typename rect_t::coord_t, coord_t>::value,
                      "Coordinate type of right operand must be convertible to coordinate type of left operand.");

        if (!is_a_rect() || !b.is_a_rect()) {
            return *this;
        }
        const coord_t new_left = std::max(_left, b._left);
        const coord_t new_top = std::max(_top, b._top);
        const coord_t new_right = std::min(get_right(), b.get_right());
        const coord_t new_bottom = std::min(get_bottom(), b.get_bottom());
        if (new_left > new_right || new_top > new_bottom) {
            _width = -1;
            _height = -1;
        }
        _left = new_left;
        _top = new_top;
        _width = new_right - new_left;
        _height = new_bottom - new_top;
        return *this;
    }

    template <typename rect_t>
    GenericRect<coord_t> operator&(const rect_t& b) const {
        GenericRect<coord_t> result = GenericRect<coord_t>(*this);
        result &= b;
        return std::move(result);
    }

    template <typename rect_t>
    GenericRect<coord_t>& operator|=(const rect_t& b) {
        static_assert(std::is_convertible<typename rect_t::coord_t, coord_t>::value,
                      "Coordinate type of right operand must be convertible to coordinate type of left operand.");

        if (!is_a_rect() || !b.is_a_rect()) {
            return *this;
        }

        if (_left == b._left && _width == b._width) {
            const coord_t me_bottom = get_bottom();
            const coord_t b_bottom = b.get_bottom();
            if (b._top <= me_bottom && _top <= b_bottom) {
                _top = std::min(_top, b._top);
                _height = std::max(me_bottom, b_bottom) - _top;
            } else {
                throw RectError("Cannot combine rects (not adjacent).");
            }
        } else if (_top == b._top && _height == b._height) {
            const coord_t me_right = get_right();
            const coord_t b_right = b.get_right();
            if (b._left <= me_right && _left <= b_right) {
                _left = std::min(_left, b._left);
                _width = std::max(me_right, b_right) - _left;
            } else {
                throw RectError("Cannot combine rects (not adjacent).");
            }

        } else {
            throw RectError("Cannot combine rects (not aligned).");
        }
        return *this;
    }

    template <typename rect_t>
    GenericRect<coord_t> operator|(const rect_t& b) const {
        GenericRect<coord_t> result = GenericRect<coord_t>(*this);
        result |= b;
        return std::move(result);
    }
};

class Rect: public GenericRect<coord_int_t>
{
public:
    Rect();
    Rect(coord_int_t top, coord_int_t left);
    Rect(coord_int_t left, coord_int_t top,
         coord_int_t right, coord_int_t bottom);
    Rect(const Rect& ref);
    Rect& operator=(const Rect& ref);
};

class CSSBox: public GenericBox<css_coord_int_t>
{
public:
    CSSBox();
    explicit CSSBox(css_coord_int_t value);
    CSSBox(css_coord_int_t left, css_coord_int_t top,
           css_coord_int_t right, css_coord_int_t bottom);
    CSSBox(const CSSBox& ref);
    CSSBox(const Box& ref);
    CSSBox& operator=(const CSSBox& ref);
    CSSBox& operator=(const Box& ref);
};

typedef CSSBox Padding;

class Margin: public CSSBox
{
public:
    Margin();
    explicit Margin(css_coord_int_t value);
    Margin(css_coord_int_t left, css_coord_int_t top,
           css_coord_int_t right, css_coord_int_t bottom);
    Margin(const Margin& ref);
    Margin(const CSSBox& ref);
    Margin(const Box& ref);
    Margin& operator=(const Margin& ref);
    Margin& operator=(const CSSBox& ref);
    Margin& operator=(const Box& ref);
protected:
    virtual void _check_value(const css_coord_int_t value) const;
public:
    template <typename rect_t>
    void deautoify(const rect_t& obj, const rect_t& outer) {
        const coord_int_t hspace = round(outer.get_width() - obj.get_width());
        const coord_int_t vspace = round(outer.get_height() - obj.get_height());

        if (Auto::is_auto(_left) && Auto::is_auto(_right)) {
            _left = std::max(0, hspace / 2);
            _right = std::max(0, hspace - _left);
        } else if (Auto::is_auto(_left)) {
            _left = std::max(0, hspace - _right);
        } else if (Auto::is_auto(_right)) {
            _right = std::max(0, hspace - _left);
        }

        if (Auto::is_auto(_top) && Auto::is_auto(_bottom)) {
            _top = std::max(0, vspace / 2);
            _bottom = std::max(0, vspace - _top);
        } else if (Auto::is_auto(_top)) {
            _top = std::max(0, vspace - _bottom);
        } else if (Auto::is_auto(_bottom)) {
            _bottom = std::max(0, vspace - _top);
        }
    }
};

template <typename box_value_t>
std::ostream& box_to_stream(std::ostream& stream,
                            const GenericBox<box_value_t> &box)
{
    return stream << "left=" << box.get_left() << ", "
                  << "top=" << box.get_top() << ", "
                  << "right=" << box.get_right() << ", "
                  << "bottom=" << box.get_bottom();
}

}

namespace std {

inline ostream& operator<<(ostream& stream, const PyEngine::CSSBox &pad)
{
    return PyEngine::box_to_stream(stream << "CSSBox(", pad) << ")";
}

inline ostream& operator<<(ostream& stream, const PyEngine::Margin &margin)
{
    return PyEngine::box_to_stream(stream << "Margin(", margin) << ")";
}

}

#endif
