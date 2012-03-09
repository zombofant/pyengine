
#ifndef PYUNI_MATH_MATRIX_TEMPLATES_H
#define PYUNI_MATH_MATRIX_TEMPLATES_H

#include "Vectors.hpp"
#include "Matrices.hpp"
#include <iostream>

namespace PyUni {

/**
 * Write a column-wise n-dimensional identity matrix to the memory 
 * pointed to by result.
 * 
 * @param result    pointer to the memory where the resulting matrix 
 *                  will be written to.
 */
template <int n>
static inline void matrixIdentity(VectorFloat result[n*n]) {
    for (int i=0; i < n; i++) {
        for (int j=0; j < n; j++) {
            if (i == j)
                result[i + j*n] = 1.;
            else
                result[i + j*n] = 0.;
        }
    }
}

/**
 * Implements a generic matrix multiplication of two square matrices
 * with dimension dim. 
 * 
 * @param a         Pointer to the column-wise coefficients of the left 
 *                  operand.
 * @param b         Pointer to the column-wise coefficients of the right 
 *                  operand.
 * @param result    Pointer to a space where the column-wise 
 *                  coefficients of the resulting matrix will be written
 *                  to. This must be at least sqr(dim) in size.
 */
template <int dim> 
static inline void matrixMult(const VectorFloat a[dim*dim], 
    const VectorFloat b[dim*dim], 
    VectorFloat result[dim*dim]) 
{
    for (int i=0; i < dim; i++) {
        for (int j=0; j < dim; j++) {
            VectorFloat sum = 0;
            for (int k=0; k < dim; k++) {
                sum += a[i + k*dim] * b[k + j*dim];
            }
            result[i + j*dim] = sum;
        }
    }
}

/**
 * Implements a generic matrix-vector multiplication of a square matrix
 * with dimension dim and a vector with dimension dim.
 */
template <int dim>
static inline void matrixVectorMult(const VectorFloat matrix[dim*dim], 
    const VectorFloat vec[dim], 
    VectorFloat result[dim]) 
{
    // r_i = m_ij * v_j
    for (int i=0; i < dim; i++) {
        VectorFloat sum = 0.;
        for (int j=0; j < dim; j++) {
            sum += matrix[i + j*dim] * vec[j];
        }
        result[i] = sum;
    }
}

template <int dim>
static inline void matrixScalarMult(const VectorFloat matrix[dim*dim], 
    const VectorFloat factor, 
    VectorFloat result[dim*dim]) 
{
    for (int i=0; i < dim*dim; i++) {
        result[i] = matrix[i] * factor;
    }
}

template <int dim>
static inline void matrixDump(const VectorFloat matrix[dim*dim]) {
    for (int i=0; i < dim; i++) {
        std::cout << "[";
        for (int j=0; j < dim; j++) {
            std::cout << " " << matrix[i + j*dim] << " ";
        }
        std::cout << "]" << std::endl;
    }
}

}

#endif
