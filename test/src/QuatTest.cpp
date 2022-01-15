#include <catch2/catch.hpp>

#include "math/Quat.h"

using namespace GLaDOS;

TEST_CASE("Quaternion unit test", "[Quaternion]") {
    SECTION("default constructor test") {
        Quat q;
        REQUIRE(Quat::identity == q);
    }
}