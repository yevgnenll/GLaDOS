#include <catch.hpp>
#include <vector>

#include "memory/STLAllocator.h"

using namespace GLaDOS;

TEST_CASE("STLAllocator unit tests", "[STLAllocator]") {
  struct CustomType {
    int a;
    float b;
    double c;
  };
  std::vector<int, STLAllocator<int>> vec;
  std::vector<CustomType, STLAllocator<CustomType>> vec2;

  SECTION("integer type allocation test") {
    vec.push_back(10);
    vec.push_back(20);
    REQUIRE(vec[0] == 10);
    REQUIRE(vec[1] == 20);
    dumpMemory();
  }

  SECTION("custom structure type allocation test") {
    vec2.push_back({10, 10.f, 10.0});
    REQUIRE(vec2[0].a == 10);
    dumpMemory();
  }
}
