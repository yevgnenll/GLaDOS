#include "StringUtils.h"

#include "platform/OSTypes.h"

namespace GameEngine {
  std::string StringUtils::normalize(const std::string& t) {
    return t;
  }
  std::string StringUtils::normalize(const char* t) {
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

  int StringUtils::toInt(const std::string& str) {
    return std::stoi(str);
  }

  long StringUtils::toLong(const std::string& str) {
    return std::stol(str);
  }

  std::pair<std::string, std::string> StringUtils::extractBaseFileNamePair(const std::string& fullPath) {
    const std::size_t lastPathSeparatorIndex = fullPath.find_last_of(PATH_SEPARATOR);
    if (std::string::npos != lastPathSeparatorIndex) {
      return std::make_pair(fullPath.substr(0, lastPathSeparatorIndex + 1), fullPath.substr(lastPathSeparatorIndex + 1));
    }

    return std::make_pair("", "");
  }
}  // namespace GameEngine