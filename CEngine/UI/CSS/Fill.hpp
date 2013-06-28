/**********************************************************************
File name: Fill.hpp
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

    /**
     * Return a new copy of the object.
     */
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

public:
    virtual bool operator==(const Fill& other) const = 0;
    inline bool operator!=(const Fill& other) const {
        return !(operator==(other));
    };

    virtual void to_stream(std::ostream &stream) const;
};

typedef std::shared_ptr<Fill> FillPtr;
typedef CSSInheritable<FillPtr> CSSFillPtr;

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

    virtual Transparent* copy() const;
public:
    bool operator==(const Fill& other) const override;
    void to_stream(std::ostream &stream) const override;
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
    virtual Colour* copy() const;

public:
    bool operator==(const Fill& oth) const override;
    void to_stream(std::ostream &stream) const override;

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
    virtual Gradient* copy() const;
    bool operator==(const Fill& oth) const override;
    void to_stream(std::ostream &stream) const override;

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
    Image(const Image& ref) = delete;
    Image& operator=(const Image& ref) = delete;
    virtual ~Image();
private:
    cairo_surface_t* _image;
public:
    virtual void set_source(cairo_t* ctx, const Rect& rect) const;
    bool operator==(const Fill& oth) const override;
    void to_stream(std::ostream &stream) const override;
};

}

namespace std
{

inline std::ostream& operator<<(
    std::ostream &stream,
    const PyEngine::Fill &fill)
{
    fill.to_stream(stream);
    return stream;
}

}

#endif
