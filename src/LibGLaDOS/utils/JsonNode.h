#ifndef GLADOS_JSONNODE_H
#define GLADOS_JSONNODE_H

#include "utils/Enumeration.h"
#include "utils/LinkedHashMap.hpp"
#include "utils/Stl.h"

namespace GLaDOS {
    class JsonNode {
        friend class JsonParser;

      public:
        typedef bool boolean;
        typedef double number;
        typedef std::string string;
        typedef Vector<JsonNode*> array;
        typedef LinkedHashMap<string, JsonNode*> object;
        union Storage {
            boolean bool_val;
            number num_val;
            string* str_val;
            array* array_val;
            object* object_val;
        };

        JsonNode();
        JsonNode(const JsonNode& other);
        explicit JsonNode(boolean val);
        explicit JsonNode(number val);
        explicit JsonNode(const string& val);
        explicit JsonNode(const char* val);
        explicit JsonNode(const array& val);
        explicit JsonNode(const object& val);
        ~JsonNode();

        JsonNode& getNode(const string& key);
        const JsonNode& getNode(const string& key) const;
        JsonNode& getElement(const std::size_t index);
        const JsonNode& getElement(const std::size_t index) const;
        boolean getBoolean() const;
        number getNumber() const;
        string& getString() const;
        array& getArray() const;
        object& getObject() const;

        JsonNode& operator[](std::size_t index);
        const JsonNode& operator[](std::size_t index) const;
        JsonNode& operator[](const string& key);
        const JsonNode& operator[](const string& key) const;

        bool has(const string& key) const;
        std::size_t length() const;
        std::string toString() const;
        bool toBoolean() const;
        std::string serialize(bool prettify = false, unsigned int indent_size = 2) const;

        JsonNode& operator=(const JsonNode& other);
        JsonNode& operator=(const boolean other);
        JsonNode& operator=(const double other);
        JsonNode& operator=(const int other);
        JsonNode& operator=(const string& other);
        JsonNode& operator=(const char* other);
        JsonNode& operator=(const array& other);
        JsonNode& operator=(const object& other);
        bool operator==(const JsonNode& other) const;
        bool operator!=(const JsonNode& other) const;
        bool operator==(const string& other) const;
        bool operator!=(const string& other) const;
        bool operator==(double other) const;
        bool operator!=(double other) const;
        bool operator==(int other) const;
        bool operator!=(int other) const;

        bool isNull() const;
        bool isBoolean() const;
        bool isNumber() const;
        bool isString() const;
        bool isArray() const;
        bool isObject() const;

      private:
        void clear();
        void set(boolean val);
        void set(number val);
        void set(const string& val);
        void set(const char* val);
        void set(const array& val);
        void set(const object& val);
        void set(string* val);
        void set(array* val);
        void set(object* val);
        void makeIndent(int indent, std::back_insert_iterator<string>& iter, unsigned int indent_size) const;
        void serializeStr(const string& str, std::back_insert_iterator<string>& iter) const;
        void internalSerialize(int indent, std::back_insert_iterator<string>& iter, unsigned int indent_size) const;

        Storage storage;
        JsonNodeType type;
    };
}  // namespace GLaDOS

#endif  //GLADOS_JSONNODE_H
