/**********************************************************************
File name: MatrixTemplates.hpp
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
#ifndef _PYUNI_MATH_MATRIX_TEMPLATES_H
#define _PYUNI_MATH_MATRIX_TEMPLATES_H

#include <iostream>

#include "Vectors.hpp"
#include "Matrices.hpp"

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
