#include <catch2/catch_all.hpp>
#include "utils/RefPtr.hpp"

using namespace GLaDOS;

class TestSuiteObject : public RefCounted {
public:
  TestSuiteObject(const std::string& n) : name{n} {}
  std::string getname() const { return name; }

private:
  std::string name;
};

TEST_CASE("RefPtr unit tests", "[RefPtr]") {
  SECTION("RefPtr initialization test") {
    RefPtr<TestSuiteObject> pTestSuite(NEW_T(TestSuiteObject("testme")));
    REQUIRE(pTestSuite->getname() == "testme");
    REQUIRE(pTestSuite->isRefOne() == true);
  }
}
