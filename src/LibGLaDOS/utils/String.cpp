#include "String.hpp"

#include <codecvt>
#include <locale>

#include "math/Math.h"

namespace GLaDOS {
  String::String(const char* chars) {
    std::size_t len = charTypeLength(chars);
    copyCharType(chars, len);
  }

  String::String(const char* chars, std::size_t n) {
    std::size_t len = Math::min(charTypeLength(chars), n);
    copyCharType(chars, len);
  }

  String::String(const char16_t* chars) {
    std::size_t len = charTypeLength(chars);
    copyCharType(chars, len);
  }

  String::String(const char16_t* chars, std::size_t n) {
    std::size_t len = Math::min(charTypeLength(chars), n);
    copyCharType(chars, len);
  }

  String::String(const std::string& str) {
    copyCharType(str.c_str(), str.length());
  }

  String::String(const std::u16string& str) {
    copyCharType(str.c_str(), str.length());
  }

  String::String(const String& str) {
    copyCharType(str.c_str(), str.length());
  }

  String::String(std::string_view sv) {
    copyCharType(sv.data(), sv.size());
  }

  String::String(std::u16string_view sv) : UTF16String{sv.data(), sv.size()} {
  }

  String& String::operator=(const char* chars) {
    if (!copyCharType(chars, charTypeLength(chars))) {
      clear();
    }

    return *this;
  }

  String& String::operator=(const std::string& str) {
    if (!copyCharType(str.c_str(), str.length())) {
      clear();
    }

    return *this;
  }

  String& String::operator=(const String& str) {
    if (!copyCharType(str.c_str(), str.length())) {
      clear();
    }

    return *this;
  }

  std::u16string_view String::toStringView() {
    return std::u16string_view(*this);
  }

  String String::fromUTF8(std::string_view sv) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    String str = convert.from_bytes(sv.data(), sv.data() + sv.size());
    if (convert.converted() < sv.size()) {
      throw std::runtime_error("incomplete conversion");
    }
    return str;
  }

  std::string String::toUTF8(std::u16string_view sv) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    std::string str = convert.to_bytes(sv.data(), sv.data() + sv.size());
    if (convert.converted() < sv.size()) {
      throw std::runtime_error("incomplete conversion");
    }
    return str;
  }
}  // namespace GLaDOS