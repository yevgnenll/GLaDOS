#include "JsonParser.h"

#include "math/Math.h"

namespace GLaDOS {
    bool JsonParser::parse(JsonNode& value, const std::string& json, std::string& err) {
        const char* token = json.c_str();
        err.clear();

        // RFC 4627: only objects or arrays were allowed as root
        if (expectToken(&token, JsonTokenType::StartObject)) {
            if (!parseObject(value, &token, err)) return false;
        } else if (expectToken(&token, JsonTokenType::StartArray)) {
            if (!parseArray(value, &token, err)) return false;
        } else {
            return makeErrMsg("invalid or empty json.", err);
        }

        return true;
    }

    bool JsonParser::expectToken(const char** token, JsonTokenType type) {
        (*token) += strspn((*token), " \t\n\r");
        if ((JsonTokenType)(*token)[0] == type) {
            (*token)++;
            return true;
        }

        return false;
    }

    bool JsonParser::makeErrMsg(const char* msg, std::string& err) {
        char buf[64];
        snprintf(buf, sizeof(buf), "%s", msg);
        err = buf;
        return false;
    }

    bool JsonParser::parseNumber(double* number, const char** token) {
        (*token) += strspn((*token), " \t");
        const char* end = (*token) + strcspn((*token), " \t,\n\r}]");
        if (end != (*token)) {
            double value;
            if (!Math::atod((*token), end, &value)) return false;
            (*number) = value;
            (*token) = end;
            return true;
        }

        return false;
    }

    bool JsonParser::parseString(string& str, const char** token) {
        // skip "
        if ((JsonTokenType)(*token)[0] == JsonTokenType::DoubleQuote) (*token)++;
        const char* end = (*token) + strcspn((*token), "\"");
        str.clear();
        if ((*token) != end) {
            str.assign((*token), static_cast<unsigned long>(end - (*token)));
            (*token) = ++end;
            return true;
        }

        (*token) = ++end;
        return false;
    }

    bool JsonParser::parseValue(JsonNode& value, const char** token, std::string& err) {
        if ((JsonTokenType)(*token)[0] == JsonTokenType::DoubleQuote) {
            // string
            string str_value;
            // allow empty string
            parseString(str_value, token);
            value.set(str_value);
        } else if (((*token)[0] == 't') && (0 == strncmp((*token), "true", 4))) {
            // boolean true
            value.set(true);
            (*token) += 4;
        } else if (((*token)[0] == 'f') && (0 == strncmp((*token), "false", 5))) {
            // boolean false
            value.set(false);
            (*token) += 5;
        } else if (((*token)[0] == 'n') && (0 == strncmp((*token), "null", 4))) {
            // null
            (*token) += 4;
        } else {
            // number
            double number = 0.0;
            if (!parseNumber(&number, token)) {
                return makeErrMsg("parse error.", err);
            }
            value.set(number);
        }

        return true;
    }

    bool JsonParser::parseObject(JsonNode& value, const char** token, std::string& err) {
        string current_key;
        object* root = new object();

        // empty object
        if (expectToken(token, JsonTokenType::EndObject)) {
            value.set(root);
            return true;
        }

        do {
            if (!expectToken(token, JsonTokenType::DoubleQuote) || !parseString(current_key, token) || !expectToken(token, JsonTokenType::Colon)) {
                return makeErrMsg("invalid token.", err);
            }

            JsonNode* current_value = new JsonNode();
            if (expectToken(token, JsonTokenType::StartObject)) {
                if (!parseObject(*current_value, token, err)) return false;
            } else if (expectToken(token, JsonTokenType::StartArray)) {
                if (!parseArray(*current_value, token, err)) return false;
            } else {
                if (!parseValue(*current_value, token, err)) return false;
            }
            root->insert(std::make_pair(current_key, current_value));
        } while (expectToken(token, JsonTokenType::Comma));

        if (!expectToken(token, JsonTokenType::EndObject)) {
            return makeErrMsg("invalid end of object.", err);
        }

        value.set(root);
        return true;
    }

    bool JsonParser::parseArray(JsonNode& value, const char** token, std::string& err) {
        array* root = new array();

        // empty array
        if (expectToken(token, JsonTokenType::EndArray)) {
            value.set(root);
            return true;
        }

        do {
            JsonNode* current_value = new JsonNode();
            if (expectToken(token, JsonTokenType::StartObject)) {
                if (!parseObject(*current_value, token, err)) return false;
            } else if (expectToken(token, JsonTokenType::StartArray)) {
                if (!parseArray(*current_value, token, err)) return false;
            } else {
                if (!parseValue(*current_value, token, err)) return false;
            }
            root->emplace_back(current_value);
        } while (expectToken(token, JsonTokenType::Comma));

        if (!expectToken(token, JsonTokenType::EndArray)) {
            return makeErrMsg("invalid end of array.", err);
        }

        value.set(root);
        return true;
    }
}  // namespace GLaDOS