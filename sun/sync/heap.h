/**
 * @file heap.h
 * @author sun-moon-star-star
 */

#ifndef SUN_SYNC_HEAP_H_
#define SUN_SYNC_HEAP_H_

#include "sun/data_structure/heap.h"

#include <mutex>

namespace sun::sync {

template <typename KeyType = uint8_t, typename ValueType = void*,
          typename LockType = std::mutex>
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

  bool pop(KeyType& key, ValueType& value) {
    if (_heap.size() == 0) {
      return false;
    }

    data d;

    {
      const std::lock_guard<LockType> lock(_lock);
      if (_heap.size() == 0) {
        return false;
      }
      pop(&d);
    }

    key = d.key;
    value = d.value;
    return true;
  }

  uint32_t size() const { return _heap.size(); }

 private:
  ::sun::data_structure::heap<data> _heap;
  LockType _lock;
};

}  // namespace sun::sync

#endif  // SUN_SYNC_HEAP_H_