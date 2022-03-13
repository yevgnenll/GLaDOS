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

        Mat<real, 2, 3> m7{1.f, 3.f, 2.f,
                           4.f, 0.f, 1.f};
        Mat<real, 3, 1> m8{1.f, 0.f, 5.f};
        Mat<real, 1, 2> m9{1.f, 1.f};
        Mat<real, 2, 2> result4 = m7 * m8 * m9;
        REQUIRE(result4.dimension() == 4);
        REQUIRE(Math::equal(result4._m16[0], 11.f));
        REQUIRE(Math::equal(result4._m16[2], 9.f));
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

        Mat<real, 4, 4> m2{
            -3.f, -1.f, 2.f, -3.f,
            -3.f, 1.f, 2.f, -2.f,
            -2.f, 3.f, 0.f, 1.f,
            1.f, -2.f, -3.f, 1.f
        };
        Vec<real, 4> column = m2.col(3);
        Vec<real, 4> row = m2.row(3);
//        REQUIRE(column == Vec<real, 4>{-3.f, -2.f, 1.f, 1.f});
//        REQUIRE(row == Vec<real, 4>{1.f, -2.f, -3.f, 1.f});
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

    SECTION("matrix equality test") {
        Mat<real, 999, 999> m1;
        Mat<real, 999, 999> m2;
        REQUIRE(m1 == m2);
        m1._m16[997] = 1.f;
        REQUIRE(m1 != m2);
    }

    SECTION("multiply with Vec") {
//        Mat<real, 4, 4> m1;
//        Vec<real, 4> v1{1,2,3,4};
//        REQUIRE((v1 * m1) == Vec<real, 4>{1,2,3,4});
    }

    SECTION("matrix diagonalize test") {
        Mat<real, 3, 3> m1{1.f, 2.f, 3.f,
                           4.f, 5.f, 6.f,
                           7.f, 8.f, 8.f};
        Vec<real, 3> diag = Mat<real, 3, 3>::diagonal(m1);
//        REQUIRE(diag == Vec<real, 3>{1.f, 5.f, 8.f});
    }

    SECTION("matrix minor test") {
        Mat<real, 3, 3> m1{1.f, 2.f, 3.f,
                           4.f, 5.f, 6.f,
                           7.f, 8.f, 9.f};
        REQUIRE(Math::equal(Mat<real, 3, 3>::minor(m1, 0, 0), -3.f));
        REQUIRE(Math::equal(Mat<real, 3, 3>::minor(m1, 0, 1), -6.f));
        REQUIRE(Math::equal(Mat<real, 3, 3>::minor(m1, 0, 2), -3.f));
        REQUIRE(Math::equal(Mat<real, 3, 3>::minor(m1, 1, 0), -6.f));
        REQUIRE(Math::equal(Mat<real, 3, 3>::minor(m1, 1, 1), -12.f));
        REQUIRE(Math::equal(Mat<real, 3, 3>::minor(m1, 1, 2), -6.f));
        REQUIRE(Math::equal(Mat<real, 3, 3>::minor(m1, 2, 0), -3.f));
        REQUIRE(Math::equal(Mat<real, 3, 3>::minor(m1, 2, 1), -6.f));
        REQUIRE(Math::equal(Mat<real, 3, 3>::minor(m1, 2, 2), -3.f));
        Mat<real, 4, 4> m2{1.f, 2.f, 3.f, 4.f,
                           5.f, 6.f, 7.f, 8.f,
                           9.f, 10.f, 11.f, 12.f,
                           13.f, 14.f, 15.f, 16.f};
        REQUIRE(Math::equal(Mat<real, 4, 4>::minor(m2, 0, 0), 0.f));
        REQUIRE(Math::equal(Mat<real, 4, 4>::minor(m2, 0, 1), 0.f));
        REQUIRE(Math::equal(Mat<real, 4, 4>::minor(m2, 0, 2), 0.f));
        REQUIRE(Math::equal(Mat<real, 4, 4>::minor(m2, 0, 3), 0.f));
        REQUIRE(Math::equal(Mat<real, 4, 4>::minor(m2, 1, 0), 0.f));
        REQUIRE(Math::equal(Mat<real, 4, 4>::minor(m2, 1, 1), 0.f));
        REQUIRE(Math::equal(Mat<real, 4, 4>::minor(m2, 1, 2), 0.f));
        REQUIRE(Math::equal(Mat<real, 4, 4>::minor(m2, 1, 3), 0.f));
        REQUIRE(Math::equal(Mat<real, 4, 4>::minor(m2, 2, 0), 0.f));
        REQUIRE(Math::equal(Mat<real, 4, 4>::minor(m2, 2, 1), 0.f));
        REQUIRE(Math::equal(Mat<real, 4, 4>::minor(m2, 2, 2), 0.f));
        REQUIRE(Math::equal(Mat<real, 4, 4>::minor(m2, 2, 3), 0.f));
        REQUIRE(Math::equal(Mat<real, 4, 4>::minor(m2, 3, 0), 0.f));
        REQUIRE(Math::equal(Mat<real, 4, 4>::minor(m2, 3, 1), 0.f));
        REQUIRE(Math::equal(Mat<real, 4, 4>::minor(m2, 3, 2), 0.f));
        REQUIRE(Math::equal(Mat<real, 4, 4>::minor(m2, 3, 3), 0.f));
    }

    SECTION("matrix cofactor test") {
        Mat<real, 3, 3> m1{1.f, 2.f, 3.f,
                           4.f, 5.f, 6.f,
                           7.f, 8.f, 9.f};
        Mat<real, 3, 3> c1{
            Mat<real, 3, 3>::cofactor(m1, 0, 0), Mat<real, 3, 3>::cofactor(m1, 0, 1), Mat<real, 3, 3>::cofactor(m1, 0, 2),
            Mat<real, 3, 3>::cofactor(m1, 1, 0), Mat<real, 3, 3>::cofactor(m1, 1, 1), Mat<real, 3, 3>::cofactor(m1, 1, 2),
            Mat<real, 3, 3>::cofactor(m1, 2, 0), Mat<real, 3, 3>::cofactor(m1, 2, 1), Mat<real, 3, 3>::cofactor(m1, 2, 2)
        };
        REQUIRE(c1 == Mat<real, 3, 3>{-3.f, 6.f, -3.f, 6.f, -12.f, 6.f, -3.f, 6.f, -3.f});
    }

    SECTION("matrix adjugate test") {
        Mat<real, 3, 3> m1{1.f, 2.f, 3.f,
                           4.f, 5.f, 6.f,
                           7.f, 8.f, 9.f};
        Mat<real, 3, 3> c1{
            Mat<real, 3, 3>::cofactor(m1, 0, 0), Mat<real, 3, 3>::cofactor(m1, 0, 1), Mat<real, 3, 3>::cofactor(m1, 0, 2),
            Mat<real, 3, 3>::cofactor(m1, 1, 0), Mat<real, 3, 3>::cofactor(m1, 1, 1), Mat<real, 3, 3>::cofactor(m1, 1, 2),
            Mat<real, 3, 3>::cofactor(m1, 2, 0), Mat<real, 3, 3>::cofactor(m1, 2, 1), Mat<real, 3, 3>::cofactor(m1, 2, 2)
        };
        REQUIRE(Mat<real, 3, 3>::transpose(c1) == Mat<real, 3, 3>::adjugate(m1));
    }

    SECTION("matrix determinant test") {
        Mat<real, 2, 2> m1;
        REQUIRE(Math::equal(Mat<real, 2, 2>::determinant(m1), 1.f));

        Mat<real, 2, 2> m2{1.f, 2.f, 3.f, 4.f};
        REQUIRE(Math::equal(Mat<real, 2, 2>::determinant(m2), -2.f));

        Mat<real, 3, 3> m3{1.f, 1.f, 2.f, 3.f, 4.f, -7.f, 6.f, 8.f, 2.f};
        REQUIRE(Math::equal(Mat<real, 3, 3>::determinant(m3), 16.f));

        Mat<real, 4, 4> m4{
            0.f, 4.f, -2.f, 4.f,
            -6.f, 2.f, 10.f, 0.f,
            5.f, 8.f, -5.f, 2.f,
            0.f, -2.f, 1.f, 0.f
        };
        REQUIRE(Math::equal(Mat<real, 4, 4>::determinant(m4), 392.f));
    }

    SECTION("inverse of matrix test") {
        Mat<real, 4, 4> m1{
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            1.f, 2.f, 3.f, 1.f
        };
        Mat<real, 4, 4> m1Inverse{
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            -1.f, -2.f, -3.f, 1.f
        };

        REQUIRE(Mat<real, 4, 4>::inverse(m1) == m1Inverse);
        REQUIRE(m1 * m1Inverse == Mat<real, 4, 4>::identity());
        REQUIRE(m1Inverse * m1 == Mat<real, 4, 4>::identity());
        REQUIRE(m1.makeInverse() == m1Inverse);

        Mat<real, 3, 3> m2{
            1.f, 0.f, 5.f,
            2.f, 1.f, 6.f,
            3.f, 4.f, 0.f
        };
        Mat<real, 3, 3> m2Inverse{
            -24.f, 20.f, -5.f,
            18.f, -15.f, 4.f,
            5.f, -4.f, 1.f
        };

        REQUIRE(Mat<real, 3, 3>::inverse(m2) == m2Inverse);
        REQUIRE(m2 * m2Inverse == Mat<real, 3, 3>::identity());
        REQUIRE(m2Inverse * m2 == Mat<real, 3, 3>::identity());
        REQUIRE(m2.makeInverse() == m2Inverse);
    }

    SECTION("inverse of determinant of matrix test") {
        Mat<real, 4, 4> m1{
            -3.f, -1.f, 2.f, -3.f,
            -3.f, 1.f, 2.f, -2.f,
            -2.f, 3.f, 0.f, 1.f,
            1.f, -2.f, -3.f, 1.f
        };
        Mat<real, 4, 4> inverseM1 = Mat<real, 4, 4>::inverse(m1);
        real determinantOfInverseM1 = Mat<real, 4, 4>::determinant(inverseM1);
        REQUIRE(Math::equalUlps(Mat<real, 4, 4>::inverseDeterminant(m1), determinantOfInverseM1, 23));
    }

    SECTION("toSquareMat test") {
        Mat<real, 4, 4> m1;
        Mat<real, 3, 3> m2 = Mat<real, 4, 4>::toSquareMat<3, 3>(m1);
        Mat<real, 2, 2> m3 = Mat<real, 4, 4>::toSquareMat<2, 2>(m1);
        REQUIRE(m2 == Mat<real, 3, 3>::identity());
        REQUIRE(m3 == Mat<real, 2, 2>::identity());
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
        // TODO
        // 1. Row-multiplying transformations
        Mat<real, 4, 4> em1 = Mat<real, 4, 4>::elementary1(2, 3.f);

        // 2. Row switching transformations
        Mat<real, 4, 4> em2 = Mat<real, 4, 4>::elementary2(1, 2);

        // 3. Row-addition transformations
        Mat<real, 4, 4> em3 = Mat<real, 4, 4>::elementary3(2, 3.f, 1);
    }

    SECTION("matrix perspective test") {
        Mat<real, 4, 4> m1 = Mat<real, 4, 4>::perspective(20_rad, 0.2f, 0.1f, 100.f);
        Mat<real, 4, 4> result{
            7.71175479f, 0.f, 0.f, 0.f,
            0.f, 1.54235101f, 0.f, 0.f,
            0.f, 0.f, -1.001001f, -1.f,
            0.f, 0.f, -0.1001001f, 0.f
        };
        REQUIRE(m1 == result);
    }

    SECTION("matrix orthogonal test") {

    }

    SECTION("matrix frustum test") {

    }

    SECTION("matrix lookAt test") {

    }

    SECTION("matrix translation test") {

    }

    SECTION("matrix scale test") {

    }

    SECTION("matrix rotation test") {

    }

    SECTION("matrix decomposition test") {

    }

    SECTION("matrix build from SRT test") {

    }
}