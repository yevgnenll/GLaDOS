#include <catch2/catch.hpp>

#include "math/Vec3.h"
#include "math/UVec3.h"
#include "math/Math.h"

using namespace GLaDOS;

TEST_CASE("Vector unit tests", "[Vector]") {
  SECTION("Vec3 default init test") {
    Vec3 v;
    REQUIRE(v.x == 0.0);
    REQUIRE(v.y == 0.0);
    REQUIRE(v.z == 0.0);
  }

  SECTION("Vec3 copy constructor test") {
    Vec3 v{1, 2, 3};
    Vec3 v2{v};
    REQUIRE(v2.x == 1);
    REQUIRE(v2.y == 2);
    REQUIRE(v2.z == 3);
  }

  SECTION("Vec3 copy assignment operator (copy and swap idiom) test") {
    Vec3 v{1, 2, 3};
    Vec3 v2;
    v2 = v;
    REQUIRE(v2.x == 1);
    REQUIRE(v2.y == 2);
    REQUIRE(v2.z == 3);
  }

  SECTION("Vec3 move constructor (copy and swap idiom) test") {
    Vec3 v = {1, 2, 3};
    Vec3 v2 = std::move(v);
    REQUIRE(v2.x == 1);
    REQUIRE(v2.y == 2);
    REQUIRE(v2.z == 3);
  }

  SECTION("Vec3 angle between two normalized vector") {
      Vec3 v1 = {4, 3, 0};
      Vec3 v2 = {3, 5, 0};
      Deg angle = Vec3::angleBetween(v1.makeNormalize(), v2.makeNormalize());
      REQUIRE(Math::equal(angle.get(), 22.16634f));
  }

  SECTION("Vec3 angle between two vector") {
      Vec3 v1 = {4, 3, 0};
      Vec3 v2 = {3, 5, 0};
      Deg angle = Vec3::angleBetween(v1, v2);
      REQUIRE(Math::equal(angle.get(), 22.16633f));
  }
}
