#include <catch2/catch.hpp>

#include "utils/JsonParser.h"
#include "math/Math.h"

using namespace GLaDOS;

std::string simpleJson = R"(
{
  "company": "Google.inc",
  "number": 123,
  "name": "Robert.Jr",
  "phone_number": "(415) 521-2389",
  "under_graduate": true,
  "absurd" :
    "value", "double_value": 99.223, "null_value": null,
  "is_married": false, "empty": [

], "empty_obj": {}, "fill_object": { "name": "hello", "age": "10" }
}
)";

TEST_CASE("JsonParser unit tests", "[JsonParser]") {
  SECTION("JsonParser basic deserialize") {
    JsonNode root;
    std::string errMessage;
    bool result = JsonParser::parse(root, simpleJson, errMessage);
    REQUIRE(result == true);
    REQUIRE(root.getNode("company").getString() == "Google.inc");
    REQUIRE(Math::equal(root.getNode("number").getNumber(), static_cast<GLaDOS::JsonNode::number>(123)));
    REQUIRE(root.getNode("name").getString() == "Robert.Jr");
    REQUIRE(root.getNode("phone_number").getString() == "(415) 521-2389");
    REQUIRE(root.getNode("under_graduate").getBoolean() == true);
    REQUIRE(root.getNode("absurd").getString() == "value");
    REQUIRE(Math::equal(root.getNode("double_value").getNumber(), 99.223));
    REQUIRE(root.getNode("null_value").isNull());
    REQUIRE(root.getNode("is_married").getBoolean() == false);
    REQUIRE(root.getNode("empty").getArray().empty());
    REQUIRE(root.getNode("empty_obj").getObject().empty());
    GLaDOS::JsonNode::object obj = root.getNode("fill_object").getObject();
    REQUIRE(!obj.empty());
    std::array<std::pair<std::string, std::string>, 2> mock = {
        std::make_pair("name", "hello"),
        std::make_pair("age", "10"),
    };
    int idx = 0;
    for (auto node : obj) {
      REQUIRE(node.first == mock[idx].first);
      REQUIRE(node.second->getString() == mock[idx].second);
      idx++;
    }
  }
}