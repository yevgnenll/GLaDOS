#include <catch2/catch.hpp>

#include "memory/Blob.h"

using namespace GLaDOS;

TEST_CASE("Blob unit tests", "[Blob]") {
    Blob sb;

  SECTION("write unsigned 32bits int") {
    sb.resize(sizeof(uint32_t));
    uint32_t i = 10;
    sb << i;
    REQUIRE(sb.size() == 4);
  }
}
