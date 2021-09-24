#ifndef GLADOS_STRING_HPP
#define GLADOS_STRING_HPP

#include <string>
#include <string_view>

#include "memory/STLAllocator.h"

namespace GLaDOS {
    // 2 byte fixed sized UTF-16 String class
    typedef std::basic_string<char16_t, std::char_traits<char16_t>, STLAllocator<char16_t>> UTF16String;
    class String : public UTF16String {
      public:
        String() = default;
        String(const char* chars);
        String(const char* chars, std::size_t n);
        String(const char16_t* chars);
        String(const char16_t* chars, std::size_t n);
        String(const std::string& str);
        String(const std::u16string& str);
        String(const String& str);
        String(std::string_view sv);
        String(std::u16string_view sv);

        String& operator=(const char* chars);
        String& operator=(const std::string& str);
        String& operator=(const String& str);

        std::u16string_view toStringView();

        static String fromUTF8(std::string_view sv);
        static std::string toUTF8(std::u16string_view str);

      private:
        template <typename T>
        std::size_t charTypeLength(T* chars) const;
        template <typename T>
        bool copyCharType(T* chars, std::size_t len);
    };

    template <typename T>
    std::size_t String::charTypeLength(T* chars) const {
        return std::char_traits<T>::length(chars);
    }

    template <typename T>
    bool String::copyCharType(T* chars, std::size_t len) {
        if (len > 0) {
            resize(len);

            for (std::size_t i = 0; i < len; i++) {
                at(i) = *(chars + i);
            }
            return true;
        }

        return false;
    }
}  // namespace GLaDOS

#endif  //GLADOS_STRING_HPP
