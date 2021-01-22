#include <catch.hpp>

#include "utils/StringUtils.h"

using namespace GameEngine;

TEST_CASE("StringUtils unit tests", "[StringUtils]") {
  SECTION("base path and file name extraction test") {
    std::string fullPath = "/usr/bin/local/test.cpp";
    auto [basePath, fileName] = StringUtils::extractBaseFileNamePair(fullPath);
    REQUIRE(basePath == "/usr/bin/local/");
    REQUIRE(fileName == "test.cpp");
  }

  SECTION("empty string extraction test") {
    std::string emptyString = "";
    auto [basePath, fileName] = StringUtils::extractBaseFileNamePair(emptyString);
    REQUIRE(basePath == "");
    REQUIRE(fileName == "");
  }
}