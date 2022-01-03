#include "StringUtils.h"

#include "platform/OSTypes.h"
#include "String.hpp"

namespace GLaDOS {
    std::string StringUtils::normalize(const std::string& t) {
        return t;
    }

    std::string StringUtils::normalize(const String& t) {
        return String::toUTF8(t);
    }

    std::string StringUtils::normalize(const char* t) {
        return std::string{t};
    }

    std::string StringUtils::normalize(char* t) {
        return std::string{t};
    }

    std::string StringUtils::padRight(std::string const& str, std::size_t num, const char paddingChar) {
        if (str.size() < num) {
            return str + std::string(num - str.size(), paddingChar);
        }
        return str;
    }

    std::string StringUtils::padLeft(std::string const& str, std::size_t num, const char paddingChar) {
        if (str.size() < num) {
            return std::string(num - str.size(), paddingChar) + str;
        }
        return str;
    }

    std::string StringUtils::toString(char c) {
        return std::string(1, c);
    }

    float StringUtils::toFloat(const std::string& str) {
        return std::stof(str);
    }

    double StringUtils::toDouble(const std::string& str) {
        return std::stod(str);
    }

    int StringUtils::toInt(const std::string& str, int base) {
        return std::stoi(str, 0, base);
    }

    long StringUtils::toLong(const std::string& str, int base) {
        return std::stol(str, 0, base);
    }

    float StringUtils::toFloat(const String& str) {
        return StringUtils::toFloat(String::toUTF8(str));
    }

    double StringUtils::toDouble(const String& str) {
        return StringUtils::toDouble(String::toUTF8(str));
    }

    int StringUtils::toInt(const String& str, int base) {
        return StringUtils::toInt(String::toUTF8(str), base);
    }

    long StringUtils::toLong(const String& str, int base) {
        return StringUtils::toLong(String::toUTF8(str), base);
    }

    bool StringUtils::contains(const std::string& str, const std::string& substr) {
        return str.find(substr) != std::string::npos;
    }

    std::size_t StringUtils::split(const std::string& str, std::vector<std::string>& slices, const std::string& delim) {
        std::size_t pos = str.find(delim);
        std::size_t initialPos = 0;
        slices.clear();

        // Decompose statement
        while (pos != std::string::npos) {
            slices.emplace_back(str.substr(initialPos, pos - initialPos));
            initialPos = pos + 1;

            pos = str.find(delim, initialPos);
        }

        // Add the last one
        slices.emplace_back(str.substr(initialPos, std::min(pos, str.size()) - initialPos + 1));

        return slices.size();
    }

    bool StringUtils::equalsIgnoreCase(const std::string& strA, const std::string& strB) {
        std::size_t length = strA.size();
        if (strB.size() != length) {
            return false;
        }

        for (std::size_t i = 0; i < length; i++) {
            if (std::tolower(strA[i]) != std::tolower(strB[i])) {
                return false;
            }
        }

        return true;
    }

    std::string StringUtils::replaceAll(std::string str, const std::string& old_str, const std::string& new_str) {
        std::string::size_type pos;
        std::string::size_type offset = 0;

        while ((pos = str.find(old_str, offset)) != std::string::npos) {
            str.replace(str.begin() + pos, str.begin() + pos + old_str.length(), new_str);
            offset = pos + new_str.length();
        }

        return str;
    }

    std::string StringUtils::replace(std::string str, const std::string& old_str, const std::string& new_str, int n) {
        if (n < 0) {
            return replaceAll(std::move(str), old_str, new_str);
        }

        std::string::size_type pos;
        std::string::size_type offset = 0;

        while (n-- > 0) {
            pos = str.find(old_str, offset);
            if (pos != std::string::npos) {
                str.replace(str.begin() + pos, str.begin() + pos + old_str.length(), new_str);
                offset = pos + new_str.length();
            }
        }

        return str;
    }

    bool StringUtils::startsWith(const std::string& str, const std::string& prefix) {
        return (str.length() >= prefix.length() && str.compare(0, prefix.length(), prefix) == 0);
    }

    bool StringUtils::endsWith(const std::string& str, const std::string& suffix) {
        return (str.length() >= suffix.length() && str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0);
    }

    std::string StringUtils::trimRight(std::string str) {
        str.erase(std::find_if(str.rbegin(), str.rend(), [](int chars) { return std::isspace(chars) == 0; }).base(), str.end());
        return str;
    }

    std::string StringUtils::trimLeft(std::string str) {
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int chars) { return std::isspace(chars) == 0; }));
        return str;
    }

    std::string StringUtils::trim(const std::string& str) {
        return trimLeft(trimRight(str));
    }

    std::string StringUtils::toLower(std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), tolower);
        return str;
    }

    std::string StringUtils::toUpper(std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), toupper);
        return str;
    }

    int StringUtils::index(const std::string& str, const std::string& substr) {
        std::string::size_type pos = str.find(substr);
        return (pos != std::string::npos) ? pos : -1;
    }

    int StringUtils::lastIndex(const std::string& str, const std::string& substr) {
        std::string::size_type pos = str.rfind(substr);
        return (pos != std::string::npos) ? pos : -1;
    }

    std::string StringUtils::repeat(const std::string& str, int count) {
        if (count < 0) {
            throw std::out_of_range("count is negative");
        }

        std::string copy;
        while ((count--) != 0) {
            copy.append(str);
        }
        return copy;
    }

    int StringUtils::count(const std::string& str, const std::string& substr, bool ignore_case) {
        if (!substr.empty()) {
            int n = 0;
            std::string::size_type pos = 0;
            std::string_view hay_stack;
            std::string_view needle;

            if (ignore_case) {
                hay_stack = toUpper(str);
                needle = toUpper(substr);
            } else {
                hay_stack = str;
                needle = substr;
            }

            while ((pos = hay_stack.find(needle, pos)) != std::string::npos) {
                n++;
                pos += needle.length();
            }
            return n;
        }

        return str.length() + 1;
    }

    std::pair<std::string, std::string> StringUtils::splitFileName(const std::string& fullPath) {
        const std::size_t lastPathSeparatorIndex = fullPath.find_last_of(PATH_SEPARATOR);
        if (std::string::npos != lastPathSeparatorIndex) {
            return std::make_pair(fullPath.substr(0, lastPathSeparatorIndex + 1), fullPath.substr(lastPathSeparatorIndex + 1));
        }

        return std::make_pair("", "");
    }
}  // namespace GLaDOS