#ifndef GLADOS_UTILITY_H
#define GLADOS_UTILITY_H

#include <map>
#include <type_traits>
#include <vector>

#include "memory/Allocation.h"
#include "memory/STLAllocator.h"
#include "utils/Debug.h"

namespace GLaDOS {
#define LOG_TRACE(fmt, ...) GLaDOS::Debug::getInstance()->trace({__FILE__, __LINE__, __FUNCTION__}, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) GLaDOS::Debug::getInstance()->debug({__FILE__, __LINE__, __FUNCTION__}, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) GLaDOS::Debug::getInstance()->info({__FILE__, __LINE__, __FUNCTION__}, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) GLaDOS::Debug::getInstance()->warn({__FILE__, __LINE__, __FUNCTION__}, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) GLaDOS::Debug::getInstance()->error({__FILE__, __LINE__, __FUNCTION__}, fmt, ##__VA_ARGS__)

  template <typename T>
  struct is_real
      : std::integral_constant<
            bool,
            std::is_same<float, typename std::remove_cv<T>::type>::value ||
                std::is_same<double, typename std::remove_cv<T>::type>::value> {};

  template <typename T>
  inline constexpr bool is_real_v = is_real<T>::value;

  template <typename T>
  using Vector = std::vector<T, STLAllocator<T>>;

  template <typename K, typename V>
  using Map = std::map<K, V, std::less<K>, STLAllocator<std::pair<const K, V>>>;

  template <typename T>
  static constexpr void deallocIterable(Vector<T>& iterable) {
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

  template <typename T>
  static constexpr T align2(T x) { return (x + 1) >> 1 << 1; }
  template <typename T>
  static constexpr T align4(T x) { return (x + 3) >> 2 << 2; }
  template <typename T>
  static constexpr T align8(T x) { return (x + 7) >> 3 << 3; }
}  // namespace GLaDOS

#endif
