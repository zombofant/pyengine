/**********************************************************************
File name: Matrices.cpp
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
#include <cmath>
#include <cstring>

#include "Matrices.hpp"

namespace PyEngine {

template <typename num_t>
inline num_t sqr(const num_t a)
{
    return a*a;
}

Matrix3 rotation3(const Vector3 &rawAxis, const VectorFloat alpha) {
    const VectorFloat saveSin = sin(alpha);
    const VectorFloat saveCos = cos(alpha);
    const VectorFloat oneMinusCos = 1-saveCos;
    const VectorFloat len = rawAxis.length();

    if (len == 0.) {
        return Matrix3();
    }

    Matrix3 result;
    const Vector3 axis = rawAxis / len;

    result.coeff[0] = (oneMinusCos * sqr(axis[eX])) + saveCos;
    result.coeff[1] = (oneMinusCos * axis[eX] * axis[eY]) - (axis[eZ] * saveSin);
    result.coeff[2] = (oneMinusCos * axis[eZ] * axis[eX]) + (axis[eY] * saveSin);

    result.coeff[3] = (oneMinusCos * axis[eX] * axis[eY]) + (axis[eZ] * saveSin);
    result.coeff[4] = (oneMinusCos * sqr(axis[eY])) + saveCos;
    result.coeff[5] = (oneMinusCos * axis[eY] * axis[eZ]) - (axis[eX] * saveSin);

    result.coeff[6] = (oneMinusCos * axis[eZ] * axis[eX]) - (axis[eY] * saveSin);
    result.coeff[7] = (oneMinusCos * axis[eY] * axis[eZ]) + (axis[eX] * saveSin);
    result.coeff[8] = (oneMinusCos * sqr(axis[eZ])) + saveCos;

    return result;
}

Matrix3 rotation3(const vector_component_x_t, const VectorFloat alpha) {
    VectorFloat sin_alpha = sin(alpha);
    VectorFloat cos_alpha = cos(alpha);
    return Matrix3(
        1,          0,           0,
        0,  cos_alpha,  -sin_alpha,
        0,  sin_alpha,   cos_alpha
    );
}

Matrix3 rotation3(const vector_component_y_t, const VectorFloat alpha) {
    VectorFloat sin_alpha = sin(alpha);
    VectorFloat cos_alpha = cos(alpha);
    return Matrix3(
        cos_alpha,  0,  sin_alpha,
                0,  1,          0,
       -sin_alpha,  0,  cos_alpha
    );
}

Matrix3 rotation3(const vector_component_z_t, const VectorFloat alpha) {
    VectorFloat sin_alpha = sin(alpha);
    VectorFloat cos_alpha = cos(alpha);
    return Matrix3(
        cos_alpha,  -sin_alpha, 0,
        sin_alpha,   cos_alpha, 0,
                0,           0, 1
    );
}

Matrix4 rotation4(const Vector3 &axis, const VectorFloat alpha) {
    return Matrix4::extend_with_identity(rotation3(axis, alpha));
};

Matrix4 rotation4(const vector_component_x_t, const VectorFloat alpha) {
    return Matrix4::extend_with_identity(rotation3(eX, alpha));
};

Matrix4 rotation4(const vector_component_y_t, const VectorFloat alpha) {
    return Matrix4::extend_with_identity(rotation3(eY, alpha));
};

Matrix4 rotation4(const vector_component_z_t, const VectorFloat alpha) {
    return Matrix4::extend_with_identity(rotation3(eZ, alpha));
};

Matrix4 translation4(const Vector3 &by) {
    Matrix4 result(Identity);
    for (unsigned int i = 0; i < 3; i++) {
        result.component(i, 3) = by[i];
    }
    return result;
}

Matrix4 scale4(const Vector3 &factors) {
    Matrix4 result(Identity);
    for (unsigned int j = 0; j < 3; j++) {
        result.component(3, j) = factors[j];
    }
    return result;
}

}
