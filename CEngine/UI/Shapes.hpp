#ifndef _PYE_UI_SHAPES_H
#define _PYE_UI_SHAPES_H

#include <stdexcept>
#include <limits>

namespace PyEngine {

typedef int coord_int_t;

const coord_int_t AUTO = std::numeric_limits<coord_int_t>::min();

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

class Point
{
public:
    Point();
    Point(coord_int_t x, coord_int_t y);
    Point(const Point& ref);
    Point& operator= (const Point& ref);
private:
    coord_int_t _x, _y;
public:
    inline void set_x(const coord_int_t value) {
        _x = value;
    };
    inline void set_y(const coord_int_t value) {
        _y = value;
    };

    inline coord_int_t get_x() const {
        return _x;
    };
    inline coord_int_t get_y() const {
        return _y;
    };
public:
    bool operator== (const Point& b) const;
    bool operator!= (const Point& b) const;
};

class Box
{
public:
    Box();
    Box(coord_int_t value);
    Box(coord_int_t left, coord_int_t right,
        coord_int_t top, coord_int_t bottom);
    Box(const Box& ref);
    Box& operator= (const Box& ref);
private:
    coord_int_t _left, _top, _right, _bottom;
protected:
    virtual void _check_value(const coord_int_t value) const;
public:
    inline void set_left(const coord_int_t value) {
        _check_value(value);
        _left = value;
    };
    inline void set_top(const coord_int_t value) {
        _check_value(value);
        _top = value;
    };
    inline void set_right(const coord_int_t value) {
        _check_value(value);
        _right = value;
    };
    inline void set_bottom(const coord_int_t value) {
        _check_value(value);
        _bottom = value;
    };

    inline coord_int_t get_left() const {
        return _left;
    };
    inline coord_int_t get_top() const {
        return _top;
    };
    inline coord_int_t get_right() const {
        return _right;
    };
    inline coord_int_t get_bottom() const {
        return _bottom;
    };

    inline coord_int_t get_horizontal() const {
        return get_left() + get_right();
    };
    inline coord_int_t get_vertical() const {
        return get_top() + get_bottom();
    };
public:
    void assign(const Box& ref);
public:
    inline bool operator==(const Box& b) const;
    inline bool operator!=(const Box& b) const;
};

class Rect
{
public:
    Rect();
    Rect(coord_int_t left, coord_int_t top);
    Rect(coord_int_t left, coord_int_t top, coord_int_t right, coord_int_t bottom);
    Rect(const Rect& ref);
    Rect& operator= (const Rect& ref);
    ~Rect();
private:
    coord_int_t _left, _top;
    coord_int_t _width, _height;
protected:
    inline void check_dimension(const coord_int_t value,
                                const std::string which) {
        if (value < 0) {
            throw RectError(which + " must be non-negative.");
        }
    };
public:
    inline bool is_a_rect() const {
        return (_width >= 0) && (_height >= 0);
    };

    void set_x(const coord_int_t value);
    void set_y(const coord_int_t value);
    void set_width(const coord_int_t value);
    void set_height(const coord_int_t value);

    void set_left(const coord_int_t value);
    void set_top(const coord_int_t value);
    void set_right(const coord_int_t value);
    void set_bottom(const coord_int_t value);

    inline coord_int_t get_x() const {
        return _left;
    };
    inline coord_int_t get_y() const {
        return _top;
    };
    inline coord_int_t get_width() const {
        return _width;
    };
    inline coord_int_t get_height() const {
        return _height;
    };

    inline coord_int_t get_left() const {
        return _left;
    };
    inline coord_int_t get_top() const {
        return _top;
    };
    inline coord_int_t get_right() const {
        return _left + _width;
    };
    inline coord_int_t get_bottom() const {
        return _top + _height;
    };
public:
    coord_int_t area() const;
    void assign(const Rect& ref);
    bool contains(const Rect& b) const;
    bool contains(const Point& p) const;
    void translate(const coord_int_t dx, const coord_int_t dy);
    void translate(const Point& point);
public:
    inline bool operator==(const Rect& b) const;
    inline bool operator!=(const Rect& b) const;
    Rect& operator&=(const Rect& b);
    Rect operator&(const Rect& b) const;
    Rect& operator|=(const Rect& b);
    Rect operator|(const Rect& b) const;
    bool operator<=(const Rect& b) const;
    bool operator<(const Rect& b) const;
};

class Margin: public Box
{
public:
    Margin();
    Margin(coord_int_t value);
    Margin(coord_int_t left, coord_int_t top,
           coord_int_t right, coord_int_t bottom);
    Margin(const Box& ref);
    Margin(const Margin& ref);
    Margin& operator= (const Margin& ref);
    Margin& operator= (const Box& ref);
protected:
    virtual void _check_value(const coord_int_t value) const;
public:
    void deautoify(const Rect& obj, const Rect &outer);
};

}


#endif
