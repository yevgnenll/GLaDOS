#ifndef GLADOS_UTILITY_H
#define GLADOS_UTILITY_H

#include <list>
#include <map>
#include <set>
#include <type_traits>
#include <unordered_map>
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

  // https://stackoverflow.com/questions/18837857/cant-use-enum-class-as-unordered-map-key
  struct EnumClassHash {
    template <typename T>
    std::size_t operator()(T t) const {
      return static_cast<std::size_t>(t);
    }
  };

  template <typename Key>
  using HashType = typename std::conditional<std::is_enum<Key>::value, EnumClassHash, std::hash<Key>>::type;

  template <typename K, typename V>
  using UnorderedMap = std::unordered_map<K, V, HashType<K>, std::less<K>, STLAllocator<std::pair<const K, V>>>;

  template <typename T>
  using List = std::list<T, STLAllocator<T>>;

  template <typename T>
  using Set = std::set<T, std::less<T>, STLAllocator<T>>;

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
