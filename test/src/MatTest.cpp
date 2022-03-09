#include <catch2/catch.hpp>

#include "math/Mat.hpp"

using namespace GLaDOS;

TEST_CASE("Generic Matrix unit tests", "[Generic Matrix]") {
    SECTION("default constructor test") {
        Mat<real, 4, 4> m44;
        REQUIRE(m44.isIdentity());
        REQUIRE(Mat<real, 4, 4>::identity() == m44);

        Mat<real, 3, 3> m33;
        REQUIRE(m33.isIdentity());
        REQUIRE(Mat<real, 3, 3>::identity() == m33);

        Mat<real, 2, 2> m22;
        REQUIRE(m22.isIdentity());
        REQUIRE(Mat<real, 2, 2>::identity() == m22);

        Mat<real, 2, 3> m23;
        REQUIRE(Mat<real, 2, 3>::zero() == m23);

        Mat<real, 3, 2> m32;
        REQUIRE(Mat<real, 3, 2>::zero() == m32);

        Mat<real, 4, 1> m41;
        REQUIRE(Mat<real, 4, 1>::zero() == m41);

        Mat<real, 1, 4> m14;
        REQUIRE(Mat<real, 1, 4>::zero() == m14);
    }

    SECTION("matrix multiplication test") {
        Mat<real, 4, 4> m1{
            2.f, 0.f, 0.f, 0.f,
            0.f, 2.f, 0.f, 0.f,
            0.f, 0.f, 2.f, 0.f,
            0.f, 0.f, 0.f, 2.f
        };
        Mat<real, 4, 4> m2{
            2.f, 0.f, 0.f, 0.f,
            0.f, 2.f, 0.f, 0.f,
            0.f, 0.f, 2.f, 0.f,
            0.f, 0.f, 0.f, 2.f
        };
        Mat<real, 4, 4> result{
            4.f, 0.f, 0.f, 0.f,
            0.f, 4.f, 0.f, 0.f,
            0.f, 0.f, 4.f, 0.f,
            0.f, 0.f, 0.f, 4.f
        };
        REQUIRE((m1 * m2) == result);

//        Mat<real, 2, 3> m3{1.f, 3.f, 2.f,
//                           4.f, 0.f, 1.f};
//        Mat<real, 3, 1> m4{1.f, 0.f, 5.f};
//        auto result2 = m3 * m4;
//        REQUIRE(result2.dimension() == 2);
//        REQUIRE(Math::equal(result2._m16[0], 11.f));
//        REQUIRE(Math::equal(result2._m16[1], 9.f));
    }

    SECTION("constructor with parameters test") {

    }

    SECTION("matrix copy test") {
    }

    SECTION("makeIdentity test") {
    }

    SECTION("makeTranspose test") {
    }

    SECTION("arithmetic with other matrix test") {
    }

    SECTION("equality test") {
    }

    SECTION("multiply with Vec4") {
    }

    SECTION("inverse of matrix test") {
    }

    SECTION("inverse of determinant of matrix test") {
    }

    SECTION("toMat3 test") {
    }

    SECTION("matrix abs test") {
    }

    SECTION("matrix perspective test") {
    }

    SECTION("matrix get col & row") {
    }

    SECTION("matrix trace") {
    }

    SECTION("matrix elementary row operations") {
    }
}