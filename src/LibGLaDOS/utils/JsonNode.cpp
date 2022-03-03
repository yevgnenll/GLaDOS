#include "JsonNode.h"

#include "math/Math.h"

namespace GLaDOS {
    JsonNode::JsonNode() : storage(), type(JsonNodeType::Null) {}

    JsonNode::JsonNode(const JsonNode& other) : storage(), type() { *this = other; }

    JsonNode::JsonNode(boolean val) : storage(), type(JsonNodeType::Boolean) { storage.bool_val = val; }

    JsonNode::JsonNode(number val) : storage(), type(JsonNodeType::Number) { storage.num_val = val; }

    JsonNode::JsonNode(const string& val) : storage(), type(JsonNodeType::String) { storage.str_val = new string(val); }

    JsonNode::JsonNode(const char* val) : storage(), type(JsonNodeType::String) { storage.str_val = new string(val); }

    JsonNode::JsonNode(const array& val) : storage(), type() { set(val); }

    JsonNode::JsonNode(const object& val) : storage(), type() { set(val); }

    JsonNode::~JsonNode() {
        clear();
    }

    JsonNode& JsonNode::getNode(const string& key) {
        static JsonNode null_node;
        if (!isObject()) return null_node;
        object::iterator iter = storage.object_val->find(key);
        return iter != storage.object_val->end() ? *(iter->second) : null_node;
    }

    const JsonNode& JsonNode::getNode(const string& key) const {
        static const JsonNode null_node;
        if (!isObject()) return null_node;
        JsonNode::object::const_iterator citer = storage.object_val->find(key);
        return citer != storage.object_val->cend() ? *(citer->second) : null_node;
    }

    JsonNode& JsonNode::getElement(const std::size_t index) {
        static JsonNode null_node;
        if (!isArray()) return null_node;
        return index < storage.array_val->size() ? *(*storage.array_val)[index] : null_node;
    }

    const JsonNode& JsonNode::getElement(const std::size_t index) const {
        static const JsonNode null_node;
        if (!isArray()) return null_node;
        return index < storage.array_val->size() ? *(*storage.array_val)[index] : null_node;
    }

    JsonNode::boolean JsonNode::getBoolean() const {
        GASSERT(isBoolean());
        return storage.bool_val;
    }

    JsonNode::number JsonNode::getNumber() const {
        GASSERT(isNumber());
        return storage.num_val;
    }

    JsonNode::string& JsonNode::getString() const {
        GASSERT(isString());
        return *(storage.str_val);
    }

    JsonNode::array& JsonNode::getArray() const {
        GASSERT(isArray());
        return *(storage.array_val);
    }

    JsonNode::object& JsonNode::getObject() const {
        GASSERT(isObject());
        return *(storage.object_val);
    }

    JsonNode& JsonNode::operator[](std::size_t index) { return this->getElement(index); }

    const JsonNode& JsonNode::operator[](std::size_t index) const { return this->getElement(index); }

    JsonNode& JsonNode::operator[](const string& key) { return this->getNode(key); }

    const JsonNode& JsonNode::operator[](const string& key) const { return this->getNode(key); }

    bool JsonNode::has(const string& key) const {
        if (!isObject()) return false;
        return storage.object_val->find(key) != storage.object_val->cend();
    }

    std::size_t JsonNode::length() const {
        switch (type) {
            case JsonNodeType::String:
                return storage.str_val->size();
            case JsonNodeType::Array:
                return storage.array_val->size();
            case JsonNodeType::Object:
                return storage.object_val->size();
            default:
                return 0;
        }
    }

    std::string JsonNode::toString() const {
        switch (type) {
            case JsonNodeType::String:
                return "string";
            case JsonNodeType::Object:
                return "object";
            case JsonNodeType::Array:
                return "array";
            case JsonNodeType::Null:
                return "null";
            case JsonNodeType::Number:
                return "number";
            case JsonNodeType::Boolean:
                return "boolean";
        }
    }

    bool JsonNode::toBoolean() const {
        switch (type) {
            case JsonNodeType::Null:
                return false;
            case JsonNodeType::Boolean:
                return storage.bool_val;
            case JsonNodeType::Number:
                return storage.num_val != 0;
            case JsonNodeType::String:
                return !storage.str_val->empty();
            default:
                return true;
        }
    }

    std::string JsonNode::serialize(bool prettify, unsigned int indent_size) const {
        std::string s;
        std::back_insert_iterator<std::string> iter = std::back_inserter(s);
        internalSerialize(prettify ? 0 : -1, iter, indent_size);
        return s;
    }

    JsonNode& JsonNode::operator=(const JsonNode& other) {
        if (this != &other) {
            clear();

            switch (other.type) {
                case JsonNodeType::String:
                    set(*other.storage.str_val);
                    break;
                case JsonNodeType::Array:
                    set(*other.storage.array_val);
                    break;
                case JsonNodeType::Object:
                    set(*other.storage.object_val);
                    break;
                default:
                    type = other.type;
                    storage = other.storage;
                    break;
            }
        }

        return *this;
    }

    JsonNode& JsonNode::operator=(const boolean other) {
        clear();
        set(other);
        return *this;
    }

    JsonNode& JsonNode::operator=(const double other) {
        clear();
        set(other);
        return *this;
    }

    JsonNode& JsonNode::operator=(const int other) {
        clear();
        set((number)other);
        return *this;
    }

    JsonNode& JsonNode::operator=(const string& other) {
        clear();
        set(other);
        return *this;
    }

    JsonNode& JsonNode::operator=(const char* other) {
        clear();
        set(other);
        return *this;
    }

    JsonNode& JsonNode::operator=(const array& other) {
        clear();
        set(other);
        return *this;
    }

    JsonNode& JsonNode::operator=(const object& other) {
        clear();
        set(other);
        return *this;
    }

    bool JsonNode::operator==(const JsonNode& other) const {
        if (type != other.type) {
            return false;
        }

        switch (type) {
            case JsonNodeType::String:
                return *(storage.str_val) == *(other.storage.str_val);
            case JsonNodeType::Number:
                return Math::equal(storage.num_val, CAST(number, other.storage.num_val));
            case JsonNodeType::Boolean:
                return storage.bool_val == other.storage.bool_val;
            case JsonNodeType::Object: {
                const object* l = this->storage.object_val;
                const object* r = other.storage.object_val;
                return (l->size() == r->size()) && std::equal(l->cbegin(), l->cend(), r->cbegin(),
                                                              [](const auto& left, const auto& right) {
                                                                  return (left.first == right.first) && (left.second == right.second);
                                                              });
            }
            case JsonNodeType::Array: {
                const array* l = this->storage.array_val;
                const array* r = other.storage.array_val;
                return (l->size() == r->size()) && std::equal(l->cbegin(), l->cend(), r->cbegin(),
                                                              [](const auto& left, const auto& right) {
                                                                  return (left == right);
                                                              });
            }
            default:
                return true;
        }
    }

    bool JsonNode::operator!=(const JsonNode& other) const {
        return !(*this == other);
    }

    bool JsonNode::operator==(const string& other) const {
        if (type != JsonNodeType::String) {
            return false;
        }

        return *(storage.str_val) == other;
    }

    bool JsonNode::operator!=(const string& other) const {
        return !(*this == other);
    }

    bool JsonNode::operator==(double other) const {
        if (type != JsonNodeType::Number) {
            return false;
        }

        return Math::equal(storage.num_val, CAST(number, other));
    }

    bool JsonNode::operator!=(double other) const {
        return !(*this == other);
    }

    bool JsonNode::operator==(int other) const {
        if (type != JsonNodeType::Number) {
            return false;
        }

        return Math::equal(storage.num_val, CAST(number, other));
    }

    bool JsonNode::operator!=(int other) const {
        return !(*this == other);
    }

    bool JsonNode::isNull() const { return type == JsonNodeType::Null; }

    bool JsonNode::isBoolean() const { return type == JsonNodeType::Boolean; }

    bool JsonNode::isNumber() const { return type == JsonNodeType::Number; }

    bool JsonNode::isString() const { return type == JsonNodeType::String; }

    bool JsonNode::isArray() const { return type == JsonNodeType::Array; }

    bool JsonNode::isObject() const { return type == JsonNodeType::Object; }

    void JsonNode::clear() {
        switch (type) {
            case JsonNodeType::String:
                delete storage.str_val;
                break;
            case JsonNodeType::Array:
                for (JsonNode* elem : *(storage.array_val)) {
                    delete elem;
                }
                delete storage.array_val;
                break;
            case JsonNodeType::Object:
                for (const auto& elem : *(storage.object_val)) {
                    delete elem.second;
                }
                delete storage.object_val;
                break;
            default:
                break;
        }
    }

    void JsonNode::set(boolean val) {
        type = JsonNodeType::Boolean;
        storage.bool_val = val;
    }

    void JsonNode::set(number val) {
        type = JsonNodeType::Number;
        storage.num_val = val;
    }

    void JsonNode::set(const string& val) {
        type = JsonNodeType::String;
        storage.str_val = new string(val);
    }

    void JsonNode::set(const char* val) {
        type = JsonNodeType::String;
        storage.str_val = new string(val);
    }

    void JsonNode::set(const array& val) {
        type = JsonNodeType::Array;
        storage.array_val = new array();
        storage.array_val->reserve(val.size());
        // deep copy
        for (auto e : val) {
            storage.array_val->emplace_back(new JsonNode(*e));
        }
    }

    void JsonNode::set(const object& val) {
        type = JsonNodeType::Object;

        storage.object_val = new object(val.size());
        // deep copy
        auto begin = val.cbegin();
        auto end = val.cend();
        for (; begin != end; ++begin) {
            storage.object_val->insert(std::make_pair(begin->first, new JsonNode(*(begin->second))));
        }
    }

    void JsonNode::set(string* val) {
        type = JsonNodeType::String;
        storage.str_val = val;
    }

    void JsonNode::set(array* val) {
        type = JsonNodeType::Array;
        storage.array_val = val;
    }

    void JsonNode::set(object* val) {
        type = JsonNodeType::Object;
        storage.object_val = val;
    }

    void JsonNode::makeIndent(int indent, std::back_insert_iterator<string>& iter, unsigned int indent_size) const {
        size_t size = indent * indent_size;
        iter++ = '\n';
        for (int i = 0; i < size; ++i) {
            iter++ = ' ';
        }
    }

    void JsonNode::serializeStr(const string& str, std::back_insert_iterator<string>& iter) const {
        iter++ = '\"';
        std::copy(str.begin(), str.end(), iter);
        iter++ = '\"';
    }

    void JsonNode::internalSerialize(int indent, std::back_insert_iterator<string>& iter, unsigned int indent_size) const {
        switch (type) {
            case JsonNodeType::String:
                serializeStr(*(storage.str_val), iter);
                break;
            case JsonNodeType::Object: {
                iter++ = '{';
                if (indent != -1) {
                    ++indent;
                }
                auto cbegin = storage.object_val->cbegin();
                auto cend = storage.object_val->cend();
                for (auto citer = cbegin; citer != cend; ++citer) {
                    if (citer != cbegin) {
                        iter++ = ',';
                    }
                    if (indent != -1) {
                        makeIndent(indent, iter, indent_size);
                    }
                    serializeStr(citer->first, iter);
                    iter++ = ':';
                    if (indent != -1) {
                        iter++ = ' ';
                    }
                    citer->second->internalSerialize(indent, iter, indent_size);
                }
                if (indent != -1) {
                    --indent;
                    if (!storage.object_val->empty()) {
                        makeIndent(indent, iter, indent_size);
                    }
                }
                iter++ = '}';
                break;
            }
            case JsonNodeType::Array: {
                iter++ = '[';
                if (indent != -1) {
                    ++indent;
                }
                auto cbegin = storage.array_val->cbegin();
                auto cend = storage.array_val->cend();
                for (auto citer = cbegin; citer != cend; ++citer) {
                    if (citer != cbegin) {
                        iter++ = ',';
                    }
                    if (indent != -1) {
                        makeIndent(indent, iter, indent_size);
                    }
                    (*citer)->internalSerialize(indent, iter, indent_size);
                }
                if (indent != -1) {
                    --indent;
                    if (!storage.array_val->empty()) {
                        makeIndent(indent, iter, indent_size);
                    }
                }
                iter++ = ']';
                break;
            }
            case JsonNodeType::Null: {
                static const char* n = "null";
                std::copy(n, n + 4, iter);
                break;
            }
            case JsonNodeType::Number: {
                char buf[Math::jsonMaxNumberStringSize];
                const char* c = Math::dtoa(buf, storage.num_val);
                std::copy(c, c + strlen(c), iter);
                break;
            }
            case JsonNodeType::Boolean: {
                static const char* t = "true";
                static const char* f = "false";
                if (storage.bool_val) {
                    std::copy(t, t + 4, iter);
                } else {
                    std::copy(f, f + 5, iter);
                }
                break;
            }
        }
    }
}  // namespace GLaDOS