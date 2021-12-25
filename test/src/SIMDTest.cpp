#include <catch2/catch.hpp>
#include "utils/SIMD.h"
#include "math/Math.h"

using namespace GLaDOS;

TEST_CASE("SIMD unit test", "[SIMD]") {
  SIMDVec4 a = SIMD_load(1, 2, 3, 4);
  SIMDVec4 b = SIMD_load(5, 6, 7, 8);

  SECTION("SIMD load and store test") {
    float result[4] = { 0.f, };
    SIMD_store(result, a);
    REQUIRE(result[0] == 1);
    REQUIRE(result[1] == 2);
    REQUIRE(result[2] == 3);
    REQUIRE(result[3] == 4);
  }

  SECTION("SIMD add test") {
    SIMDVec4 c = SIMD_add(a, b);
    float result[4] = { 0.f, };
    SIMD_store(result, c);
    float testSuite[4] = { 6, 8, 10, 12 };
    for (int i = 0; i < 4; i++) {
      REQUIRE(result[i] == testSuite[i]);
    }
  }

  SECTION("SIMD sub test") {
    SIMDVec4 c = SIMD_sub(a, b);
    float result[4] = { 0.f, };
    SIMD_store(result, c);
    float testSuite[4] = { -4, -4, -4, -4 };
    for (int i = 0; i < 4; i++) {
      REQUIRE(result[i] == testSuite[i]);
    }
  }

  SECTION("SIMD mul test") {
    SIMDVec4 c = SIMD_mul(a, b);
    float result[4] = { 0.f, };
    SIMD_store(result, c);
    float testSuite[4] = { 5, 12, 21, 32 };
    for (int i = 0; i < 4; i++) {
      REQUIRE(result[i] == testSuite[i]);
    }
  }

  SECTION("SIMD div test") {
    SIMDVec4 c = SIMD_div(a, b);
    float result[4] = { 0.f, };
    SIMD_store(result, c);
    float testSuite[4] = { 0.2f, 0.333333343f, 0.428571433f, 0.5f };
    for (int i = 0; i < 4; i++) {
      REQUIRE(Math::equal(result[i], testSuite[i]));
    }
  }

  SECTION("SIMD rcp test") {
    SIMDVec4 c = SIMD_rcp(a);
    float result[4] = { 0.f, };
    SIMD_store(result, c);
    float testSuite[4] = { 0.998046875f, 0.499023438f, 0.333007813f, 0.249511719f };
    for (int i = 0; i < 4; i++) {
      REQUIRE(Math::equal(result[i], testSuite[i]));
    }
  }

  SECTION("SIMD sqrt test") {
    SIMDVec4 c = SIMD_sqrt(a);
    float result[4] = { 0.f, };
    SIMD_store(result, c);
    float testSuite[4] = { 1.f, 1.41421354f, 1.73205078f, 2.f };
    for (int i = 0; i < 4; i++) {
      REQUIRE(Math::equal(result[i], testSuite[i]));
    }
  }

  SECTION("SIMD rsqrt test") {
    SIMDVec4 c = SIMD_rsqrt(a);
    float result[4] = { 0.f, };
    SIMD_store(result, c);
    float testSuite[4] = { 0.998046875f, 0.705078125f, 0.576171875f, 0.499023438f };
    for (int i = 0; i < 4; i++) {
      REQUIRE(Math::equal(result[i], testSuite[i]));
    }
  }
}