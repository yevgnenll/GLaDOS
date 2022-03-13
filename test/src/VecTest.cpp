#include <catch2/catch.hpp>

#include "math/Vec.hpp"

using namespace GLaDOS;

TEST_CASE("Vec unit tests", "[Vector]") {
    SECTION("Vec constructor test") {
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
        v2.yx = Vec<real, 2>{1.f, 2.f};
        REQUIRE(v2.xy == Vec<real, 2>{2.f, 1.f});

        Vec<real, 4> v3;
        REQUIRE(v3 == Vec<real, 4>{0.f, 0.f, 0.f, 0.f});
        v3.s = 1.f;
        v3.t = 2.f;
        v3.p = 3.f;
        v3.q = 4.f;
        REQUIRE(v3 == Vec<real, 4>{1.f, 2.f, 3.f, 4.f});
    }

    SECTION("Vec copy test") {
        Vec<real, 2> v1;
        Vec<real, 2> v2{1.f, 2.f};
        v1 = v2;
        REQUIRE(v1 == Vec<real, 2>{1.f, 2.f});

        Vec<real, 2> v3 = Vec<real, 2>{1.f, 2.f};
        REQUIRE(v3 == Vec<real, 2>{1.f, 2.f});

        Vec<real, 3> v4;
        Vec<real, 3> v5{1.f, 2.f, 3.f};
        v4 = v5;
        REQUIRE(v4 == Vec<real, 3>{1.f, 2.f, 3.f});

        Vec<real, 3> v6 = Vec<real, 3>{1.f, 2.f, 3.f};
        REQUIRE(v6 == Vec<real, 3>{1.f, 2.f, 3.f});

        Vec<real, 4> v7;
        Vec<real, 4> v8{1.f, 2.f, 3.f, 4.f};
        v7 = v8;
        REQUIRE(v7 == Vec<real, 4>{1.f, 2.f, 3.f, 4.f});

        Vec<real, 4> v9 = Vec<real, 4>{1.f, 2.f, 3.f, 4.f};
        REQUIRE(v9 == Vec<real, 4>{1.f, 2.f, 3.f, 4.f});
    }

    SECTION("Vec subscript operator test") {
        Vec<real, 2> v1{1.f, 2.f};
        REQUIRE(v1[0] == 1.f);
        REQUIRE(v1[1] == 2.f);

        Vec<real, 3> v2{1.f, 2.f, 3.f};
        REQUIRE(v2[0] == 1.f);
        REQUIRE(v2[1] == 2.f);
        REQUIRE(v2[2] == 3.f);

        Vec<real, 4> v3{1.f, 2.f, 3.f, 4.f};
        REQUIRE(v3[0] == 1.f);
        REQUIRE(v3[1] == 2.f);
        REQUIRE(v3[2] == 3.f);
        REQUIRE(v3[3] == 4.f);
    }

    SECTION("Vec3 cross product test") {
    }

    SECTION("Vec dot product test") {
    }

    SECTION("Vec angle between two normalized vector test") {
    }

    SECTION("Vec3 angle between two vector test") {
    }

    SECTION("Vec2 angle between two normalized vector test") {
    }

    SECTION("Vec2 angle between two vector test") {
    }

    SECTION("Vec2 slerp test") {
    }

    SECTION("Vec2 slerp2 test") {
    }

    SECTION("Vec2 slerp3 test") {
    }
}