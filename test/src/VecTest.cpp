#include <catch2/catch.hpp>

#include "math/Vec.hpp"

using namespace GLaDOS;

TEST_CASE("Vec unit tests", "[Vector]") {
    SECTION("default constructor test") {
        Vec<real, 2> v;
        REQUIRE(v == Vec<real, 2>{0.f, 0.f});
        v.xy = Vec<real, 2>{1.f, 2.f};
        REQUIRE(v == Vec<real, 2>{1.f, 2.f});

        Vec<real, 3> v2;
        REQUIRE(v2 == Vec<real, 3>{0.f, 0.f, 0.f});
        v2.xyz = Vec<real, 3>{1.f, 2.f, 3.f};
        REQUIRE(v2 == Vec<real, 3>{1.f, 2.f, 3.f});
        REQUIRE(v2.s == 1.f);
        REQUIRE(v2.t == 2.f);
        REQUIRE(v2.p == 3.f);
        REQUIRE(v2.v[0] == 1.f);
        REQUIRE(v2.v[1] == 2.f);
        REQUIRE(v2.v[2] == 3.f);
        REQUIRE(v2.xz == Vec<real, 2>{1.f, 3.f});
        REQUIRE(v2.x == 1.f);
    }

    SECTION("Vec copy constructor test") {
    }

    SECTION("Vec copy assignment operator (copy and swap idiom) test") {
    }

    SECTION("Vec move constructor (copy and swap idiom) test") {
    }

    SECTION("Vec3 cross product") {
    }

    SECTION("Vec dot product") {
    }

    SECTION("Vec3 angle between two normalized vector") {
    }

    SECTION("Vec3 angle between two vector") {
    }

    SECTION("Vec2 angle between two normalized vector") {
    }

    SECTION("Vec2 angle between two vector") {
    }

    SECTION("Vec2 slerp") {
    }

    SECTION("Vec2 slerp2") {
    }

    SECTION("Vec2 slerp3") {
    }
}