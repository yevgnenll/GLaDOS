#ifndef GLADOS_LINKEDHASHMAP_HPP
#define GLADOS_LINKEDHASHMAP_HPP

#include "Utility.h"

namespace GLaDOS {
  template <typename K, typename V>
  class LinkedHashMap {
  public:
    typedef std::pair<K, V> valueType;
    typedef typename std::list<valueType>::size_type sizeType;
    typedef typename std::list<valueType>::iterator iterator;
    typedef typename std::list<valueType>::const_iterator constIterator;

    LinkedHashMap();
    explicit LinkedHashMap(std::size_t size);
    LinkedHashMap(const LinkedHashMap<K, V>& other);

    std::pair<iterator, bool> insert(const valueType& value);
    bool erase(const K& key);
    sizeType size() const;
    bool empty() const;
    iterator find(const K& key);
    constIterator find(const K& key) const;

    iterator begin();
    iterator end();
    constIterator cbegin() const;
    constIterator cend() const;

  private:
    List<valueType> mLinkedList;
    UnorderedMap<K, iterator> mHashMap;
  };

  template <typename K, typename V>
  LinkedHashMap<K, V>::LinkedHashMap() : mLinkedList{}, mHashMap{} {
  }

  template <typename K, typename V>
  LinkedHashMap<K, V>::LinkedHashMap(std::size_t size) : mLinkedList{}, mHashMap{} {
    mHashMap.reserve(size);
  }

  template <typename K, typename V>
  LinkedHashMap<K, V>::LinkedHashMap(const LinkedHashMap<K, V>& other) : mLinkedList{}, mHashMap{} {
    for (auto citer = other.cbegin(); citer != other.cend(); citer++) {
      insert(*citer);
    }
  }

  template <typename K, typename V>
  std::pair<LinkedHashMap::iterator, bool> LinkedHashMap<K, V>::insert(const valueType& value) {
    mLinkedList.emplace_back(value);
    return mHashMap.insert(std::make_pair(value.first, std::prev(mLinkedList.end())));
  }

  template <typename K, typename V>
  bool LinkedHashMap<K, V>::erase(const K& key) {
    auto iter = mHashMap.find(key);
    if (iter == hash_map.end()) {
      return false;
    }
    mLinkedList.erase(iter->second);
    mHashMap.erase(iter);
    return true;
  }

  template <typename K, typename V>
  LinkedHashMap<K, V>::sizeType LinkedHashMap<K, V>::size() const {
    return mLinkedList.size();
  }

  template <typename K, typename V>
  bool LinkedHashMap<K, V>::empty() const {
    return mLinkedList.empty();
  }

  template <typename K, typename V>
  LinkedHashMap<K, V>::iterator LinkedHashMap<K, V>::find(const K& key) {
    auto iter = mHashMap.find(key);
    return iter != mHashMap.end() ? iter->second : end();
  }

  template <typename K, typename V>
  LinkedHashMap<K, V>::constIterator LinkedHashMap<K, V>::find(const K& key) const {
    auto citer = mHashMap.find(key);
    return citer != mHashMap.cend() ? citer->second : cend();
  }

  template <typename K, typename V>
  LinkedHashMap<K, V>::iterator LinkedHashMap<K, V>::begin() {
    return linked_list.begin();
  }

  template <typename K, typename V>
  LinkedHashMap<K, V>::iterator LinkedHashMap<K, V>::end() {
    return linked_list.end();
  }

  template <typename K, typename V>
  LinkedHashMap<K, V>::constIterator LinkedHashMap<K, V>::cbegin() const {
    return linked_list.cbegin();
  }

  template <typename K, typename V>
  LinkedHashMap<K, V>::constIterator LinkedHashMap<K, V>::cend() const {
    return linked_list.cend();
  }
}  // namespace GLaDOS

#endif  //GLADOS_LINKEDHASHMAP_HPP
