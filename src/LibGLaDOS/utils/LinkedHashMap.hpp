#ifndef GLADOS_LINKEDHASHMAP_HPP
#define GLADOS_LINKEDHASHMAP_HPP

#include "Utility.h"

namespace GLaDOS {
    template <typename K, typename V>
    class LinkedHashMap {
      public:
        typedef std::pair<K, V> value_type;
        typedef typename List<value_type>::size_type size_type;
        typedef typename List<value_type>::iterator iterator;
        typedef typename List<value_type>::const_iterator const_iterator;

        LinkedHashMap();
        explicit LinkedHashMap(std::size_t size);
        LinkedHashMap(const LinkedHashMap<K, V>& other);

        std::pair<typename UnorderedMap<K, iterator>::iterator, bool> insert(const value_type& value);
        bool erase(const K& key);
        size_type size() const;
        bool empty() const;
        iterator find(const K& key);
        const_iterator find(const K& key) const;

        iterator begin();
        iterator end();
        const_iterator cbegin() const;
        const_iterator cend() const;

      private:
        List<value_type> mLinkedList;
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
    std::pair<typename UnorderedMap<K, typename LinkedHashMap<K, V>::iterator>::iterator, bool> LinkedHashMap<K, V>::insert(const value_type& value) {
        mLinkedList.emplace_back(value);
        return mHashMap.insert(std::make_pair(value.first, std::prev(mLinkedList.end())));
    }

    template <typename K, typename V>
    bool LinkedHashMap<K, V>::erase(const K& key) {
        auto iter = mHashMap.find(key);
        if (iter == mHashMap.end()) {
            return false;
        }
        mLinkedList.erase(iter->second);
        mHashMap.erase(iter);
        return true;
    }

    template <typename K, typename V>
    typename LinkedHashMap<K, V>::size_type LinkedHashMap<K, V>::size() const {
        return mLinkedList.size();
    }

    template <typename K, typename V>
    bool LinkedHashMap<K, V>::empty() const {
        return mLinkedList.empty();
    }

    template <typename K, typename V>
    typename LinkedHashMap<K, V>::iterator LinkedHashMap<K, V>::find(const K& key) {
        auto iter = mHashMap.find(key);
        return iter != mHashMap.end() ? iter->second : end();
    }

    template <typename K, typename V>
    typename LinkedHashMap<K, V>::const_iterator LinkedHashMap<K, V>::find(const K& key) const {
        auto citer = mHashMap.find(key);
        return citer != mHashMap.cend() ? citer->second : cend();
    }

    template <typename K, typename V>
    typename LinkedHashMap<K, V>::iterator LinkedHashMap<K, V>::begin() {
        return mLinkedList.begin();
    }

    template <typename K, typename V>
    typename LinkedHashMap<K, V>::iterator LinkedHashMap<K, V>::end() {
        return mLinkedList.end();
    }

    template <typename K, typename V>
    typename LinkedHashMap<K, V>::const_iterator LinkedHashMap<K, V>::cbegin() const {
        return mLinkedList.cbegin();
    }

    template <typename K, typename V>
    typename LinkedHashMap<K, V>::const_iterator LinkedHashMap<K, V>::cend() const {
        return mLinkedList.cend();
    }
}  // namespace GLaDOS

#endif  //GLADOS_LINKEDHASHMAP_HPP
