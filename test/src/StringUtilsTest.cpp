#include <catch2/catch_all.hpp>

#include "utils/StringUtils.h"

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
}
