#include <catch2/catch.hpp>

#include "utils/StringUtils.h"
#include "utils/String.hpp"

using namespace GLaDOS;

TEST_CASE("StringUtils unit tests", "[StringUtils]") {
  SECTION("base path and file name extraction test") {
    std::string fullPath = "/usr/bin/local/test.cpp";
    auto [basePath, fileName] = StringUtils::splitFileName(fullPath);
    REQUIRE(basePath == "/usr/bin/local/");
    REQUIRE(fileName == "test.cpp");
  }

  SECTION("empty string extraction test") {
    std::string emptyString = "";
    auto [basePath, fileName] = StringUtils::splitFileName(emptyString);
    REQUIRE(basePath == "");
    REQUIRE(fileName == "");
  }

  SECTION("String to decimal number test") {
      String s = "1123";
      REQUIRE(StringUtils::toLong(s) == 1123);
      REQUIRE(StringUtils::toInt(s) == 1123);
      REQUIRE(StringUtils::toDouble(s) == 1123.0);
      REQUIRE(StringUtils::toFloat(s) == 1123.f);
  }
}
