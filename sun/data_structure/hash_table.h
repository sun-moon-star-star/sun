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
  hash_entry() : num(0), head(nullptr) {}

 private:
  hash_node* search(uint64_t hashcode) {
    for (hash_node* cur = head; cur != nullptr; cur = cur->nxt) {
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
      ++num;
      node = new hash_node{hashcode, data, head};
      head = node;
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
    hash_node* cur = head;

    while (cur != nullptr) {
      if (cur->hashcode == hashcode) {
        --num;
        if (last == nullptr) {
          head = cur->nxt;
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

  uint32_t num;
  hash_node* head;
};

}  // namespace __hash_table

template <typename KeyType, typename ValueType,
          typename HashFunc = ::sun::util::hasher<KeyType>>
class hash_table {
  using data_type = struct {
    KeyType key;
    ValueType value;
  };

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

    data_type* ptr = new data_type{.key = key, .value = value};

    void* data = _slots[slot].set(hashcode, ptr);
    if (data != nullptr) {
      ptr = reinterpret_cast<data_type*>(data);
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
      *value = reinterpret_cast<data_type*>(data)->value;
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
    data_type* raw_data = reinterpret_cast<data_type*>(data);
    *value = raw_data->value;
    delete raw_data;

    return true;
  }

  uint32_t get_slot(uint64_t hashcode) const {
    return hashcode & (_slots_num - 1);
  }

  uint32_t count() const { return _count; }

  struct iterator {
   private:
    bool next() {
      for (; _now_slot < _slots_num; _node = nullptr, ++_now_slot, ++_slot) {
        if (_node == nullptr) {
          _node = _slot->head;
        } else {
          _node = _node->nxt;
        }

        if (_node != nullptr) {
          return true;
        }
      }
      return false;
    }

   public:
    iterator(uint32_t slots_num, uint32_t now_slot,
             __hash_table::hash_entry* slot)
        : _slots_num(slots_num),
          _now_slot(now_slot),
          _slot(slot),
          _node(nullptr) {
      next();
    }

    data_type* operator->() const {
      return reinterpret_cast<data_type*>(_node->data);
    }

    iterator& operator++() {
      next();
      return *this;
    }

    bool valid() const { return _node != nullptr; }

   private:
    uint32_t _slots_num;
    uint32_t _now_slot;
    __hash_table::hash_entry* _slot;
    __hash_table::hash_node* _node;
  };

  iterator get_iterator() const { return iterator(_slots_num, 0, _slots); }

 private:
  const uint32_t _factor;
  uint32_t _slots_num;
  uint64_t _count;
  __hash_table::hash_entry* _slots;
  HashFunc _hashfunc;
};

}  // namespace sun::data_structure

#endif  // SUN_DATA_STRUCTURE_HASH_TABLE_H_