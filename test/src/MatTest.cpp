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

        Mat<real, 1, 1> m11;
        REQUIRE(m11.isIdentity());
        REQUIRE(Mat<real, 1, 1>::identity() == m11);
    }
}