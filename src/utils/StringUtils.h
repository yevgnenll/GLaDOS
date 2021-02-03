#ifndef GLADOS_STRINGUTILS_H
#define GLADOS_STRINGUTILS_H

#include <string>

namespace GLaDOS {
  class StringUtils {
  public:
    StringUtils() = delete;
    template <typename T>
    static std::string normalize(const T& t);
    static std::string normalize(const std::string& t);
    static std::string normalize(const char* t);
    static std::string padRight(std::string const& str, std::size_t num, const char paddingChar = ' ');
    static std::string padLeft(std::string const& str, std::size_t num, const char paddingChar = ' ');
    static std::string toString(char c);
    static float toFloat(const std::string& str);
    static double toDouble(const std::string& str);
    static int toInt(const std::string& str, int base = 10);
    static long toLong(const std::string& str, int base = 10);
    static std::pair<std::string, std::string> extractBaseFileNamePair(const std::string& fullPath);
  };

  template <typename T>
  std::string StringUtils::normalize(const T& t) {
    return std::to_string(t);
  }
}  // namespace GLaDOS

#endif
