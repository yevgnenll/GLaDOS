#ifndef GLADOS_STRINGUTILS_H
#define GLADOS_STRINGUTILS_H

#include <string>
#include <sstream>
#include <vector>

namespace GLaDOS {
    class String;
    class StringUtils {
      public:
        StringUtils() = delete;
        template <typename T>
        static std::string normalize(const T& t);
        static std::string normalize(const std::string& t);
        static std::string normalize(const String& t);
        static std::string normalize(const char* t);
        static std::string normalize(char* t);

        static std::string padRight(std::string const& str, std::size_t num, const char paddingChar = ' ');
        static std::string padLeft(std::string const& str, std::size_t num, const char paddingChar = ' ');

        static std::string toString(char c);
        static float toFloat(const std::string& str);
        static double toDouble(const std::string& str);
        static int toInt(const std::string& str, int base = 10);
        static long toLong(const std::string& str, int base = 10);
        static float toFloat(const String& str);
        static double toDouble(const String& str);
        static int toInt(const String& str, int base = 10);
        static long toLong(const String& str, int base = 10);

        template <typename Iter>
        static std::string join(const std::string& delimiter, Iter start, Iter end);
        template <typename Iter>
        static std::string join(const std::string& delimiter, const std::string& prefix, const std::string& suffix, Iter start, Iter end);

        static bool contains(const std::string& str, const std::string& substr);
        static std::size_t split(const std::string& str, std::vector<std::string>& slices, const std::string& delim);
        static bool equalsIgnoreCase(const std::string& strA, const std::string& strB);
        static std::string replaceAll(std::string str, const std::string& old_str, const std::string& new_str);
        static std::string replace(std::string str, const std::string& old_str, const std::string& new_str, int n = 1);
        static bool startsWith(const std::string& str, const std::string& prefix);
        static bool endsWith(const std::string& str, const std::string& suffix);
        static std::string trimRight(std::string str);
        static std::string trimLeft(std::string str);
        static std::string trim(const std::string& str);
        static std::string toLower(std::string str);
        static std::string toUpper(std::string str);
        static int index(const std::string& str, const std::string& substr);
        static int lastIndex(const std::string& str, const std::string& substr);
        static std::string repeat(const std::string& str, int count);
        static int count(const std::string& str, const std::string& substr, bool ignore_case = false);

        static std::pair<std::string, std::string> splitFileName(const std::string& fullPath);
    };

    template <typename T>
    std::string StringUtils::normalize(const T& t) {
        return std::to_string(t);
    }

    template <typename Iter>
    std::string StringUtils::join(const std::string& delimiter, Iter start, Iter end) {
        std::stringstream ss;
        for (Iter it = start; it != end; it++) {
            if (&*it != &*start) {
                ss << delimiter;
            }
            ss << *it;
        }

        return ss.str();
    }

    template <typename Iter>
    std::string StringUtils::join(const std::string& delimiter, const std::string& prefix, const std::string& suffix, Iter start, Iter end) {
        std::string result = prefix;
        return result.append(join(delimiter, start, end)).append(suffix);
    }
}  // namespace GLaDOS

#endif
