/**********************************************************************
File name: Matrices.cpp
This file is part of: Pythonic Universe

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

For feedback and questions about pyuni please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/
#include <cmath>
#include <cstring>

#include "Matrices.hpp"
#include "MatrixTemplates.hpp"

namespace PyUni {
    
static inline VectorFloat sqr(VectorFloat a) {
    return a * a;
}

Matrix3::Matrix3() {
    matrixIdentity<3>(coeff);
}

Matrix3::Matrix3(const Matrix3f &mat) {
    for (int i=0; i<9; i++) {
        coeff[i] = mat.coeff[i];
    }
}

Matrix3f Matrix3::toMatrix3f() {
    Matrix3f result;
    for (int i=0; i<9; i++) {
        result.coeff[i] = coeff[i];
    }
    return result;
}

Matrix3 Matrix3::Rotation(const Vector3 rawAxis, const VectorFloat angle) {
    const VectorFloat saveSin = sin(angle);
    const VectorFloat saveCos = cos(angle);
    const VectorFloat oneMinusCos = 1-saveCos;
    const VectorFloat len = rawAxis.length();
    
    if (len == 0.) {
        return Matrix3();
    }
    
    Matrix3 result;
    const Vector3 axis = rawAxis / len;
    
    result.coeff[0] = (oneMinusCos * sqr(axis.x)) + saveCos;
    result.coeff[1] = (oneMinusCos * axis.x * axis.y) - (axis.z * saveSin);
    result.coeff[2] = (oneMinusCos * axis.z * axis.x) + (axis.y * saveSin);

    result.coeff[3] = (oneMinusCos * axis.x * axis.y) + (axis.z * saveSin);
    result.coeff[4] = (oneMinusCos * sqr(axis.y)) + saveCos;
    result.coeff[5] = (oneMinusCos * axis.y * axis.z) - (axis.x * saveSin);

    result.coeff[6] = (oneMinusCos * axis.z * axis.x) - (axis.y * saveSin);
    result.coeff[7] = (oneMinusCos * axis.y * axis.z) + (axis.x * saveSin);
    result.coeff[8] = (oneMinusCos * sqr(axis.z)) + saveCos;
    
    return result;
}

Matrix4::Matrix4() {
    matrixIdentity<4>(coeff);
}

Matrix4::Matrix4(const Matrix4f &mat) {
    for (int i=0; i<16; i++) {
        coeff[i] = mat.coeff[i];
    }
}

Matrix4f Matrix4::toMatrix4f() {
    Matrix4f result;
    for (int i=0; i<16; i++) {
        result.coeff[i] = coeff[i];
    }
    return result;
}

Matrix3 Matrix4::extractRotation() const {
    Matrix3 result;
    for (int i = 0; i < 3; i++) {
        for (int k = 0; k < 3; k++) {
            result.coeff[i + k*3] = coeff[i + k*4];
        }
    }
    return result;
}

void Matrix4::dump() {
    matrixDump<4>(coeff);
}

Matrix4 Matrix4::Rotation(const Vector3 &axis, const VectorFloat angle) {
    return Matrix4::Rotation(Matrix3::Rotation(axis, angle));
}

Matrix4 Matrix4::Rotation(const Matrix3 &m3) {
    Matrix4 result;
    result.coeff[0] = m3.coeff[0];
    result.coeff[1] = m3.coeff[1];
    result.coeff[2] = m3.coeff[2];
    
    result.coeff[4] = m3.coeff[3];
    result.coeff[5] = m3.coeff[4];
    result.coeff[6] = m3.coeff[5];
    
    result.coeff[8] = m3.coeff[6];
    result.coeff[9] = m3.coeff[7];
    result.coeff[10] = m3.coeff[8];
    return result;
}

Matrix4 Matrix4::RotationX(const VectorFloat angle) {
    const VectorFloat Sin = sin(angle);
    const VectorFloat Cos = cos(angle);
    
    Matrix4 result;
    result.coeff[5] = Cos;
    result.coeff[6] = Sin;
    result.coeff[9] = -Sin;
    result.coeff[10] = Cos;
    return result;
}

Matrix4 Matrix4::RotationY(const VectorFloat angle) {
    const VectorFloat Sin = sin(angle);
    const VectorFloat Cos = cos(angle);
    
    Matrix4 result;
    result.coeff[0] = Cos;
    result.coeff[2] = -Sin;
    result.coeff[8] = Sin;
    result.coeff[10] = Cos;
    return result;
}

Matrix4 Matrix4::RotationZ(const VectorFloat angle) {
    const VectorFloat Sin = sin(angle);
    const VectorFloat Cos = cos(angle);
    
    Matrix4 result;
    result.coeff[0] = Cos;
    result.coeff[1] = Sin;
    result.coeff[4] = -Sin;
    result.coeff[5] = Cos;
    return result;
}

Matrix4 Matrix4::Translation(const Vector3 &by) {
    Matrix4 result;
    result.coeff[12] = by.x;
    result.coeff[13] = by.y;
    result.coeff[14] = by.z;
    return result;
}

Matrix4 Matrix4::Scale(const Vector3 &by) {
    Matrix4 result;
    result.coeff[0] = by.x;
    result.coeff[5] = by.y;
    result.coeff[10] = by.z;
    return result;
}

/* matrix operations */

Vector3 operator* (Matrix3 a, Vector3 b) {
    Vector3 result;
    matrixVectorMult<3>(a.coeff, b.as_array, result.as_array);
    return result;
}

Matrix3 operator* (Matrix3 a, Matrix3 b) {
    Matrix3 result;
    matrixMult<3>(a.coeff, b.coeff, result.coeff);
    return result;
}

Matrix3 operator* (Matrix3 a, VectorFloat b) {
    Matrix3 result;
    matrixScalarMult<3>(a.coeff, b, result.coeff);
    return result;
}


Vector4 operator* (Matrix4 a, Vector4 b) {
    Vector4 result;
    matrixVectorMult<3>(a.coeff, b.as_array, result.as_array);
    return result;
}

Matrix4 operator* (Matrix4 a, Matrix4 b) {
    Matrix4 result;
    matrixMult<4>(a.coeff, b.coeff, result.coeff);
    return result;
}

Matrix4 operator* (Matrix4 a, VectorFloat b) {
    Matrix4 result;
    matrixScalarMult<4>(a.coeff, b, result.coeff);
    return result;
}

}
