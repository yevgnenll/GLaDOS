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
        Deg angle = Deg{60};
        Quat q1 = Quat::angleAxis(Rad{angle}, norm);
        REQUIRE(q1 == Quat{0.8660254, 0, 0.3535534, 0.3535534});

        UVec3 norm2 = Vec3{1,0,0}.makeNormalize();
        Quat q2 = Quat::angleAxis(Rad{Deg{180}}, norm2);
        REQUIRE(q2 == Quat{0, 1, 0, 0});

        // same but radian (180 degree == pi)
        Quat q3 = Quat::angleAxis(Rad{Math::pi}, norm2);
        REQUIRE(q3 == Quat{0, 1, 0, 0});

        Mat4<real> rot = Quat::toRotationMat(q2);
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
    }
}