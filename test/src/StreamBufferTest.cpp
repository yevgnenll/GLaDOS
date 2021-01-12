#include <catch.hpp>

#include "memory/StreamBuffer.h"

using namespace GameEngine;

TEST_CASE("StreamBuffer test") {
  StreamBuffer sb;

  SECTION("write unsigned 32bits int") {
    uint32_t i = 10;
    sb << i;
    REQUIRE(sb.size() == 4);
  }
}