#include <catch2/catch.hpp>

#include "math/Vec2.h"
#include "math/UVec2.h"
#include "math/Vec3.h"
#include "math/UVec3.h"
#include "math/Math.h"

using namespace GLaDOS;

TEST_CASE("Vector unit tests", "[Vector]") {
  SECTION("Vec3 default init test") {
    Vec3 v;
    REQUIRE(v == Vec3{0, 0, 0});
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

  SECTION("Vec2 angle between two normalized vector") {
      Vec2 v1 = {1, 0};
      Vec2 v2 = {0.5f, Math::sqrt(3.f)/2.f};
      Deg degree = Vec2::angleBetween(v1.makeNormalize(), v2.makeNormalize());
      REQUIRE(Math::equal(degree.get(), 60.f));
  }

  SECTION("Vec2 angle between two vector") {
      Vec2 v1 = {1, 0};
      Vec2 v2 = {0.5f, Math::sqrt(3.f)/2.f};
      Deg degree = Vec2::angleBetween(v1, v2);
      REQUIRE(Math::equal(degree.get(), 60.f));
  }

  SECTION("Vec2 slerp") {
      Vec2 v1 = {1, 0};
      Vec2 v2 = {0.5f, Math::sqrt(3.f)/2.f};
      Vec2 result = Vec2::slerp(v1, v2, 0.5f);
      REQUIRE(result == Vec2{Math::sqrt(3.f)/2.f, 0.5f});
  }

  SECTION("Vec2 slerp2") {
      Vec2 v1 = {1, 0};
      Vec2 v2 = {0.5f, Math::sqrt(3.f)/2.f};
      Vec2 result = Vec2::slerp(v1, v2, 0.75f);
      REQUIRE(result == Vec2{Math::sqrt(2.f)/2.f, Math::sqrt(2.f)/2.f});
  }

  SECTION("Vec2 slerp3") {
      Vec2 v1 = {1, 0};
      Vec2 v2 = {0, 1};
      Vec2 result = Vec2::slerp(v1, v2, 0.75f);
      REQUIRE(result == Vec2{0.38268342, 0.92387950});
  }
}
