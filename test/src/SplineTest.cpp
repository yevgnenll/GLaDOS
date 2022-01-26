#include <catch2/catch.hpp>
#include "math/curve/BezierSpline.hpp"
#include "math/Vec3.h"

using namespace GLaDOS;

TEST_CASE("Spline unit tests", "[BezierSpline]") {
    SECTION("BezierSpline Vec3 test") {
        BezierSpline<Vec3> curve{
            Vec3{-5, 0, 0}, Vec3{-2, 1, 0},
            Vec3{5, 0, 0}, Vec3{2, 1, 0}
        };
        Vec3 result = curve.interpolate(0.5f);
        REQUIRE(result == Vec3{0, 0.75, 0});
    }
}