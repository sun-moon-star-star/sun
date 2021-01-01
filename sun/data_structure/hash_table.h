/**
 * @file hash_table.h
 * @author sun-moon-star-star
 */

#ifndef SUN_DATA_STRUCTURE_HASH_TABLE_H_
#define SUN_DATA_STRUCTURE_HASH_TABLE_H_

#include "sun/util/common.h"
#include "sun/util/hash.h"

#include <cstdint>
#include <functional>

namespace sun::data_structure {

namespace __hash_table {

struct hash_node {
  uint64_t hashcode;
  void* data;
  hash_node* nxt;
};

class hash_entry {
 public:
  hash_entry() : _num(0), _head(nullptr) {}

 private:
  hash_node* search(uint64_t hashcode) {
    for (hash_node* cur = _head; cur != nullptr; cur = cur->nxt) {
      if (cur->hashcode == hashcode) {
        return cur;
      }
    }
    return nullptr;
  }

 public:
  void* set(uint64_t hashcode, void* data) {
    hash_node* node = search(hashcode);
    void* raw_data = nullptr;
    if (node != nullptr) {
      raw_data = node->data;
      node->data = data;
    } else {
      ++_num;
      node = new hash_node{hashcode, data, _head};
      _head = node;
    }
    return raw_data;
  }

  void* get(uint64_t hashcode) {
    hash_node* node = search(hashcode);
    if (node == nullptr) {
      return nullptr;
    }
    return node->data;
  }

  void* remove(uint64_t hashcode) {
    hash_node* last = nullptr;
    hash_node* cur = _head;

    while (cur != nullptr) {
      if (cur->hashcode == hashcode) {
        --_num;
        if (last == nullptr) {
          _head = cur->nxt;
        } else {
          last->nxt = cur->nxt;
        }
        void* raw_data = cur->data;
        delete cur;
        return raw_data;
      }
      last = cur;
      cur = cur->nxt;
    }

    return nullptr;
  }

 private:
  uint32_t _num;
  hash_node* _head;
};
}  // namespace __hash_table

template <typename KeyType, typename ValueType,
          typename HashFunc = ::sun::util::hasher<KeyType>>
class hash_table {
 public:
  hash_table(uint32_t factor = 5, uint32_t slots_num = 16)
      : _factor(factor),
        _slots_num(1u << (32 - __builtin_clz(
                                   min(max(slots_num, 16u), 2147483648u) - 1))),
        _count(0),
        _slots(new __hash_table::hash_entry[_slots_num]) {}

  void set(const KeyType& key, const ValueType& value) {
    uint64_t hashcode = _hashfunc(key);
    uint32_t slot = get_slot(hashcode);

    ValueType* ptr = reinterpret_cast<ValueType*>(malloc(sizeof(ValueType)));
    *ptr = value;

    void* data = _slots[slot].set(hashcode, ptr);
    if (data != nullptr) {
      ptr = reinterpret_cast<ValueType*>(data);
      delete ptr;
    } else {
      ++_count;
    }
  }

  bool get(const KeyType& key, ValueType* value) {
    uint64_t hashcode = _hashfunc(key);
    uint32_t slot = get_slot(hashcode);

    void* data = _slots[slot].get(hashcode);
    if (data == nullptr) {
      return false;
    } else {
      *value = *reinterpret_cast<ValueType*>(data);
    }
    return true;
  }

  bool erase(const KeyType& key, ValueType* value = nullptr) {
    uint64_t hashcode = _hashfunc(key);
    uint32_t slot = get_slot(hashcode);

    void* data = _slots[slot].remove(hashcode);

    if (data == nullptr) {
      return false;
    }

    --_count;
    ValueType* raw_data = reinterpret_cast<ValueType*>(data);
    *value = *raw_data;
    delete raw_data;

    return true;
  }

  uint32_t get_slot(uint64_t hashcode) const {
    return hashcode & (_slots_num - 1);
  }

  uint32_t count() const { return _count; }

 private:
  const uint32_t _factor;
  uint32_t _slots_num;
  uint64_t _count;
  __hash_table::hash_entry* _slots;
  HashFunc _hashfunc;
};

}  // namespace sun::data_structure

#endif  // SUN_DATA_STRUCTURE_HASH_TABLE_H_