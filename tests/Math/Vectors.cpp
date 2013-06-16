/**********************************************************************
File name: Vectors.cpp
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
#include <catch.hpp>

#include <CEngine/Math/Vectors.hpp>

using namespace PyEngine;

TEST_CASE("Math/Vectors/Vector2/operations"
          "Test vector2 operations")
{
    Vector2 ex(1, 0);
    Vector2 ey(0, 1);
    Vector2 a(1, 1);

    const double anormcomp = 1./sqrt(2);

    CHECK(ex.length() == 1);
    CHECK(ey.length() == 1);
    CHECK(a.length() == sqrt(2));

    CHECK(a.normalized() == Vector2(anormcomp, anormcomp));

    CHECK((ex + ex) == Vector2(2, 0));
    CHECK((ex*ex) == 1);
    CHECK((ex*a) == 1);
    CHECK((ey*ex) == 0);
    CHECK((2*ex) == Vector2(2, 0));
    CHECK((2*a) == Vector2(2, 2));
    CHECK((ex-a) == Vector2(0, -1));

    CHECK(&a.x == &a.u);
    CHECK(&a.y == &a.v);
    CHECK(&a.as_array[0] == &a.x);
    CHECK(&a.as_array[1] == &a.y);
}

TEST_CASE("Math/Vectors/Vector3/operations"
          "Test vector3 operations")
{
    Vector3 ex(1, 0, 0);
    Vector3 ey(0, 1, 0);
    Vector3 ez(0, 0, 1);
    Vector3 a(1, 1, 1);

    const double anormcomp = 1./sqrt(3);

    CHECK(ex.length() == 1);
    CHECK(ey.length() == 1);
    CHECK(ez.length() == 1);
    CHECK(a.length() == sqrt(3));

    CHECK(a.normalized() == Vector3(anormcomp, anormcomp, anormcomp));

    CHECK(a.vec2() == Vector2(1, 1));

    CHECK((ex + ey + ez) == a);
    CHECK((ex*ex) == 1);
    CHECK((ex*ey) == 0);
    CHECK((ex*ez) == 0);
    CHECK((ex*a) == 1);
    CHECK((ex-a) == Vector3(0, -1, -1));

    CHECK(&a.x == &a.r);
    CHECK(&a.y == &a.g);
    CHECK(&a.z == &a.b);
    CHECK(&a.as_array[0] == &a.x);
    CHECK(&a.as_array[1] == &a.y);
    CHECK(&a.as_array[2] == &a.z);

    CHECK((ex%ey) == ez);
}

TEST_CASE("Math/Vectors/Vector4/operations"
          "Test vector4 operations")
{
    Vector4 ex(1, 0, 0, 0);
    Vector4 ey(0, 1, 0, 0);
    Vector4 ez(0, 0, 1, 0);
    Vector4 ew(0, 0, 0, 1);
    Vector4 a(1, 1, 1, 1);


    const double anormcomp = 1./2.;


    CHECK(ex.length() == 1);
    CHECK(ey.length() == 1);
    CHECK(ez.length() == 1);
    CHECK(ew.length() == 1);
    CHECK(a.length() == 2);

    CHECK(a.normalized() == Vector4(anormcomp, anormcomp, anormcomp, anormcomp));


    CHECK((ex + ex) == Vector4(2, 0, 0, 0));
    CHECK((ex*ex) == 1);
    CHECK((ex*a) == 1);
    CHECK((ey*ex) == 0);
    CHECK((2*ex) == Vector4(2, 0, 0, 0));
    CHECK((2*a) == Vector4(2, 2, 2, 2));
    CHECK((ex-a) == Vector4(0, -1, -1, -1));

    CHECK(&a.x == &a.r);
    CHECK(&a.y == &a.g);
    CHECK(&a.z == &a.b);
    CHECK(&a.w == &a.a);
    CHECK(&a.as_array[0] == &a.x);
    CHECK(&a.as_array[1] == &a.y);
    CHECK(&a.as_array[2] == &a.z);
    CHECK(&a.as_array[3] == &a.w);
}
