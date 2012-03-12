/**********************************************************************
File name: Vectors.hpp
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

#ifndef PYUNI_MATH_VECTORS_H
#define PYUNI_MATH_VECTORS_H

#include <vector>
#include <ostream>

namespace PyUni {
    
typedef double VectorFloat;

struct Vector2f {
    static const unsigned int dimension = 2;
    union {
        struct {
            float x, y;
        };
        float as_array[2];
    };
};

struct Vector3f {
    static const unsigned int dimension = 3;
    union {
        struct {
            float x, y, z;
        };
        float as_array[3];
    };
};

struct Vector4f {
    static const unsigned int dimension = 4;
    union {
        struct {
            float x, y, z, w;
        };
        float as_array[4];
    };
};
    
struct Vector2 {
    public:
        union {
            struct {
                union {
                    double x;
                    double u;
                };
                union {
                    double y;
                    double v;
                };
            };
            double as_array[2];
        };
        
        Vector2();
        Vector2(const Vector2 &vec2);
        Vector2(const VectorFloat X, const VectorFloat Y);
        Vector2(const Vector2f &vec2f);
        Vector2(const VectorFloat angle);
        
        VectorFloat length() const;
        Vector2 normalized() const;
        void normalize();
        Vector2f toVector2f() const;
        
        void dump() const;
        
        Vector2 &operator+= (const Vector2 b);
        Vector2 &operator/= (const VectorFloat b);
        Vector2 &operator*= (const VectorFloat b);
};

struct Vector3 {
    public:
        union {
            struct {
                union {
                    double x;
                    double r;
                };
                union {
                    double y;
                    double g;
                };
                union {
                    double z;
                    double b;
                };
            };
            double as_array[3];
        };
        
        Vector3();
        Vector3(double X, double Y, double Z);
        Vector3(Vector2 vec2, double Z = 0.);
        Vector3(Vector3f vec3f);
        
        Vector2 vec2() const;
        
        VectorFloat length() const;
        Vector3 normalized() const;
        void normalize();
        Vector3f toVector3f() const;
        
        void dump() const;
        
        Vector3 &operator+= (const Vector3 b);
        Vector3 &operator/= (const VectorFloat b);
};

typedef std::vector<Vector3*> Vector3List;

struct Vector4 {
    public:
        union {
            struct {
                union {
                    double x;
                    double r;
                };
                union {
                    double y;
                    double g;
                };
                union {
                    double z;
                    double b;
                };
                union {
                    double w;
                    double a;
                };
            };
            double as_array[4];
        };
        
        Vector4();
        Vector4(double X, double Y, double Z, double W);
        Vector4(Vector2 vec2, double Z = 0., double W = 0.);
        Vector4(Vector3 vec3, double W = 0.);
        Vector4(Vector4f vec4f);
        
        Vector2 vec2() const;
        Vector3 vec3() const;
        
        VectorFloat length() const;
        Vector4 normalized() const;
        void normalize();
        Vector4f toVector4f() const;
        
        void dump() const;
};

Vector2 operator+ (Vector2 a, Vector2 b);
Vector2 operator+ (Vector2 a, VectorFloat b);
Vector2 operator+ (VectorFloat a, Vector2 b);

Vector2 operator- (Vector2 a);
Vector2 operator- (Vector2 a, Vector2 b);
Vector2 operator- (Vector2 a, VectorFloat b);

VectorFloat operator* (Vector2 a, Vector2 b);
Vector2 operator* (Vector2 a, VectorFloat b);

Vector2 operator/ (Vector2 a, VectorFloat b);

bool operator== (Vector2 a, Vector2 b);


Vector3 operator+ (Vector3 a, Vector3 b);
Vector3 operator+ (Vector3 a, VectorFloat b);
Vector3 operator+ (VectorFloat a, Vector3 b);

Vector3 operator- (Vector3 a);
Vector3 operator- (Vector3 a, Vector3 b);
Vector3 operator- (Vector3 a, VectorFloat b);

VectorFloat operator* (Vector3 a, Vector3 b);
Vector3 operator* (Vector3 a, VectorFloat b);

Vector3 operator% (Vector3 a, Vector3 b);

Vector3 operator/ (Vector3 a, VectorFloat b);

bool operator== (Vector3 a, Vector3 b);


Vector4 operator+ (Vector4 a, Vector4 b);
Vector4 operator+ (Vector4 a, VectorFloat b);
Vector4 operator+ (VectorFloat a, Vector4 b);

Vector4 operator- (Vector4 a);
Vector4 operator- (Vector4 a, Vector4 b);
Vector4 operator- (Vector4 a, VectorFloat b);

VectorFloat operator* (Vector4 a, Vector4 b);
Vector4 operator* (Vector4 a, VectorFloat b);

Vector4 operator/ (Vector4 a, VectorFloat b);

bool operator== (Vector4 a, Vector4 b);


std::ostream &operator<< (std::ostream &stream, const Vector2 &v);
std::ostream &operator<< (std::ostream &stream, const Vector2f &v);
std::ostream &operator<< (std::ostream &stream, const Vector3 &v);
std::ostream &operator<< (std::ostream &stream, const Vector3f &v);
std::ostream &operator<< (std::ostream &stream, const Vector4 &v);
std::ostream &operator<< (std::ostream &stream, const Vector4f &v);

}

#endif
