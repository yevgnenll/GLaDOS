#ifndef GLADOS_STL_H
#define GLADOS_STL_H

#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "String.hpp"
#include "memory/STLAllocator.h"

namespace GLaDOS {
  // https://stackoverflow.com/questions/18837857/cant-use-enum-class-as-unordered-map-key
  struct EnumClassHash {
    // for use any type of enum class
    template <typename T>
    std::size_t operator()(T t) const {
      return static_cast<std::size_t>(t);
    }
  };

// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
#if SIZE_MAX == UINT32_MAX
  constexpr std::size_t FNV_PRIME = 16777619u;
  constexpr std::size_t FNV_OFFSET_BASIS = 2166136261u;
#elif SIZE_MAX == UINT64_MAX
  constexpr std::size_t FNV_PRIME = 1099511628211u;
  constexpr std::size_t FNV_OFFSET_BASIS = 14695981039346656037u;
#else
#error "std::size_t must be greater than 32 bits."
#endif

  template <typename T>
  using Vector = std::vector<T, STLAllocator<T>>;

  // std::map default Compare is std::less<Key>
  template <typename K, typename V>
  using Map = std::map<K, V, std::less<K>, STLAllocator<std::pair<const K, V>>>;

  template <typename Key>
  using HashType = typename std::conditional<std::is_enum<Key>::value, EnumClassHash, std::hash<Key>>::type;

  // std::unordered_map default Pred is std::equal_to<Key>
  template <typename K, typename V>
  using UnorderedMap = std::unordered_map<K, V, HashType<K>, std::equal_to<K>, STLAllocator<std::pair<const K, V>>>;

  template <typename T>
  using List = std::list<T, STLAllocator<T>>;

  template <typename T>
  using Set = std::set<T, std::less<T>, STLAllocator<T>>;

  template <typename T>
  using Deque = std::deque<T, STLAllocator<T>>;

  template <typename T>
  using Queue = std::queue<T, Deque<T>>;
}  // namespace GLaDOS

namespace std {
  template <>
  struct hash<GLaDOS::String> {
    size_t operator()(const GLaDOS::String& k) const {
      size_t hashCode = GLaDOS::FNV_OFFSET_BASIS;
      for (auto it = k.begin(); it != k.end(); it++) {
        hashCode ^= *it;
        hashCode *= GLaDOS::FNV_PRIME;
      }
      return hashCode;
    }
  };
}  // namespace std

#endif  //GLADOS_STL_H
