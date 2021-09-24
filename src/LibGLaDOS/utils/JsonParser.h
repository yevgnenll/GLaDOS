#ifndef GLADOS_JSONPARSER_H
#define GLADOS_JSONPARSER_H

#include "JsonNode.h"

namespace GLaDOS {
    class JsonParser {
        typedef JsonNode::boolean boolean;
        typedef JsonNode::number number;
        typedef JsonNode::string string;
        typedef JsonNode::array array;
        typedef JsonNode::object object;

      public:
        JsonParser() = delete;
        ~JsonParser() = delete;
        static bool parse(JsonNode& value, const std::string& json, std::string& err);

      private:
        static bool expectToken(const char** token, JsonTokenType type);
        static bool makeErrMsg(const char* msg, std::string& err);
        static bool parseNumber(double* number, const char** token);
        static bool parseString(string& str, const char** token);
        static bool parseValue(JsonNode& value, const char** token, std::string& err);
        static bool parseObject(JsonNode& value, const char** token, std::string& err);
        static bool parseArray(JsonNode& value, const char** token, std::string& err);
    };
}  // namespace GLaDOS

#endif  //GLADOS_JSONPARSER_H
