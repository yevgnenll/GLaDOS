#include <catch2/catch.hpp>
#include "memory/Allocation.h"

using namespace GLaDOS;

TEST_CASE("Allocation unit tests", "[Allocation]") {
  SECTION("calculate minimum size memory aligntment") {
    std::size_t aligned = alignment(sizeof(int32_t) + sizeof(MemBlockDList), _mem_alignment);
    REQUIRE(aligned == 64);
  }

  SECTION("calculate aligntment size over _mem_alignment size") {
    std::size_t aligned = alignment(60 + sizeof(MemBlockDList), _mem_alignment);
    REQUIRE(aligned == 128);
  }
}