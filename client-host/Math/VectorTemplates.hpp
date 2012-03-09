
#ifndef PYUNI_MATH_VECTOR_TEMPLATES_H
#define PYUNI_MATH_VECTOR_TEMPLATES_H

#include "Vectors.hpp"
#include <iostream>

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

