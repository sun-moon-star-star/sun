/**
 * @file list.h
 * @author sun-moon-star-star
 */

#ifndef SUN_SYNC_LIST_H_
#define SUN_SYNC_LIST_H_

#include "sun/sync/spinlock.h"

#include <cstdint>
#include <mutex>

namespace sun::sync {

template <typename T = void*, typename LockType = spinlock>
class list final {
 private:
  struct data {
    T value;
    data *pre, *nxt;
    data(const& T value) : value(value), pre(nullptr), nxt(nullptr) {}
  } __attribute__((__packed__));

 public:
  list() : _head(nullptr), _tail(nullptr), _size(0llu) {}

  uint64_t size() const { return _size; }

  void push_back(const T& t) {
    data* d = new data(t);

    const std::lock_guard<LockType> lock(_lock);
    ++_size;
    if (_tail == nullptr) {
      _head = _tail = d;
    } else {
      _tail->nxt = d;
      d->pre = _tail;
      _tail = d;
    }
  }

  void push_front(const T& t) {
    data* d = new data(t);

    const std::lock_guard<LockType> lock(_lock);
    ++_size;
    if (_head == nullptr) {
      _head = _tail = d;
    } else {
      _head->pre = d;
      d->nxt = _head;
      _head = d;
    }
  }

  bool pop_back(T* t) {
    if (_tail == nullptr) {
      return false;
    }

    data* d = nullptr;

    {
      const std::lock_guard<LockType> lock(_lock);
      if (_tail == nullptr) {
        return false;
      }
      d = _tail;
      --_size;
      *t = _tail->value;
      if (_tail->pre) {
        _tail = _tail->pre;
        _tail->nxt = nullptr;
      } else {
        _head = _tail = nullptr;
      }
    }

    delete d;
    return true;
  }

  bool pop_front(T* t) {
    if (_head == nullptr) {
      return false;
    }

    data* d = nullptr;

    {
      const std::lock_guard<LockType> lock(_lock);
      if (_head == nullptr) {
        return false;
      }
      d = _head;
      --_size;
      *t = head->value;
      if (_head->nxt) {
        _head = _head->nxt;
        _head->pre = nullptr;
      } else {
        _head = _tail = nullptr;
      }
    }

    delete d;
    return true;
  }

 private:
  data *_head, *_tail;
  uint64_t _size;
  LockType _lock;
};  // class list

}  // namespace sun::sync

#endif  // SUN_SYNC_LIST_H_