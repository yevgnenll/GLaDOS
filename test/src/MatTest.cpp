#include <catch2/catch.hpp>

#include "math/Mat.hpp"

using namespace GLaDOS;

TEST_CASE("Mat unit tests", "[Matrix]") {
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

    SECTION("constructor with parameters test") {
        Mat<real, 2, 2> m1{
            1.f, 1.f,
            1.f, 1.f
        };
        Mat<real, 2, 2> m2 = Mat<real, 2, 2>::from(1.f);
        REQUIRE(m1.dimension() == 4);
        REQUIRE(m2.dimension() == 4);
        REQUIRE(m1 == m2);

        Mat<real, 2, 2> m3 = Mat<real, 2, 2>::from(Vec<real, 2>{1.f, 0.f});
        Mat<real, 2, 2> m4{1.f, 0.f, 1.f, 0.f};
        REQUIRE(m3.dimension() == 4);
        REQUIRE(m4.dimension() == 4);
        REQUIRE(m3 == m4);
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

        Mat<real, 3, 3> m3{
            12.f, 8.f, 4.f,
            3.f, 17.f, 14.f,
            9.f, 8.f, 10.f
        };
        Mat<real, 3, 3> m4{
            5.f, 19.f, 3.f,
            6.f, 15.f, 9.f,
            7.f, 8.f, 16.f
        };
        Mat<real, 3, 3> result2{
            136.f, 380.f, 172.f,
            215.f, 424.f, 386.f,
            163.f, 371.f, 259.f
        };
        REQUIRE((m3 * m4) == result2);

        Mat<real, 2, 2> m5{
            1.f, 2.f,
            3.f, 4.f
        };
        Mat<real, 2, 2> m6{
            4.f, 3.f,
            2.f, 1.f
        };
        Mat<real, 2, 2> result3{
            8.f, 5.f,
            20.f, 13.f
        };
        REQUIRE((m5 * m6) == result3);

//        Mat<real, 2, 3> m3{1.f, 3.f, 2.f,
//                           4.f, 0.f, 1.f};
//        Mat<real, 3, 1> m4{1.f, 0.f, 5.f};
//        auto result2 = m3 * m4;
//        REQUIRE(result2.dimension() == 2);
//        REQUIRE(Math::equal(result2._m16[0], 11.f));
//        REQUIRE(Math::equal(result2._m16[1], 9.f));
    }

    SECTION("matrix addition, subtraction test") {
        Mat<real, 2, 2> m1{4.f, 8.f, 3.f, 7.f};
        Mat<real, 2, 2> m2{1.f, 0.f, 5.f, 2.f};
        Mat<real, 2, 2> result{5.f, 8.f, 8.f, 9.f};
        REQUIRE(m1 + m2 == result);

        Mat<real, 3, 3> m3{1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f};
        Mat<real, 3, 3> m4{9.f, 8.f, 7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f};
        Mat<real, 3, 3> result2{10.f, 10.f, 10.f, 10.f, 10.f, 10.f, 10.f, 10.f, 10.f};
        REQUIRE(m3 + m4 == result2);

        Mat<real, 3, 2> m5{5.f, 2.f, 4.f, 9.f, 10.f, -3.f};
        Mat<real, 3, 2> m6{-11.f, 0.f, 7.f, 1.f, -6.f, -8.f};
        Mat<real, 3, 2> result3{-6.f, 2.f, 11.f, 10.f, 4.f, -11.f};
        REQUIRE(m5 + m6 == result3);

        Mat<real, 2, 3> m7{-1.f, 2.f, 0.f, 4.f, 1.f, 10.f};
        Mat<real, 2, 3> m8{3.f, 2.f, 4.f, 0.f, 8.f, 16.f};
        Mat<real, 2, 3> result4{-4.f, 0.f, -4.f, 4.f, -7.f, -6.f};
        REQUIRE(m7 - m8 == result4);

        Mat<int, 2, 2> m9{1, 2, 3, 4};
        Mat<int, 2, 2> m10{1, 3, 5, 5};
        Mat<int, 2, 2> result5{0, -1, -2, -1};
        REQUIRE(m9 - m10 == result5);
    }

    SECTION("matrix scalar multiplication test") {
        Mat<real, 2, 2> m1;
        Mat<real, 2, 2> m2{2.f, 0.f, 0.f, 2.f};
        REQUIRE(m1 * 2.f == m2);
        m1 *= 2.f;
        REQUIRE(m1 == m2);
        REQUIRE(m1 / 2.f == Mat<real, 2, 2>::identity());
        m1 /= 2.f;
        REQUIRE(m1 == Mat<real, 2, 2>::identity());
    }

    SECTION("matrix index operator test") {
        Mat<real, 2, 2> m1{1.f, 2.f, 3.f, 4.f};
        REQUIRE(m1(0, 0) == 1.f);
        REQUIRE(m1(0,1) == 2.f);
        REQUIRE(m1(1,0) == 3.f);
        REQUIRE(m1(1,1) == 4.f);

        // TODO: operator[] with Vec test

        REQUIRE(m1.at(0, 0) == 1.f);
        REQUIRE(m1.at(0, 1) == 2.f);
        REQUIRE(m1.at(1, 0) == 3.f);
        REQUIRE(m1.at(1, 1) == 4.f);

        REQUIRE(m1.at(0) == 1.f);
        REQUIRE(m1.at(1) == 2.f);
        REQUIRE(m1.at(2) == 3.f);
        REQUIRE(m1.at(3) == 4.f);

        // TODO: col, row (index) test
    }

    SECTION("matrix copy test") {
        Mat<real, 2, 2> m1;
        Mat<real, 2, 2> m2{4.f, 8.f, 3.f, 7.f};
        m1 = m2;
        REQUIRE(m1 == m2);

        Mat<real, 3, 2> m3;
        Mat<real, 3, 2> m4{1.f, 2.f, 3.f, 4.f, 5.f, 6.f};
        m3 = m4;
        REQUIRE(m3 == m4);
    }

    SECTION("makeIdentity test") {
        Mat<real, 2, 2> m1{4.f, 8.f, 3.f, 7.f};
        REQUIRE(m1.isIdentity() == false);
        m1.makeIdentity();
        REQUIRE(m1 == Mat<real, 2, 2>::identity());
        REQUIRE(m1.isIdentity());

        Mat<real, 100, 100> m2 = Mat<real, 100, 100>::from(100.f);
        REQUIRE(m2.isIdentity() == false);
        m2.makeIdentity();
        REQUIRE(m2 == Mat<real, 100, 100>::identity());
        REQUIRE(m2.isIdentity());
    }

    SECTION("matrix transpose test") {
        Mat<real, 3, 3> m1{1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f};
        Mat<real, 3, 3> result1{1.f, 4.f, 7.f, 2.f, 5.f, 8.f, 3.f, 6.f, 9.f};
        REQUIRE(Mat<real, 3, 3>::transpose(m1) == result1);

        Mat<real, 3, 4> m2{1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f};
        Mat<real, 4, 3> result2{1.f, 5.f, 9.f, 2.f, 6.f, 10.f, 3.f, 7.f, 11.f, 4.f, 8.f, 12.f};
        REQUIRE(Mat<real, 3, 4>::transpose(m2) == result2);

        Mat<real, 1, 4> m3{1.f, 2.f, 3.f, 4.f};
        Mat<real, 4, 1> result3{1.f, 2.f, 3.f, 4.f};
        REQUIRE(Mat<real, 1, 4>::transpose(m3) == result3);
    }

    SECTION("arithmetic with other matrix test") {
    }

    SECTION("equality test") {
    }

    SECTION("multiply with Vec") {
//        Mat<real, 4, 4> m1;
//        Vec<real, 4> v1{1,2,3,4};
//        REQUIRE((v1 * m1) == Vec<real, 4>{1,2,3,4});
    }

    SECTION("inverse of matrix test") {
    }

    SECTION("inverse of determinant of matrix test") {
    }

    SECTION("toSquareMat test") {
    }

    SECTION("matrix trace") {
        // tr(I) = n
        Mat<real, 4, 4> I;
        REQUIRE(Math::equal(Mat<real, 4, 4>::trace(I), 4.f));

        // tr(A) = sum(Aii)
        Mat<int, 4, 4> m1{
            1, 0, 0, 0,
            0, 2, 0, 0,
            0, 0, 3, 0,
            0, 0, 0, 4
        };
        REQUIRE(Mat<int, 4, 4>::trace(m1) == 10);
    }

    SECTION("matrix elementary row operations") {
    }
}