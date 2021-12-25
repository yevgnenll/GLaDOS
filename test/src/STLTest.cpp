#include <catch2/catch.hpp>
#include "utils/Stl.h"

using namespace GLaDOS;

TEST_CASE("STL unit tests", "[STL]") {
  struct TestStruct {
    int a = 10;
  };

  SECTION("UnorderedMap find test") {
    UnorderedMap<std::string, TestStruct*> umap;
    umap.try_emplace("test", NEW_T(TestStruct));
    REQUIRE(umap.find("test") != umap.end());
  }

  SECTION("Map find test") {
    Map<std::string, TestStruct*> map;
    map.try_emplace("test", NEW_T(TestStruct));
    REQUIRE(map.find("test") != map.end());
  }
}
