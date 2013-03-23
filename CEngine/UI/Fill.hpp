#ifndef _PYE_UI_FILL_H
#define _PYE_UI_FILL_H

#include <cairo.h>

#include "CEngine/GL/AbstractImage.hpp"

#include "Shapes.hpp"

namespace PyEngine {

enum RepeatMode {
    RM_REPEAT,
    RM_STRETCH
};

enum GradientDirection {
    GD_HORIZONTAL = 0,
    GD_VERTICAL = 1
};

/**
 * Base class for anything which can fill a rectangle.
 *
 * Do not instanciate this class; instead, refer to its descendants.
 */
class Fill
{
public:
    Fill();
    Fill(const Fill& ref);
    Fill& operator=(const Fill& ref);
    virtual ~Fill();
protected:
    RepeatMode _repeat_x, _repeat_y;
public:
    /**
     * Set the filler as a source for drawing in the current cairo
     * context within the given rectangle rect.
     *
     * @param ctx cairo context to affect.
     * @param rect Rectangle inside which the source shall be valid.
     */
    virtual void set_source(cairo_t* ctx, const Rect& rect) const = 0;

    /**
     * Draw the filler in the given rectangle on the given context.
     *
     * @param ctx cairo context to draw in.
     * @param rect Rectangle to draw in.
     */
    virtual void in_cairo(cairo_t* ctx, const Rect& rect) const;
public:
    inline RepeatMode get_repeat_x() const {
        return _repeat_x;
    };
    inline RepeatMode get_repeat_y() const {
        return _repeat_y;
    };

    virtual void set_repeat_x(const RepeatMode value) {
        _repeat_x = value;
    };
    virtual void set_repeat_y(const RepeatMode value) {
        _repeat_y = value;
    };
};

/**
 * This is a transparent filler.
 */
class Transparent: public Fill
{
public:
    Transparent();
    Transparent(const Transparent& ref);
    Transparent& operator=(const Transparent& ref);
public:
    /**
     * Sets a transparent RGBA colour as source.
     */
    virtual void set_source(cairo_t* ctx, const Rect& rect) const;

    /**
     * Does nothing.
     */
    virtual void in_cairo(cairo_t* ctx, const Rect& rect) const;
};

/**
 * Use a solid colour as filler.
 */
class Colour: public Fill
{
public:
    Colour();
    Colour(double r, double g, double b);
    Colour(double r, double g, double b, double a);
    Colour(const Colour& ref);
    Colour& operator=(const Colour& ref);
private:
    double _r, _g, _b, _a;
public:
    virtual void set_source(cairo_t* ctx, const Rect& rect) const;
public:
    inline double get_r() const {
        return _r;
    };
    inline double get_g() const {
        return _g;
    };
    inline double get_b() const {
        return _b;
    };
    inline double get_a() const {
        return _a;
    };

    void set_r(const double r) {
        _r = std::max(std::min(r, 1.0), 0.0);
    };
    void set_g(const double g) {
        _g = std::max(std::min(g, 1.0), 0.0);
    };
    void set_b(const double b) {
        _b = std::max(std::min(b, 1.0), 0.0);
    };
    void set_a(const double a) {
        _a = std::max(std::min(a, 1.0), 0.0);
    };
};

/**
 * Use a linear gradient as filler.
 */
class Gradient: public Fill
{
public:
    struct Stop
    {
    public:
        Stop(double position, const Colour& colour);
    public:
        double position;
        Colour colour;
    public:
        bool operator< (const Stop& ref);

    };
public:
    Gradient(GradientDirection dir, std::initializer_list<Stop> stop);
    Gradient(const Gradient& ref);
    Gradient& operator=(const Gradient& ref);
    virtual ~Gradient();
private:
    GradientDirection _dir;
    cairo_pattern_t* _patt;
private:
    void copy_pattern(cairo_pattern_t* patt);
public:
    virtual void set_repeat_x(RepeatMode value);
    virtual void set_repeat_y(RepeatMode value);
    virtual void set_source(cairo_t* ctx, const Rect& rect) const;
};

/**
 * Use an image as filler.
 */
class Image: public Fill
{
public:
    Image();
    Image(cairo_surface_t* image);
    Image(GL::AbstractImage2D* image);
    Image(const Image& ref);
    Image& operator=(const Image& ref);
    virtual ~Image();
private:
    cairo_surface_t* _image;
public:
    virtual void set_source(cairo_t* ctx, const Rect& rect) const;
};

}

#endif
