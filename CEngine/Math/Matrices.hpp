/**********************************************************************
File name: Matrices.hpp
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
#ifndef _PYE_MATH_MATRICES_H
#define _PYE_MATH_MATRICES_H

#include "Vectors.hpp"

namespace PyEngine {

enum matrix_identity_t
{
    Identity
};

#include "variadic_initializer.hpp"

template <typename float_t, unsigned int _rows, unsigned int _columns>
struct Matrix
{
    static constexpr unsigned int rows = _rows;
    static constexpr unsigned int columns = _columns;

    Matrix():
        coeff()
    {

    };

    Matrix(matrix_identity_t):
        coeff()
    {
        static_assert(rows == columns,
                      "Identity construction only makes sense for "
                      "square matrices.");
        for (unsigned int n = 0; n < rows; n++) {
            component(n,n) = 1;
        }
    }

    template <typename... other_float_ts>
    explicit Matrix(const other_float_ts... values):
        coeff()
    {
        static_assert(sizeof...(other_float_ts) == rows*columns,
                      "Matrix must be initialized with exactly "
                      "rows*columns elements");
        init_array_with_data<float_t, other_float_ts...>::init(
            coeff,
            values...);
    }

    template <typename other_float_t>
    Matrix(const Matrix<other_float_t, rows, columns> &ref):
        coeff()
    {
        *this = ref;
    };

    template <typename other_float_t>
    Matrix& operator=(const Matrix<other_float_t, rows, columns> &ref)
    {
        for (unsigned int i = 0; i < rows*columns; i++) {
            coeff[i] = ref.coeff[i];
        }
    }

    float_t coeff[_rows*_columns];

    inline float_t& component(unsigned int i, unsigned int j)
    {
        return coeff[i*_columns+j];
    }

    inline float_t component(unsigned int i, unsigned int j) const
    {
        return coeff[i*_columns+j];
    }

    inline Vector<float_t, _columns>& operator[](unsigned int i)
    {
        return static_cast<Vector<float_t, _columns>>(&coeff[i*_columns]);
    }

    template <
        typename other_float_t,
        unsigned int other_columns,
        typename out_float_t = decltype(coeff[0] * other_float_t(0))>
    Matrix<out_float_t, rows, other_columns> operator*(
        const Matrix<other_float_t, columns, other_columns> &mat) const
    {
        Matrix<out_float_t, rows, other_columns> result;
        for (unsigned int i = 0; i < rows; i++) {
            // i: row of new matrix
            // j: column of new matrix
            // k: product index
            for (unsigned int j = 0; j < other_columns; j++) {
                float_t &comp = result.component(i,j);
                comp = 0;
                for (unsigned int k = 0; k < columns; k++) {
                    comp += component(i,k) * mat.component(k,j);
                }
            }
        }
        return result;
    }

    template <typename other_float_t>
    inline Matrix& operator*=(other_float_t scale)
    {
        for (unsigned int i = 0; i < rows*columns; i++) {
            coeff[i] *= scale;
        }
        return *this;
    }

    template <
        typename other_float_t,
        typename out_float_t = decltype(coeff[0] * other_float_t(0))>
    inline Matrix<out_float_t, rows, columns> operator*(
        other_float_t scale) const
    {
        Matrix<out_float_t, rows, columns> result(*this);
        result *= scale;
        return result;
    }

    template <typename other_float_t>
    inline Matrix& operator/=(other_float_t scale)
    {
        for (unsigned int i = 0; i < rows*columns; i++) {
            coeff[i] /= scale;
        }
        return *this;
    }

    template <
        typename other_float_t,
        typename out_float_t = decltype(coeff[0] / other_float_t(1))>
    inline Matrix<out_float_t, rows, columns> operator/(
        other_float_t scale) const
    {
        Matrix<out_float_t, rows, columns> result(*this);
        result /= scale;
        return result;
    }

    template <typename other_float_t>
    inline Matrix& operator+=(
        const Matrix<other_float_t, rows, columns> &mat)
    {
        for (unsigned int i = 0; i < rows*columns; i++) {
            coeff[i] += mat.coeff[i];
        }
        return *this;
    }

    template <
        typename other_float_t,
        typename out_float_t = decltype(coeff[0] + other_float_t(1))>
    inline Matrix<out_float_t, rows, columns> operator+(
        const Matrix<other_float_t, rows, columns> &mat) const
    {
        Matrix<out_float_t, rows, columns> result(*this);
        result += mat;
        return result;
    }

    template <typename other_float_t>
    inline Matrix& operator-=(
        const Matrix<other_float_t, rows, columns> &mat)
    {
        for (unsigned int i = 0; i < rows*columns; i++) {
            coeff[i] -= mat.coeff[i];
        }
        return *this;
    }

    template <
        typename other_float_t,
        typename out_float_t = decltype(coeff[0] - other_float_t(1))>
    inline Matrix<out_float_t, rows, columns> operator-(
        const Matrix<other_float_t, rows, columns> &mat) const
    {
        Matrix<out_float_t, rows, columns> result(*this);
        result -= mat;
        return result;
    }

    inline Matrix operator-() const
    {
        Matrix result;
        for (unsigned int i = 0; i < rows*columns; i++) {
            result.coeff[i] = -coeff[i];
        }
        return result;
    }

    inline Matrix operator+() const
    {
        return Matrix(*this);
    }

    template <
        typename other_float_t,
        typename out_float_t = decltype(coeff[0] * other_float_t(1))>
    inline Vector<out_float_t, rows> operator*(
        const Vector<other_float_t, columns> &vec) const
    {
        Vector<out_float_t, rows> result(0, 0, 0);
        for (unsigned int i = 0; i < rows; i++) {
            for (unsigned int j = 0; j < columns; j++) {
                result[i] += component(i,j) * vec[j];
            }
        }
        return result;
    }

    template <typename other_float_t>
    inline bool operator==(
        const Matrix<other_float_t, rows, columns> &oth)
    {
        for (unsigned int i = 0; i < rows*columns; i++) {
            if (coeff[i] != oth.coeff[i]) {
                return false;
            }
        }
        return true;
    }

    template <typename other_float_t>
    inline bool operator!=(
        const Matrix<other_float_t, rows, columns> &oth)
    {
        return !(*this == oth);
    }

    template <
        typename other_float_t,
        unsigned int other_rows,
        unsigned int other_columns>
    static inline Matrix extend(
        const Matrix<other_float_t, other_rows, other_columns> &mat)
    {
        static_assert(other_rows <= rows,
                      "Source matrix must have equal or less rows than "
                      "destination matrix.");
        static_assert(other_columns <= columns,
                      "Source matrix must have equal or less columns "
                      "than destination matrix.");

        Matrix result;
        for (unsigned int i = 0; i < other_rows; i++) {
            for (unsigned int j = 0; j < other_columns; j++) {
                result.component(i, j) = mat.component(i, j);
            }
        }

        return result;
    }

    template <
        typename other_float_t,
        unsigned int other_rows,
        unsigned int other_columns>
    static inline Matrix extend_with_identity(
        const Matrix<other_float_t, other_rows, other_columns> &mat)
    {
        static_assert(other_rows <= rows,
                      "Source matrix must have equal or less rows than "
                      "destination matrix.");
        static_assert(other_columns <= columns,
                      "Source matrix must have equal or less columns "
                      "than destination matrix.");
        static_assert(rows == columns,
                      "Extending with identity only makes sense for "
                      "square destination matrices.");

        Matrix result;
        for (unsigned int i = 0; i < other_rows; i++) {
            for (unsigned int j = 0; j < other_columns; j++) {
                result.component(i, j) = mat.component(i, j);
            }
        }
        for (unsigned int i = other_rows; i < rows; i++) {
            for (unsigned int j = 0; j < columns; j++) {
                result.component(i, j) = (i == j ? 1 : 0);
            }
        }
        for (unsigned int j = other_columns; j < columns; j++) {
            for (unsigned int i = 0; i < other_rows; i++) {
                result.component(i, j) = (i == j ? 1 : 0);
            }
        }

        return result;
    }

    float_t abssum() const
    {
        float_t summed = 0;
        for (unsigned int i = 0; i < rows*columns; i++) {
            summed += abs(coeff[i]);
        }
        return summed;
    }
};

typedef Matrix<float, 3, 3> Matrix3f;
typedef Matrix<float, 4, 4> Matrix4f;

typedef Matrix<VectorFloat, 3, 3> Matrix3;
typedef Matrix<VectorFloat, 4, 4> Matrix4;

Matrix3 rotation3(const Vector3 &axis, const VectorFloat alpha);
Matrix3 rotation3(const vector_component_x_t, const VectorFloat alpha);
Matrix3 rotation3(const vector_component_y_t, const VectorFloat alpha);
Matrix3 rotation3(const vector_component_z_t, const VectorFloat alpha);

Matrix4 rotation4(const Vector3 &axis, const VectorFloat alpha);
Matrix4 rotation4(const vector_component_x_t, const VectorFloat alpha);
Matrix4 rotation4(const vector_component_y_t, const VectorFloat alpha);
Matrix4 rotation4(const vector_component_z_t, const VectorFloat alpha);

Matrix4 translation4(const Vector3 &by);
Matrix4 scale4(const Vector3 &factors);

template <
    typename scale_float_t,
    typename float_t,
    unsigned int rows,
    unsigned int columns>
inline Matrix3 operator*(
    const scale_float_t scale,
    const Matrix<float_t, rows, columns> &mat)
{
    return mat*scale;
}

namespace {

template <typename float_t, unsigned int rows, unsigned int columns>
inline void matrix_data_to_stream(
    std::ostream &stream,
    const Matrix<float_t, rows, columns> &mat)
{
    stream << "(";
    for (unsigned int i = 0; i < rows; i++) {
        if (i > 0) {
            stream << "; ";
        }
        stream << "[" << mat.component(i, 0);
        for (unsigned int j = 1; j < columns; j++) {
            stream << ", " << mat.component(i, j);
        }
        stream << "]";
    }
    stream << ")";
}

template <typename float_t, unsigned int rows, unsigned int columns>
inline void matrix_dim_to_stream(
    std::ostream &stream,
    const Matrix<float_t, rows, columns>&)
{
    if (rows == columns) {
        stream << rows;
    } else {
        stream << rows << "×" << columns;
    }
}


template <typename float_t, unsigned int rows, unsigned int columns>
struct matrix_to_stream;

template <unsigned int rows, unsigned int columns>
struct matrix_to_stream<float, rows, columns>
{
    static inline std::ostream& put(
        std::ostream &stream,
        const Matrix<float, rows, columns> &mat)
    {
        stream << "mat";
        matrix_dim_to_stream(stream, mat);
        stream << "f";
        matrix_data_to_stream(stream, mat);
        return stream;
    }
};

template <unsigned int rows, unsigned int columns>
struct matrix_to_stream<double, rows, columns>
{
    static inline std::ostream& put(
        std::ostream &stream,
        const Matrix<double, rows, columns> &mat)
    {
        stream << "mat";
        matrix_dim_to_stream(stream, mat);
        stream << "d";
        matrix_data_to_stream(stream, mat);
        return stream;
    }
};

}

template <typename float_t, unsigned int rows, unsigned int columns>
std::ostream& operator<<(
    std::ostream &stream,
    const Matrix<float_t, rows, columns> &mat)
{
    return matrix_to_stream<float_t, rows, columns>::put(stream, mat);
}

}

#endif
