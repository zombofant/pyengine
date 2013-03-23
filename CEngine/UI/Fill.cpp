/**********************************************************************
File name: Fill.cpp
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
#include "Fill.hpp"

namespace PyEngine {

static const int GRADIENT_DIRECTION_WH[2][2] = {
    {1, 0},
    {0, 1}
};

inline cairo_pattern_t* cairo_pattern_linear_for_direction(
    GradientDirection dir)
{
    return cairo_pattern_create_linear(
        0, 0,
        GRADIENT_DIRECTION_WH[0][int(dir)],
        GRADIENT_DIRECTION_WH[1][int(dir)]);
}

/* PyEngine::Fill */

Fill::Fill():
    _repeat_x(RM_STRETCH),
    _repeat_y(RM_STRETCH)
{

}

Fill::Fill(const Fill& ref):
    _repeat_x(ref._repeat_x),
    _repeat_y(ref._repeat_y)
{

}

Fill& Fill::operator=(const Fill& ref)
{
    _repeat_x = ref._repeat_x;
    _repeat_y = ref._repeat_y;
    return *this;
}

Fill::~Fill()
{

}

void Fill::in_cairo(cairo_t* ctx, const Rect& rect) const
{
    set_source(ctx, rect);
    cairo_rectangle(ctx, rect.get_x(), rect.get_y(),
                    rect.get_width(), rect.get_height());
    cairo_fill(ctx);
}

/* PyEngine::Transparent */

Transparent::Transparent():
    Fill()
{

}

Transparent::Transparent(const Transparent& ref):
    Fill(ref)
{

}

Transparent& Transparent::operator=(const Transparent& ref)
{
    this->Fill::operator=(ref);
    return *this;
}

void Transparent::set_source(cairo_t* ctx, const Rect& rect) const
{
    cairo_set_source_rgba(ctx, 0, 0, 0, 0);
}

void Transparent::in_cairo(cairo_t* ctx, const Rect& rect) const
{

}

/* PyEngine::Colour */

Colour::Colour():
    Fill(),
    _r(0), _g(0), _b(0), _a(0)
{

}

Colour::Colour(double r, double g, double b):
    Fill(),
    _r(r), _g(g), _b(b), _a(1)
{

}

Colour::Colour(double r, double g, double b, double a):
    Fill(),
    _r(r), _g(g), _b(b), _a(a)
{

}

Colour::Colour(const Colour& ref):
    Fill(ref),
    _r(ref._r), _g(ref._g), _b(ref._b), _a(ref._a)
{

}

Colour& Colour::operator=(const Colour &ref)
{
    this->Fill::operator=(ref);
    _r = ref._r;
    _g = ref._g;
    _b = ref._b;
    _a = ref._a;
    return *this;
}

void Colour::set_source(cairo_t* ctx, const Rect& rect) const
{
    cairo_set_source_rgba(ctx, _r, _g, _b, _a);
}

/* PyEngine::Gradient::Stop */

Gradient::Stop::Stop(double position, const Colour& colour):
    position(position),
    colour(colour)
{

}

bool Gradient::Stop::operator< (const Stop& ref)
{
    return position < ref.position;
}

/* PyEngine::Gradient */

Gradient::Gradient(GradientDirection dir, std::initializer_list<Stop> stops):
    Fill(),
    _dir(dir),
    _patt(cairo_pattern_linear_for_direction(dir))
{
    // std::array<Step> sorted_steps(steps);
    // std::stable_sort(sorted_steps.begin(), sorted_steps.end());

    for (auto stop: stops) {
        cairo_pattern_add_color_stop_rgba(
            _patt,
            stop.position,
            stop.colour.get_r(),
            stop.colour.get_b(),
            stop.colour.get_g(),
            stop.colour.get_a());
    }
}

Gradient::Gradient(const Gradient& ref):
    Fill(ref),
    _dir(ref._dir),
    _patt(cairo_pattern_linear_for_direction(_dir))
{
    copy_pattern(ref._patt);
}

Gradient& Gradient::operator=(const Gradient& ref)
{
    this->Fill::operator=(ref);
    _dir = ref._dir;

    cairo_pattern_destroy(_patt);
    _patt = cairo_pattern_linear_for_direction(_dir);

    copy_pattern(ref._patt);

    return *this;
}

Gradient::~Gradient()
{
    cairo_pattern_destroy(_patt);
}

void Gradient::copy_pattern(cairo_pattern_t* patt)
{
    int stop_count = 0;
    cairo_pattern_get_color_stop_count(patt, &stop_count);

    for (int i = 0; i < stop_count; i++) {
        double p, r, g, b, a;
        cairo_pattern_get_color_stop_rgba(
            patt, i,
            &p, &r, &g, &b, &a);
        cairo_pattern_add_color_stop_rgba(
            _patt,
            p, r, g, b, a);
    }
}

void Gradient::set_repeat_x(RepeatMode value)
{
    if (value != RM_STRETCH) {
        throw std::invalid_argument("Gradient can only be stretched.");
    }
    this->Fill::set_repeat_x(value);
}

void Gradient::set_repeat_y(RepeatMode value)
{
    if (value != RM_STRETCH) {
        throw std::invalid_argument("Gradient can only be stretched.");
    }
    this->Fill::set_repeat_y(value);
}

void Gradient::set_source(cairo_t* ctx, const Rect& rect) const
{
    cairo_matrix_t mat;
    cairo_get_matrix(ctx, &mat);
    cairo_translate(ctx, rect.get_x(), rect.get_y());
    cairo_scale(ctx, rect.get_width(), rect.get_height());
    cairo_set_source(ctx, _patt);
    cairo_set_matrix(ctx, &mat);
}

/* PyEngine::Image */

Image::Image():
    _image()
{

}

Image::Image(GL::AbstractImage2D* image):
    _image(image->cairoSurface())
{

}

Image::~Image()
{
    cairo_surface_destroy(_image);
}

void Image::set_source(cairo_t* ctx, const Rect& rect) const
{
    const int w = cairo_image_surface_get_width(_image);
    const int h = cairo_image_surface_get_height(_image);
    cairo_matrix_t mat;
    cairo_get_matrix(ctx, &mat);
    cairo_translate(ctx, rect.get_x(), rect.get_y());
    if (_repeat_x == RM_STRETCH && _repeat_y == RM_STRETCH) {
        cairo_scale(ctx, rect.get_width() / w, rect.get_height() / h);
        cairo_set_source_surface(ctx, _image, 0, 0);
    } else {
        if (_repeat_x == RM_STRETCH) {
            cairo_scale(ctx, rect.get_width() / w, 1.0);
        } else if (_repeat_y == RM_STRETCH) {
            cairo_scale(ctx, 1.0, rect.get_height() / h);
        }
        cairo_set_source_surface(ctx, _image, 0, 0);

        cairo_pattern_t* src = cairo_get_source(ctx);
        cairo_pattern_set_extend(src, CAIRO_EXTEND_REPEAT);
    }
    cairo_set_matrix(ctx, &mat);
}

}
