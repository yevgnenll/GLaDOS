#include <benchmark/benchmark.h>
#include "utils/JsonParser.h"

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

static void BM_JsonParserDeserialize(benchmark::State& state) {
  for (auto _ : state) {
    JsonNode root;
    std::string errMessage;
    JsonParser::parse(root, simpleJson, errMessage);
  }
}

BENCHMARK(BM_JsonParserDeserialize);