#include "Shapes.hpp"

namespace PyEngine {

/* PyEngine::BoxError */

BoxError::BoxError(const std::string& what_arg):
    std::invalid_argument(what_arg)
{

}

BoxError::BoxError(const char* what_arg):
    std::invalid_argument(what_arg)
{

}

/* PyEngine::RectError */

RectError::RectError(const std::string& what_arg):
    std::invalid_argument(what_arg)
{

}

RectError::RectError(const char* what_arg):
    std::invalid_argument(what_arg)
{

}

/* PyEngine::Point */

Point::Point():
    _x(0), _y(0)
{

}

Point::Point(coord_int_t x, coord_int_t y):
    _x(x), _y(y)
{

}

Point::Point(const Point& ref):
    _x(ref._x),
    _y(ref._y)
{

}

Point& Point::operator= (const Point& ref)
{
    _x = ref._x;
    _y = ref._y;
    return *this;
}

bool Point::operator== (const Point& b) const
{
    return ((_x == b._x) &&
            (_y == b._y));
}

bool Point::operator!= (const Point& b) const
{
    return !(operator==(b));
}

/* PyEngine::Box */

Box::Box():
    _left(0), _top(0), _right(0), _bottom(0)
{

}

Box::Box(coord_int_t value):
    _left(value),
    _top(value),
    _right(value),
    _bottom(value)
{

}

Box::Box(coord_int_t left, coord_int_t right,
         coord_int_t top, coord_int_t bottom):
    _left(left),
    _top(top),
    _right(right),
    _bottom(bottom)
{

}

Box::Box(const Box& ref):
    _left(ref._left),
    _top(ref._top),
    _right(ref._right),
    _bottom(ref._bottom)
{

}

Box& Box::operator= (const Box& ref)
{
    _left = ref._left;
    _top = ref._top;
    _right = ref._right;
    _bottom = ref._bottom;
    return *this;
}

void Box::_check_value(const coord_int_t value) const
{
    if (value == AUTO) {
        throw BoxError("Non-margin boxes do not support AUTO value.");
    }
    if (value < 0) {
        throw BoxError("Non-margin boxes do not allow negative values.");
    }
}

void Box::assign(const Box& ref)
{
    *this = ref;
}

inline bool Box::operator==(const Box& b) const
{
    return ((_left == b._left) &&
            (_top == b._top) &&
            (_right == b._right) &&
            (_bottom == b._bottom));
}

inline bool Box::operator!=(const Box& b) const
{
    return !(operator==(b));
}

/* PyEngine::Rect */

Rect::Rect():
    _left(0), _top(0), _width(-1), _height(-1)
{

}

Rect::Rect(coord_int_t top, coord_int_t left):
    _left(left),
    _top(top),
    _width(0),
    _height(0)
{

}

Rect::Rect(coord_int_t left, coord_int_t top,
           coord_int_t right, coord_int_t bottom):
    _left(left),
    _top(top),
    _width(0),
    _height(0)
{
    set_right(right);
    set_bottom(bottom);
}

Rect::Rect(const Rect &ref):
    _left(ref._left),
    _top(ref._top),
    _width(ref._width),
    _height(ref._height)
{

}

Rect& Rect::operator= (const Rect &ref)
{
    _left = ref._left;
    _top = ref._top;
    _width = ref._width;
    _height = ref._height;
    return *this;
}

Rect::~Rect()
{

}

void Rect::set_x(const coord_int_t value)
{
    _left = value;
}

void Rect::set_y(const coord_int_t value)
{
    _top = value;
}

void Rect::set_width(const coord_int_t value)
{
    check_dimension(value, "width");
    _width = value;
}

void Rect::set_height(const coord_int_t value)
{
    check_dimension(value, "height");
    _height = value;
}

void Rect::set_left(const coord_int_t value)
{
    if (value > get_right()) {
        throw RectError("left must be less than or equal to right.");
    }
    _left = value;
}

void Rect::set_top(const coord_int_t value)
{
    if (value > get_bottom()) {
        throw RectError("top must be less than or equal to bottom.");
    }
    _top = value;
}

void Rect::set_right(const coord_int_t value)
{
    if (value < _left) {
        throw RectError("right must be greater than or equal to left.");
    }
    _width = value - _left;
}

void Rect::set_bottom(const coord_int_t value)
{
    if (value < _top) {
        throw RectError("bottom must be greater than or equal to top.");
    }
    _height = value - _top;
}

coord_int_t Rect::area() const
{
    if (is_a_rect()) {
        return 0;
    }
    return _width * _height;
}

void Rect::assign(const Rect& ref)
{
    *this = ref;
}

bool Rect::contains(const Rect& b) const
{
    if (!b.is_a_rect()) {
        return true;
    }
    if (!is_a_rect()) {
        return false;
    }

    const coord_int_t me_right = get_right();
    const coord_int_t me_bottom = get_bottom();

    const coord_int_t b_right = b.get_right();
    const coord_int_t b_bottom = b.get_bottom();

    return ((_left <= b._left) &&
            (_top <= b._top) &&
            (me_right >= b_right) &&
            (me_bottom >= b_bottom));
}

bool Rect::contains(const Point& p) const
{
    const coord_int_t me_right = get_right();
    const coord_int_t me_bottom = get_bottom();

    const coord_int_t p_x = p.get_x();
    const coord_int_t p_y = p.get_y();

    return (_left <= p_x && p_x < me_right &&
            _top <= p_y && p_y < me_bottom);
}

void Rect::translate(const coord_int_t dx, const coord_int_t dy)
{
    _left += dx;
    _top += dy;
}

void Rect::translate(const Point& point)
{
    _left += point.get_x();
    _top += point.get_y();
}

inline bool Rect::operator==(const Rect& b) const
{
    return (_left == b._left) &&
        (_top == b._top) &&
        (_width == b._width) &&
        (_height == b._height);
}

inline bool Rect::operator!=(const Rect& b) const
{
    return !(operator==(b));
}

Rect& Rect::operator&=(const Rect& b)
{
    if (!is_a_rect() || !b.is_a_rect()) {
        return *this;
    }
    const coord_int_t new_left = std::max(_left, b._left);
    const coord_int_t new_top = std::max(_top, b._top);
    const coord_int_t new_right = std::min(get_right(), b.get_right());
    const coord_int_t new_bottom = std::min(get_bottom(), b.get_bottom());
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

Rect Rect::operator&(const Rect& b) const
{
    Rect result = Rect(*this);
    result &= b;
    return std::move(result);
}

Rect& Rect::operator|=(const Rect& b)
{
    if (!is_a_rect() || !b.is_a_rect()) {
        return *this;
    }

    if (_left == b._left && _width == b._width) {
        const coord_int_t me_bottom = get_bottom();
        const coord_int_t b_bottom = b.get_bottom();
        if (b._top <= me_bottom && _top <= b_bottom) {
            _top = std::min(_top, b._top);
            _height = std::max(me_bottom, b_bottom) - _top;
        } else {
            throw RectError("Cannot combine rects (not adjacent).");
        }
    } else if (_top == b._top && _height == b._height) {
        const coord_int_t me_right = get_right();
        const coord_int_t b_right = b.get_right();
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

Rect Rect::operator|(const Rect& b) const
{
    Rect result = Rect(*this);
    result |= b;
    return result;
}

bool Rect::operator<=(const Rect& b) const
{
    return contains(b);
}

bool Rect::operator<(const Rect& b) const
{
    return (*this <= b) && (*this != b);
}

/* PyEngine::Margin */

Margin::Margin():
    Box::Box()
{

}

Margin::Margin(coord_int_t value):
    Box::Box(value)
{

}

Margin::Margin(coord_int_t left, coord_int_t top,
               coord_int_t right, coord_int_t bottom):
    Box::Box(left, top, right, bottom)
{

}

Margin::Margin(const Box& ref):
    Box::Box(ref)
{

}

Margin::Margin(const Margin& ref):
    Box::Box(ref)
{

}

Margin& Margin::operator= (const Margin& ref)
{
    this->Box::operator=(ref);
    return *this;
}

Margin& Margin::operator= (const Box& ref)
{
    this->Box::operator=(ref);
    return *this;
}

}
