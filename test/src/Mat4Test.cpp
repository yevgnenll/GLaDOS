#include <catch2/catch.hpp>
#include <array>

#include "math/Mat4.hpp"

using namespace GLaDOS;

TEST_CASE("Matrix unit tests", "[Matrix]") {
  SECTION("default constructor test") {
    Mat4<real> m;
    REQUIRE(Mat4<real>::identity() == m);
  }

  SECTION("constructor with parameters test") {
    Mat4<real> m{
        1.F, 2.F, 3.F, 4.F,
        5.F, 6.F, 7.F, 8.F,
        9.F, 10.F, 11.F, 12.F,
        13.F, 14.F, 15.F, 16.F};
    std::array<real, 16> results = {1.F, 2.F, 3.F, 4.F, 5.F, 6.F, 7.F, 8.F, 9.F, 10.F, 11.F, 12.F, 13.F, 14.F, 15.F, 16.F};
    for (int i = 0; i < 16; ++i) {
      REQUIRE(m._m16[i] == results[i]);
    }

    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        REQUIRE(m._m44[i][j] == results[i * 4 + j]);
      }
    }

    REQUIRE(m._11 == 1.F);
    REQUIRE(m._12 == 2.F);
  }

  SECTION("matrix copy test") {
    Mat4<real> m{
        1.F, 2.F, 3.F, 4.F,
        5.F, 6.F, 7.F, 8.F,
        9.F, 10.F, 11.F, 12.F,
        13.F, 14.F, 15.F, 16.F};
    Mat4<real> m2 = m;  // copy constructor
    REQUIRE(m2 == m);
    Mat4<real> m3;
    m3 = m2;  // assign operator
    REQUIRE(m3 == m2);
    REQUIRE(m3 == m);
  }

  SECTION("makeIdentity test") {
    Mat4<real> m{
        1.F, 2.F, 3.F, 4.F,
        5.F, 6.F, 7.F, 8.F,
        9.F, 10.F, 11.F, 12.F,
        13.F, 14.F, 15.F, 16.F};
    m.makeIdentity();
    REQUIRE(m == Mat4<real>::identity());
  }

  SECTION("makeTranspose test") {
    Mat4<real> m{
        1.F, 2.F, 3.F, 4.F,
        5.F, 6.F, 7.F, 8.F,
        9.F, 10.F, 11.F, 12.F,
        13.F, 14.F, 15.F, 16.F};
    Mat4<real> result{
        1.F, 5.F, 9.F, 13.F,
        2.F, 6.F, 10.F, 14.F,
        3.F, 7.F, 11.F, 15.F,
        4.F, 8.F, 12.F, 16.F};
    m.makeTranspose();
    REQUIRE(m == result);
  }

  SECTION("arithmetic with other matrix test") {
    Mat4<real> m1, m2;
    Mat4<real> result{
        2.F, 0.F, 0.F, 0.F,
        0.F, 2.F, 0.F, 0.F,
        0.F, 0.F, 2.F, 0.F,
        0.F, 0.F, 0.F, 2.F};

    // plus
    REQUIRE(m1 + m2 == result);
    m1 += m2;
    REQUIRE(m1 == result);

    // minus
    REQUIRE(m1 - m2 == Mat4<real>::identity());
    m1 -= m2;
    REQUIRE(m1 == m2);

    // multiply
    REQUIRE(m2 * Mat4<real>::zero == Mat4<real>::zero);
    m2 *= Mat4<real>::identity();
    REQUIRE(m2 == m2);

    Mat4<real> m3{
        1.F, 2.F, 3.F, 4.F,
        5.F, 6.F, 7.F, 8.F,
        9.F, 10.F, 11.F, 12.F,
        13.F, 14.F, 15.F, 16.F};
    Mat4<real> m4{
        16.F, 15.F, 14.F, 13.F,
        12.F, 11.F, 10.F, 9.F,
        8.F, 7.F, 6.F, 5.F,
        4.F, 3.F, 2.F, 1.F};
    Mat4<real> mul = m3 * m4;
    REQUIRE(mul == Mat4<real>{80.F, 70.F, 60.F, 50.F,
                         240.F, 214.F, 188.F, 162.F,
                         400.F, 358.F, 316.F, 274.F,
                         560.F, 502.F, 444.F, 386.F});
  }

  SECTION("equality test") {
    Mat4<real> m1, m2;
    REQUIRE(m1 == m2);
    m1._12 = 1.F;
    REQUIRE(m1 != m2);
    REQUIRE(Mat4<real>::zero == Mat4<real>::zero);
  }

  SECTION("multiply with Vec4") {
    Mat4<real> m1;
    Vec4 v1{1,2,3,4};
    Vec4 v2 = v1 * m1;
    REQUIRE(v2 == Vec4{1,2,3,4});
  }

  SECTION("inverse of matrix test") {
    Mat4<real> m1{
        -3.F, -1.F, 2.F, -3.F,
        -3.F, 1.F, 2.F, -2.F,
        -2.F, 3.F, 0.F, 1.F,
        1.F, -2.F, -3.F, 1.F};
    Mat4<real> result{
        -11.F / 7.F, 2.F, -1.F, 2.F / 7.F,
        -15.F / 7.F, 3.F, -1.F, 4.F / 7.F,
        2.F, -3.F, 1.F, -1.F,
        23.F / 7.F, -5.F, 2.F, -8.F / 7.F};

    REQUIRE(Mat4<real>::inverse(m1) == result);
    REQUIRE(m1.makeInverse() == result);
  }

  SECTION("inverse of determinant of matrix test") {
      Mat4<real> m1{
          -3.F, -1.F, 2.F, -3.F,
          -3.F, 1.F, 2.F, -2.F,
          -2.F, 3.F, 0.F, 1.F,
          1.F, -2.F, -3.F, 1.F};
      Mat4<real> inverseM1 = Mat4<real>::inverse(m1);
      real determinantOfInverseM1 = Mat4<real>::determinant(inverseM1);

      REQUIRE(Math::equalUlps(Mat4<real>::inverseDeterminant(m1), determinantOfInverseM1, 16));
  }

  SECTION("toMat3 test") {
    Mat4<real> m1{
        -3.F, -1.F, 2.F, -3.F,
        -3.F, 1.F, 2.F, -2.F,
        -2.F, 3.F, 0.F, 1.F,
        1.F, -2.F, -3.F, 1.F};
    Mat4<real> result{
        -3.F, -1.F, 2.F, -3.F,
        -3.F, 1.F, 2.F, -2.F,
        -2.F, 3.F, 0.F, 1.F,
        0.F, 0.F, 0.F, 1.F};

    REQUIRE(Mat4<real>::toMat3(m1) == result);
  }

  SECTION("matrix abs test") {
    Mat4<real> m1{
        -3.F, -1.F, 2.F, -3.F,
        -3.F, 1.F, 2.F, -2.F,
        -2.F, 3.F, 0.F, 1.F,
        1.F, -2.F, -3.F, 1.F};
    Mat4<real> result{
        3.F, 1.F, 2.F, 3.F,
        3.F, 1.F, 2.F, 2.F,
        2.F, 3.F, 0.F, 1.F,
        1.F, 2.F, 3.F, 1.F};

    REQUIRE(Mat4<real>::abs(m1) == result);
  }

  SECTION("matrix perspective test") {
    Mat4<real> m1;
    auto m2 = Mat4<real>::perspective(20_rad, 0.2f, 0.1f, 100.f);
  }

  SECTION("matrix get col & row") {
      Mat4<real> m1{
          -3.F, -1.F, 2.F, -3.F,
          -3.F, 1.F, 2.F, -2.F,
          -2.F, 3.F, 0.F, 1.F,
          1.F, -2.F, -3.F, 1.F};
      Vec4 columnVector = m1.col(3);
      REQUIRE(columnVector == Vec4{-3.f, -2.f, 1.f, 1.f});

      Mat4<real> I;
      Vec4 basis = I.row(0);
      REQUIRE(basis == Vec4::right);
  }

  SECTION("matrix trace") {
      // tr(I) = n
      Mat4<real> I;
      REQUIRE(Mat4<real>::trace(I) == 4);

      // tr(A) = sum(Aii)
      Mat4<real> A = {
          1, 0, 0, 0,
          0, 2, 0, 0,
          0, 0, 3, 0,
          0, 0, 0, 4
      };
      REQUIRE(Mat4<real>::trace(A) == 10);
  }
}