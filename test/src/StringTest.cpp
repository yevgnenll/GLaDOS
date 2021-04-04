#include <catch.hpp>
#include "utils/String.hpp"
#include "utils/Utility.h"

using namespace GLaDOS;

TEST_CASE("String unit tests", "[String]") {
  SECTION("String UTF-16 initialize test") {
    using namespace std::literals;

    String str = "hello";
    REQUIRE(str.length() == 5);
    String str2 = TEXT("日本語");
    REQUIRE(str2.length() == 3);
    String str3{"world", 2};
    REQUIRE(str3.length() == 2);
    String str4{TEXT("한국어"), 2};
    REQUIRE(str4.length() == 2);
    String str5{str4};
    REQUIRE(str5.length() == 2);
    String str6{"hello"sv};
    REQUIRE(str6.length() == 5);
    String str7{TEXT("다국어"sv)};
    REQUIRE(str7.length() == 3);
    REQUIRE(str7[0] == TEXT('다'));
    REQUIRE(str7[1] == TEXT('국'));
    REQUIRE(str7[2] == TEXT('어'));
  }

  SECTION("UTF-16 decode valid sequences") {
    struct TestSuite {
      std::string endocing;
      std::vector<char16_t> expectedDecoding;
    };
    const std::vector<TestSuite> testVectors = {
        { "𣎴", { 0xD84C, 0xDFB4 } },
        { "日本語", { 0x65E5, 0x672C, 0x8A9E } },
    };

    for (const TestSuite& testVector : testVectors) {
      String actualDecoding = String::fromUTF8(testVector.endocing);
      for (int i = 0; i < actualDecoding.length(); ++i) {
        REQUIRE(testVector.expectedDecoding[i] == actualDecoding[i]);
      }
    }
  }
}
