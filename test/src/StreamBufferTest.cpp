#include <catch.hpp>

#include "memory/StreamBuffer.h"

using namespace GLaDOS;

TEST_CASE("StreamBuffer unit tests", "[StreamBuffer]") {
  StreamBuffer sb;

  SECTION("write unsigned 32bits int") {
    uint32_t i = 10;
    sb << i;
    REQUIRE(sb.size() == 4);
  }
}
