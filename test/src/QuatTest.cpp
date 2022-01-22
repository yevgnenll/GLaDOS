#include <catch2/catch.hpp>

#include "math/Quat.h"
#include "math/UVec3.h"
#include "math/Vec3.h"
#include "math/Mat4.hpp"
#include "math/Math.h"

using namespace GLaDOS;

TEST_CASE("Quaternion unit tests", "[Quaternion]") {
    SECTION("Quat init & copy") {
        Quat q1;
        REQUIRE(Quat::identity == q1);
        q1.w = 2;
        q1.vector = Vec3{1,2,3};
        Quat q2 = q1;
        REQUIRE(q2.scalar == 2);
        REQUIRE(q2.vector == Vec3{1,2,3});
    }

    SECTION("Quat AngleAxis") {
        UVec3 norm = Vec3{0, 1, 1}.makeNormalize();
        Quat q1 = Quat::angleAxis(Deg{60}, norm);
        REQUIRE(q1 == Quat{0.8660254, 0, 0.3535534, 0.3535534});

        UVec3 norm2 = Vec3{1,0,0}.makeNormalize();
        Quat q2 = Quat::angleAxis(Deg{180}, norm2);
        REQUIRE(q2 == Quat{0, 1, 0, 0});

        Mat4<real> rot = Mat4<real>::rotate(q2);
        Mat4<real> rotResult = {1,0,0,0,
                                0,-1,0,0,
                                0,0,-1,0,
                                0,0,0,1};
        REQUIRE(rot == rotResult);
    }

    SECTION("Quat conjugate") {
        Quat q1{1,2,3,4};
        Quat q2 = Quat::conjugate(q1);
        REQUIRE(q2 == Quat{1, -2, -3, -4});

        q1.makeConjugate();
        REQUIRE(q1 == Quat{1, -2, -3, -4});

        // (pq)* == q*p*
        Quat q3 = {1,2,3,4};
        Quat q4 = {5,6,7,8};
        REQUIRE(Quat::conjugate(q3 * q4) == Quat::conjugate(q4) * Quat::conjugate(q3));

        // (p + q)* == q* + p*
        REQUIRE(Quat::conjugate(q3 + q4) == Quat::conjugate(q3) + Quat::conjugate(q4));

        // (q*)* == q
        REQUIRE(Quat::conjugate(Quat::conjugate(q3)) == q3);

        // (sq)* == sq* (if s in Real number)
        REQUIRE(Quat::conjugate(q3 * 3) == Quat::conjugate(q3) * 3);

        // q + q* == 2w
        REQUIRE((q3 + Quat::conjugate(q3)).scalar == 2 * q3.scalar);

        // qq* == q*q
        REQUIRE(q3 * Quat::conjugate(q3) == Quat::conjugate(q3) * q3);
    }

    SECTION("Quat length & normalize") {
        Quat q1{1,2,3,4};
        real len = q1.length();
        REQUIRE(Math::equal(len, 5.47722557f));

        // sqrt(qq*) == ||q||
        Quat qmul = q1 * Quat::conjugate(q1);
        REQUIRE(Math::equal(len, Math::sqrt(qmul.scalar)));

        // sqrt(q*q) == ||q||
        Quat qmul2 = Quat::conjugate(q1) * q1;
        REQUIRE(Math::equal(len, Math::sqrt(qmul2.scalar)));

        Quat qnorm = Quat::normalize(q1);
        REQUIRE(qnorm == q1 / len);
    }

    SECTION("Quat inverse") {
        // q-1 == q*/||q||^2
        Quat q1 = {1,2,3,4};
        Quat qinv = Quat::inverse(q1);
        REQUIRE(qinv == Quat::conjugate(q1) / (q1.length() * q1.length()));

        // (q-1)-1 == q
        // compensate floating point precision
        Quat qiinv = Quat::inverse(qinv);
        qiinv.w = Math::round(qiinv.w);
        qiinv.x = Math::round(qiinv.x);
        qiinv.y = Math::round(qiinv.y);
        qiinv.z = Math::round(qiinv.z);
        REQUIRE(qiinv == q1);

        // (pq)-1 == q-1p-1
        Quat q2 = {5,6,7,8};
        REQUIRE(Quat::inverse(q1 * q2) == Quat::inverse(q2) * Quat::inverse(q1));

        // qq-1 == (1,0,0,0)
        Quat qq = q1 * Quat::inverse(q1);
        REQUIRE(qq == Quat{1, 0, 0, 0});

        // q-1q == (1,0,0,0)
        Quat qq2 = Quat::inverse(q1) * q1;
        REQUIRE(qq2 == Quat{1, 0, 0, 0});
    }

    SECTION("Quat from to euler") {
        Quat q = {3, 6, 2, 1};
        Vec3 eulerAngle = Quat::toEuler(q);
        REQUIRE(eulerAngle == Vec3{126.8698983, 0, 36.8698983});

        Vec3 euler = {Deg{12}.get(), Deg{30}.get(), Deg{8}.get()};
        Quat q2 = Quat::fromEuler(euler);
        REQUIRE(q2 == Quat{0.96018159, 0.08276540, 0.26381728, 0.04002241});
    }

    SECTION("Quat cross & dot product") {
        Quat q1 = {1, 2, 3, 4};
        Quat q2 = {5, 6, 7, 8};
        real d = Quat::dot(q1, q2);
        REQUIRE(d == 70.f);
    }

    SECTION("Quat multiplication with Vector") {
        Vec3 v1 = {-1, -1, 0};
        Vec3 v2 = {1, -1, 0};
        Vec3 v3 = {0, 1, 0};
        Quat q = Quat::angleAxis(Deg{5}, UVec3::backward);
        REQUIRE(q == Quat{0.99904820, 0, 0, 0.04361939});

        Vec3 r1 = q * v1;
        REQUIRE(r1 == Vec3{-0.90903896, -1.08335042, 0});

        Vec3 r2 = q * v2;
        REQUIRE(r2 == Vec3{1.08335042, -0.90903896, 0});

        Vec3 r3 = q * v3;
        REQUIRE(r3 == Vec3{-0.08715575, 0.99619470, 0});
    }

    SECTION("Quat from & to rotation matrix") {
        Quat q1 = Quat::fromEuler(Vec3{180, 90, 60});
        REQUIRE(q1 == Quat{0.35355338, 0.61237239, 0.35355335, -0.61237245});

        // TODO: from quat to mat4
        Mat4<real> m = Mat4<real>::rotate(q1);
        Mat4<real> result = {
            0,  0.8660254, -0.5, 0,
            0, -0.5, -0.8660254, 0,
            -1,  0, 0, 0,
            0, 0, 0, 1
        };
        REQUIRE(m == result);

        // TODO: from mat4 to quat
        Quat q2 = Quat::fromRotation(result);
    }

    SECTION("Angle between two Quaternion") {
        Deg anlge = Deg{60};
        Quat q1 = {1, 0, 0, 0};
        Quat q2 = Quat::angleAxis(anlge, UVec3::right);
        Deg deg = Quat::angleBetween(q1, q2);
        REQUIRE(Math::equal(deg.get(), anlge.get()));
    }

    SECTION("Quat linear interpolation & spherical linear interpolation") {

    }
}