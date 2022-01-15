#include <catch2/catch.hpp>
#include "memory/BitMask.h"

using namespace GLaDOS;

TEST_CASE("BitMask unit test", "[BitMask]") {
    SECTION("BitMask set test") {
        BitMask bitMask;
        bitMask.set(0);
        bitMask.set(2);
        bitMask.set(0);
        bitMask.set(7);
        bitMask.set(45);
        bitMask.clear(7);

        REQUIRE(bitMask.isSet(0));
        REQUIRE(bitMask.isSet(2));
        REQUIRE(bitMask.isSet(45));
    }
    SECTION("BitMask clear all test") {
        BitMask bitMask;
        bitMask.set(56);
        bitMask.set(33);
        bitMask.clearAll();

        REQUIRE(bitMask.isSet(56) == false);
        REQUIRE(bitMask.isSet(33) == false);
    }
}