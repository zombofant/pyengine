/**********************************************************************
File name: Matrices.hpp
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

#ifndef _PYUNI_MATH_MATRICES_H
#define _PYUNI_MATH_MATRICES_H

#include "Vectors.hpp"

namespace PyUni {

struct Matrix3f {
    float coeff[9];
};

struct Matrix3 {
    double coeff[9];
    
    Matrix3();
    Matrix3(const Matrix3f &mat);
    
    Matrix3f toMatrix3f();
    
    static Matrix3 Rotation(const Vector3 axis, const VectorFloat angle);
};

struct Matrix4f {
    float coeff[16];
};

struct Matrix4 {
    double coeff[16];
    
    Matrix4();
    Matrix4(const Matrix4f &mat);
    
    Matrix4f toMatrix4f();
    Matrix3 extractRotation() const;
    
    void dump();
    
    static Matrix4 Rotation(const Vector3 &axis, const VectorFloat angle);
    static Matrix4 Rotation(const Matrix3 &m3);
    static Matrix4 RotationX(const VectorFloat angle);
    static Matrix4 RotationY(const VectorFloat angle);
    static Matrix4 RotationZ(const VectorFloat angle);
    static Matrix4 Translation(const Vector3 &by);
    static Matrix4 Scale(const Vector3 &by);
};

Vector3 operator* (Matrix3 a, Vector3 b);
Matrix3 operator* (Matrix3 a, Matrix3 b);
Matrix3 operator* (Matrix3 a, VectorFloat b);

Vector4 operator* (Matrix4 a, Vector4 b);
Matrix4 operator* (Matrix4 a, Matrix4 b);
Matrix4 operator* (Matrix4 a, VectorFloat b);
    
}

#endif
