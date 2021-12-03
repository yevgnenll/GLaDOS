#ifndef GLADOS_UTILITY_H
#define GLADOS_UTILITY_H

#include "Logger.hpp"
#include "LoggerRegistry.h"
#include "Stl.h"
#include "memory/Allocation.h"
#include "platform/OSTypes.h"

namespace GLaDOS {
    typedef unsigned long long uint64;
    typedef unsigned int uint32;
    typedef unsigned short uint16;
    typedef unsigned char uint8;
    typedef long long int64;
    typedef int int32;
    typedef short int16;
    typedef char int8;
    typedef int32 bool32;
    typedef float float32;
    typedef double float64;

#ifdef PLATFORM_WINDOW
#define ANSI_COLOR_RED ""
#define ANSI_COLOR_GREEN ""
#define ANSI_COLOR_BLUE ""
#define ANSI_COLOR_MAGENTA ""
#define ANSI_COLOR_CYAN ""
#define ANSI_COLOR_GREY ""
#define ANSI_COLOR_YELLOW ""
#define ANSI_COLOR_RESET ""
#else
#define ANSI_COLOR_RED "\x1b[31;1m"
#define ANSI_COLOR_GREEN "\x1b[32;1m"
#define ANSI_COLOR_BLUE "\x1b[34;1m"
#define ANSI_COLOR_MAGENTA "\x1b[35;1m"
#define ANSI_COLOR_CYAN "\x1b[36;1m"
#define ANSI_COLOR_GREY "\x1b[30;1m"
#define ANSI_COLOR_YELLOW "\x1b[33;1m"
#define ANSI_COLOR_RESET "\x1b[0m"
#endif

#ifndef GTEXT
#define GTEXT(str) (u##str)
#endif

#ifndef CAST
#define CAST(type, x) (static_cast<type>(x))
#endif

#if defined(MSVC)
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE inline __attribute__((always_inline))
#endif

#ifndef GASSERT
#include <cassert>
#define GASSERT(_EXPR) assert(_EXPR)
#endif

/*
* If you declare a copy constructor (even if you define it as deleted in the declaration),
* no move constructor will be declared implicitly. Cf. C++11 12.8/9:
*/
#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(c) \
    c(const c&) = delete; \
    c& operator=(const c&) = delete
#endif

#ifndef DISALLOW_MOVE_AND_ASSIGN
#define DISALLOW_MOVE_AND_ASSIGN(c) \
    c(c&&) = delete; \
    c& operator=(c&&) = delete
#endif

#ifndef UNUSED
#if defined(MSVC)
#define UNUSED(x) (__pragma(warning(suppress : 4100))(x))
#elif defined(GCC)
#define UNUSED(x) __attribute__((__unused__)) (x)
#else
// fallback
#define UNUSED(x) ((void)(sizeof(x)))
#endif
#endif

#ifndef LOG_TRACE
#define LOG_TRACE(logger, fmt, ...) logger->trace({__FILE__, __LINE__, __FUNCTION__}, fmt, ##__VA_ARGS__)
#endif

#ifndef LOG_DEBUG
#define LOG_DEBUG(logger, fmt, ...) logger->debug({__FILE__, __LINE__, __FUNCTION__}, fmt, ##__VA_ARGS__)
#endif

#ifndef LOG_INFO
#define LOG_INFO(logger, fmt, ...) logger->info({__FILE__, __LINE__, __FUNCTION__}, fmt, ##__VA_ARGS__)
#endif

#ifndef LOG_WARN
#define LOG_WARN(logger, fmt, ...) logger->warn({__FILE__, __LINE__, __FUNCTION__}, fmt, ##__VA_ARGS__)
#endif

#ifndef LOG_ERROR
#define LOG_ERROR(logger, fmt, ...) logger->error({__FILE__, __LINE__, __FUNCTION__}, fmt, ##__VA_ARGS__)
#endif

    template <typename T>
    struct is_real
        : std::integral_constant<
              bool,
              std::is_same<float, typename std::remove_cv<T>::type>::value ||
                  std::is_same<double, typename std::remove_cv<T>::type>::value> {};

    template <typename T>
    inline constexpr bool is_real_v = is_real<T>::value;

    template <typename T>
    static constexpr void deallocIterable(Vector<T*>& iterable) {
        for (auto& i : iterable) {
            DELETE_T(i, T);
        }
        iterable.clear();
    }

    template <typename K, typename V>
    static constexpr void deallocValueInMap(Map<K, V*>& iterable) {
        for (auto& [k, v] : iterable) {
            DELETE_T(v, V);
        }
        iterable.clear();
    }

    template <typename K, typename V>
    static constexpr void deallocValueInMap(UnorderedMap<K, V*>& iterable) {
        for (auto& [k, v] : iterable) {
            DELETE_T(v, V);
        }
        iterable.clear();
    }

    template <typename T>
    static constexpr T align2(T x) { return (x + 1) >> 1 << 1; }
    template <typename T>
    static constexpr T align4(T x) { return (x + 3) >> 2 << 2; }
    template <typename T>
    static constexpr T align8(T x) { return (x + 7) >> 3 << 3; }
}  // namespace GLaDOS

#endif
