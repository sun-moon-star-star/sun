/**
 * @file lru_cache.h
 * @author sun-moon-star-star
 */

#ifndef SUN_CACHE_LRU_CACHE_H_
#define SUN_CACHE_LRU_CACHE_H_

#include "sun/data_structure/hash_table.h"
#include "sun/util/hash.h"

#include <functional>
#include <list>

using sun::data_structure::hash_table;

namespace sun::cache {

template <typename KeyType, typename ValueType>
class lru_cache {
 private:
  using lru_node = struct {
    uint64_t expire_time;
    KeyType key;
    std::shared_ptr<ValueType> value;
  };

  using key_node = struct {
    KeyType key;
    std::shared_ptr<key_node> pre;
    std::shared_ptr<key_node> next;
  };

  void move_to_head(const KeyType& key) {
    std::shared_ptr<key_node> p = _keys[key];

    if (p->pre == nullptr) {
      return;
    }
    p->pre->next = p->next;

    if (p->next) {
      p->next->pre = p->pre;
    }

    p->next = _head;
    _head->pre = p;
    _head = p;
  }

  void remove_tail() {
    if (_tail == nullptr) {
      return;
    }

    if (destory_func) {
      destory_func(_tail->key, _data[_tail->key]->value,
                   _data[_tail->key]->expire_time);
    }

    _data.erase(_tail->key);
    _keys.erase(_tail->key);
    if (_head == _tail) {
      _head = _tail = nullptr;
      return;
    }

    std::shared_ptr<key_node> tmp = _tail->pre;
    _tail = tmp;
    _tail->next = nullptr;
  }

 public:
  using expire_func = std::function<void(KeyType, std::shared_ptr<ValueType>,
                                         uint64_t expire_time)>;
  using destory_func = std::function<void(KeyType, std::shared_ptr<ValueType>,
                                          uint64_t expire_time)>;

  explicit lru_cache(uint64_t size)
      : _head(nullptr), _tail(nullptr), _size(size) {}

  uint64_t size() const { return _keys.size(); }

  void push(const KeyType& key, std::shared_ptr<ValueType> value,
            uint64_t expire_time) {
    std::shared_ptr<lru_node> node_ptr = nullptr;

    if (_data.get(key, &node_ptr)) {
      node_ptr->value = value;
      move_to_head(key);
      return;
    }

    if (_size == _keys.size()) {
      remove_tail();
    }

    node_ptr =
        new lru_node{.expire_time = expire_time, .key = key, .value = value};

    std::shared_ptr<key_node> key_node_ptr = new key_node{key, nullptr, _head};
    _keys.push(key, key_node_ptr);

    if (_head == nullptr) {
      _head = _tail = key_node_ptr;
    }

    _head->pre = key_node_ptr;
    _head = key_node_ptr;
  }

  void push(const KeyType& key, std::shared_ptr<ValueType> value) {
    return push(key, value, 0);
  }

  std::shared_ptr<ValueType> value get(const KeyType& key) {
    std::shared_ptr<lru_node> node_ptr = nullptr;
    if (!_data.get(key, &node_ptr)) {
      return nullptr;
    }

    if (_expire_func && _expire_func(node_ptr->key, node_ptr->value,
                                    node_ptr->expire_time) {
      _data.erase(key);

      if (_head == _keys[key]) {
        _head = _head->next;
      }

      if (_tail == _keys[key]) {
        _key = _key->pre;
      }

      _keys.erase(key);
      return nullptr;                     
    }

    return node_ptr->value;
  }

  hash_table<KeyType, std::shared_ptr<lru_node>> _data;
  hash_table<KeyType, std::shared_ptr<key_node>> _keys;
  std::shared_ptr<key_node> _head;
  std::shared_ptr<key_node> _tail;
  const uint64_t _size;
  destory_func _destory_func;
  expire_func _expire_func;
};  // class lru_cache

}  // namespace sun::cache

#endif  // SUN_CACHE_LRU_CACHE_H_