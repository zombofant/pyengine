/**********************************************************************
File name: Vectors.hpp
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
#ifndef _PYE_MATH_VECTORS_H
#define _PYE_MATH_VECTORS_H

#include <cmath>
#include <ostream>
#include <vector>

namespace PyEngine {

enum vector_component_x_t {
    eX = 0
};

enum vector_component_y_t {
    eY = 1
};

enum vector_component_z_t {
    eZ = 2
};

enum vector_component_w_t {
    eW = 3
};

typedef double VectorFloat;

#include "variadic_initializer.hpp"

template <typename vector_float_t, unsigned int _dimension>
struct Vector {
    static constexpr unsigned int dimension = _dimension;

    static_assert(dimension > 0, "Vector dimension must be "
                                 "greater than 0");

    Vector():
        as_array()
    {
        for (unsigned int i = 0; i < dimension; i++) {
            as_array[i] = 0;
        }
    }

    template <typename... other_float_ts>
    explicit Vector(other_float_ts... values):
        as_array()
    {
        static_assert(sizeof...(values) == dimension,
                      "Must initialize vector with exactly N = dim "
                      "elements");
        init_array_with_data<vector_float_t, other_float_ts...>::init(
            &as_array[0], values...);
    }

    template <typename other_float_t, unsigned int other_dimension>
    explicit Vector(const Vector<other_float_t, other_dimension> &vec):
        as_array()
    {
        static_assert(other_dimension > dimension,
                      "Dimension of source vector must be larger than "
                      "dimension of destination vector.");
        for (unsigned int i = 0; i < dimension; i++) {
            as_array[i] = vec.as_array[i];
        }
    }

    template <typename other_float_t>
    Vector(const Vector<other_float_t, dimension> &ref):
        as_array()
    {
        for (unsigned int i = 0; i < dimension; i++) {
            as_array[i] = ref.as_array[i];
        }
    }

    vector_float_t as_array[dimension];

    template <
        typename other_float_t,
        typename out_float_t = decltype(as_array[0] * other_float_t(0))>
    inline out_float_t operator*(
        const Vector<other_float_t, dimension> &oth) const
    {
        out_float_t dotp = 0;
        for (unsigned int i = 0; i < dimension; i++) {
            dotp += as_array[i] * oth.as_array[i];
        }
        return dotp;
    }

    template <typename other_float_t>
    inline Vector& operator+=(
        const Vector<other_float_t, dimension> &oth)
    {
        for (unsigned int i = 0; i < dimension; i++) {
            as_array[i] += oth.as_array[i];
        }
        return *this;
    }

    template <
        typename other_float_t,
        typename out_float_t = decltype(as_array[0] + other_float_t(0))>
    inline Vector<out_float_t, dimension> operator+(
        const Vector<other_float_t, dimension> &oth) const
    {
        Vector<out_float_t, dimension> result(*this);
        result += oth;
        return result;
    }

    template <typename other_float_t>
    inline Vector& operator-=(
        const Vector<other_float_t, dimension> &oth)
    {
        for (unsigned int i = 0; i < dimension; i++) {
            as_array[i] -= oth.as_array[i];
        }
        return *this;
    }

    template <
        typename other_float_t,
        typename out_float_t = decltype(as_array[0] - other_float_t(0))>
    inline Vector<out_float_t, dimension> operator-(
        const Vector<other_float_t, dimension> &oth) const
    {
        Vector<out_float_t, dimension> result(*this);
        result -= oth;
        return result;
    }

    inline Vector operator-() const
    {
        Vector result(*this);
        for (unsigned int i = 0; i < dimension; i++) {
            result.as_array[i] = -result.as_array[i];
        }
        return result;
    }

    inline Vector operator+() const
    {
        return Vector(*this);
    }

    template <typename other_float_t>
    inline Vector& operator/=(const other_float_t &oth)
    {
        for (unsigned int i = 0; i < dimension; i++) {
            as_array[i] /= oth;
        }
        return *this;
    }

    template <typename other_float_t>
    inline Vector operator/(const other_float_t &oth) const
    {
        Vector result(*this);
        result /= oth;
        return result;
    }

    template <typename other_float_t>
    inline Vector& operator*=(const other_float_t &oth)
    {
        for (unsigned int i = 0; i < dimension; i++) {
            as_array[i] *= oth;
        }
        return *this;
    }

    template <typename other_float_t>
    inline Vector operator*(const other_float_t &oth) const
    {
        Vector result(*this);
        result *= oth;
        return result;
    }

    inline vector_float_t length() const
    {
        return sqrt((*this) * (*this));
    }

    inline Vector& normalize()
    {
        vector_float_t l = length();
        if (l == 0) {
            return *this;
        }
        *this /= l;
        return *this;
    }

    inline Vector normalized() const
    {
        Vector result(*this);
        result.normalize();
        return result;
    }

    template <typename other_float_t>
    inline bool operator==(const Vector<other_float_t, dimension> &oth)
    {
        for (unsigned int i = 0; i < dimension; i++) {
            if (as_array[i] != oth.as_array[i]) {
                return false;
            }
        }
        return true;
    }

    template <typename other_float_t>
    inline bool operator!=(const Vector<other_float_t, dimension> &oth)
    {
        return !(*this == oth);
    }

    inline vector_float_t& operator[](unsigned int i)
    {
        return as_array[i];
    }

    inline vector_float_t operator[](unsigned int i) const
    {
        return as_array[i];
    }

    inline vector_float_t& operator[](vector_component_x_t)
    {
        return as_array[0];
    }

    inline vector_float_t& operator[](vector_component_y_t)
    {
        static_assert(dimension > 1, "Attempt to access Y component in "
                                     "Vector with dim <= 1");
        return as_array[1];
    }

    inline vector_float_t& operator[](vector_component_z_t)
    {
        static_assert(dimension > 2, "Attempt to access Z component in "
                                     "Vector with dim <= 2");
        return as_array[2];
    }

    inline vector_float_t& operator[](vector_component_w_t)
    {
        static_assert(dimension > 3, "Attempt to access W component in "
                                     "Vector with dim <= 3");
        return as_array[3];
    }

    inline vector_float_t operator*(vector_component_x_t) const
    {
        return (*this)[eX];
    }

    inline vector_float_t operator*(vector_component_y_t) const
    {
        return (*this)[eY];
    }

    inline vector_float_t operator*(vector_component_z_t) const
    {
        return (*this)[eZ];
    }

    inline vector_float_t operator*(vector_component_w_t) const
    {
        return (*this)[eW];
    }

    vector_float_t abssum() const
    {
        vector_float_t summed = 0;
        for (unsigned int i = 0; i < dimension; i++) {
            summed += abs(as_array[i]);
        }
        return summed;
    }

};

template <typename vector_float_t1, typename vector_float_t2,
          typename out_float_t =
            decltype(vector_float_t1(0) * vector_float_t2(0))>
inline Vector<out_float_t, 3> operator%(
    const Vector<vector_float_t1, 3> &a,
    const Vector<vector_float_t2, 3> &b)
{
    return Vector<out_float_t, 3>(
        a[eY] * b[eZ] - a[eZ] * b[eY],
        a[eZ] * b[eX] - a[eX] * b[eZ],
        a[eX] * b[eY] - a[eY] * b[eX]
    );
}

template <
    typename float_t,
    typename vector_float_t,
    unsigned int dimension,
    typename out_float_t = decltype(float_t(0) * vector_float_t(0))>
inline Vector<vector_float_t, dimension> operator*(
    const float_t scale,
    const Vector<vector_float_t, dimension> &vec)
{
    return vec * scale;
}

typedef Vector<float, 2> Vector2f;
typedef Vector<float, 3> Vector3f;
typedef Vector<float, 4> Vector4f;

typedef Vector<VectorFloat, 2> Vector2;
typedef Vector<VectorFloat, 3> Vector3;
typedef Vector<VectorFloat, 4> Vector4;

namespace {

template <typename vector_float_t, unsigned int dimension>
inline void vector_data_to_stream(
    std::ostream &ostream,
    const Vector<vector_float_t, dimension> &vec)
{
    ostream << "(" << vec[0];
    for (unsigned int i = 1; i < dimension; i++) {
        ostream << ", " << vec[i];
    }
    ostream << ")";
}

template <typename vector_float_t, unsigned int dimension>
struct vector_to_stream;

template <unsigned int dimension>
struct vector_to_stream<float, dimension>
{
    static inline std::ostream& put(
        std::ostream &ostream,
        const Vector<float, dimension> &vec)
    {
        ostream << "vec" << dimension << "f";
        vector_data_to_stream(ostream, vec);
        return ostream;
    }
};

template <unsigned int dimension>
struct vector_to_stream<double, dimension>
{
    static inline std::ostream& put(
        std::ostream &ostream,
        const Vector<double, dimension> &vec)
    {
        ostream << "vec" << dimension << "d";
        vector_data_to_stream(ostream, vec);
        return ostream;
    }
};

}

template <typename vector_float_t, unsigned int dimension>
std::ostream& operator<<(
    std::ostream &ostream,
    const Vector<vector_float_t, dimension> &vec)
{
    return vector_to_stream<vector_float_t, dimension>::put(
        ostream, vec);
}

}

#endif
