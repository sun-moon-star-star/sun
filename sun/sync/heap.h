/**
 * @file heap.h
 * @author sun-moon-star-star
 */

#ifndef SUN_SYNC_HEAP_H_
#define SUN_SYNC_HEAP_H_

#include "sun/data_structure/heap.h"
#include "sun/sync/spinlock.h"

#include <mutex>

namespace sun::sync {

template <typename KeyType = uint8_t, typename ValueType = void*,
          typename LockType = spinlock>
class heap {
 private:
  struct data {
    KeyType key;
    ValueType value;
    data(const KeyType& key, const ValueType& value) : key(key), value(value) {}
    bool operator<(const data& d) const { return key < d.key; }
  } __attribute__((__packed__));

  void push(const KeyType& key, const ValueType& value) {
    const std::lock_guard<LockType> lock(_lock);
    _heap.push(key, value);
  }

  void pop(KeyType& key, ValueType& value) {
    data d;

    {
      const std::lock_guard<LockType> lock(_lock);
      pop(&d);
    }

    key = d.key;
    value = d.value;
  }

 private:
  ::sun::data_structure::heap<data> _heap;
  LockType _lock;
};

}  // namespace sun::sync

#endif  // SUN_SYNC_HEAP_H_