/**********************************************************************
File name: VectorTemplates.hpp
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
#ifndef _PYUNI_MATH_VECTOR_TEMPLATES_H
#define _PYUNI_MATH_VECTOR_TEMPLATES_H

#include <iostream>

#include "Vectors.hpp"

namespace PyUni {
    
template <int n>
static inline VectorFloat vectorLength(const VectorFloat vec[n]) {
    double sum = 0.;
    for (int i=0; i<n; i++) {
        sum += vec[i] * vec[i];
    }
    return sum;
}

template <int n>
static inline void vectorAdd(const VectorFloat a[n], 
    const VectorFloat b[n], 
    VectorFloat result[n]) 
{
    for (int i=0; i<n; i++) {
        result[i] = a[i] + b[i];
    }
}

template <int n>
static inline void vectorScalarAdd(const VectorFloat a[n], 
    const VectorFloat b, 
    VectorFloat result[n]) 
{
    for (int i=0; i<n; i++) {
        result[i] = a[i] + b;
    }
}

template <int n>
static inline void vectorSub(const VectorFloat a[n], 
    const VectorFloat b[n], 
    VectorFloat result[n]) 
{
    for (int i=0; i<n; i++) {
        result[i] = a[i] - b[i];
    }
}

template <int n>
static inline void vectorScalarSub(const VectorFloat a[n], 
    const VectorFloat b, 
    VectorFloat result[n]) 
{
    for (int i=0; i<n; i++) {
        result[i] = a[i] - b;
    }
}

template <int n>
static inline void vectorInvert(VectorFloat vec[n]) {
    for (int i=0; i<n; i++) {
        vec[i] = -vec[i];
    }
}

template <int n>
static inline VectorFloat vectorDotProduct(const VectorFloat a[n], 
    const VectorFloat b[n]) 
{
    double result = 0.;
    for (int i=0; i<n; i++) {
        result += a[i] * b[i];
    }
    return result;
}

template <int n>
static inline void vectorScalarMult(const VectorFloat a[n], 
    const VectorFloat b, 
    VectorFloat result[n]) 
{
    for (int i=0; i<n; i++) {
        result[i] = a[i] * b;
    }
}

template <int n>
static inline void vectorScalarDiv(const VectorFloat a[n], 
    const VectorFloat b, 
    VectorFloat result[n]) 
{
    for (int i=0; i<n; i++) {
        result[i] = a[i] / b;
    }
}

template <int n>
static inline void vectorNormalize(VectorFloat vec[n]) 
{
    const VectorFloat len = vectorLength<n>(vec);
    for (int i = 0; i < n; i++) {
        vec[i] /= len;
    }
}

template <int n>
static inline void vectorDump(const VectorFloat vec[n]) 
{
    std::cout << "(";
    for (int i = 0; i < n; i++) {
        std::cout << " " << vec[i] << " ";
    }
    std::cout << ")" << std::endl;
}

}

#endif

