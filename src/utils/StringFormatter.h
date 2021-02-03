#ifndef GLADOS_STRINGFORMATTER_H
#define GLADOS_STRINGFORMATTER_H

#include <string>

#include "StringUtils.h"

namespace GLaDOS {
  class Argument {
  public:
    Argument() = default;
    virtual ~Argument() = default;
    virtual void append(std::string& dest) const = 0;
  };

  template <typename T>
  class ArgumentType : public Argument {
  public:
    explicit ArgumentType(T const& t) : mData(t){};
    void append(std::string& dest) const override {
      dest.append(StringUtils::normalize(mData));
    }

  private:
    T const& mData;
  };

  class StringFormatter {
  public:
    StringFormatter() = delete;
    template <typename... Args>
    static std::string formatTo(const std::string& fmt, Args&&... args);

  private:
    template <typename T>
    static ArgumentType<T> makeArgument(T const& t);
    template <typename Args = Argument, std::size_t Len>
    static std::string formatToImplInner(const std::string& fmt, Args const* (&arguments)[Len]);
    template <typename... Args>
    static std::string formatToImplOuter(const std::string& fmt, Args const&... args);
  };

  template <typename... Args>
  std::string StringFormatter::formatTo(const std::string& fmt, Args&&... args) {
    return formatToImplOuter(fmt, makeArgument(std::forward<Args>(args))...);
  }

  template <typename T>
  ArgumentType<T> StringFormatter::makeArgument(T const& t) {
    return static_cast<ArgumentType<T>>(t);
  }

  template <typename Args, std::size_t Len>
  std::string StringFormatter::formatToImplInner(const std::string& fmt, Args const* (&arguments)[Len]) {
    std::size_t format_len = fmt.size();
    std::string result;

    for (std::size_t i = 0; i != format_len; ++i) {
      switch (fmt[i]) {
        case '{': {
          std::size_t firstCurly = fmt.find_first_of('}', i);
          if (firstCurly >= format_len) {
            throw std::invalid_argument{"invalid format string"};
          }
          std::size_t index = static_cast<size_t>(StringUtils::toLong(fmt.substr(i + 1, firstCurly)));
          if (index >= Len || index < 0) {
            throw std::invalid_argument{"Too few arguments"};
          }
          arguments[index]->append(result);
          i += (firstCurly + 1) - (i + 1);
          break;
        }
        default:
          result.push_back(fmt[i]);
      }
    }

    return result;
  }

  template <typename... Args>
  std::string StringFormatter::formatToImplOuter(const std::string& fmt, Args const&... args) {
    Argument const* array[sizeof...(args)] = {static_cast<Argument const*>(&args)...};
    return formatToImplInner(fmt, array);
  }
}  // namespace GLaDOS

#endif
