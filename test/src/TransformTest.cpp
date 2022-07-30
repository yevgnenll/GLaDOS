#include <catch2/catch_test_macros.hpp>
#include "core/component/Transform.h"

using namespace GLaDOS;

TEST_CASE("Transform unit tests", "[Transform]") {
    SECTION("Transform direction") {

    }

    SECTION("Transform point") {
        Vec3 position = {3, 0, 0};

        Transform transform1;
        Vec3 result = transform1.transformPoint(position);
        REQUIRE(result == Vec3{6, 0, 0});

        Transform transform2;
        transform2.setLocalScale(Vec3{2, 1, 1});
        Vec3 result2 = transform2.transformPoint(position);
        REQUIRE(result2 == Vec3{9, 0, 0});

        Transform transform3;
        transform3.setLocalRotation(Quat::fromEuler(Vec3{0, 0, 90}));
        Vec3 result3 = transform3.transformPoint(position);
        REQUIRE(result3 == Vec3{3, 3, 0});

        Transform transform4;
        transform4.setLocalScale(Vec3{2, 1, 1});
        transform4.setLocalRotation(Quat::fromEuler(Vec3{0, 0, 90}));
        Vec3 result4 = transform4.transformPoint(position);
        REQUIRE(result4 == Vec3{3, 6, 0});
    }

    SECTION("Transform vector") {

    }
}