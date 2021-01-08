/**
 * @file queue.h
 * @author sun-moon-star-star
 * @note the queue use uint64_t if
 */

#ifndef SUN_SYNC_QUEUE_H_
#define SUN_SYNC_QUEUE_H_

#include "sun/data_structure/array.h"
#include "sun/sync/spinlock.h"

#include <array>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>

namespace sun::sync {

/**
 * @brief queue
 * @param <typename LockType> LockType must have lock(), unlock() and all
 *     initial operation in construct function
 */
template <typename LockType = spinlock>
struct queue final {
  const uint32_t capcity;

  queue(const uint32_t capcity)
      : capcity(capcity), _front(0llu), _back(0llu), _data(1llu * capcity) {}

  NO_COPYABLE(queue);

  uint32_t count() const {
    const std::lock_guard<LockType> lock(_lock);
    return static_cast<uint32_t>(_back - _front);
  }

  bool empty() const {
    const std::lock_guard<LockType> lock(_lock);
    return _front == _back;
  }

  void fixed() {
    ::sun::data_structure::array<void*> data(1llu * capcity);

    const std::lock_guard<LockType> lock(_lock);

    for (uint64_t i = _front; i < _back; ++i) {
      data.push_back(std::move(_data[i % capcity]));
    }
    _back -= _front;
    _front = 0llu;
    _data.swap(data);
  }

 private:
  void push_common(void* const item) {
    uint64_t new_idx_in_data = _back % capcity;
    ++_back;

    if (new_idx_in_data == _data.size()) {
      _data.push_back(item);
      return;
    }

    assert(new_idx_in_data < _data.size());
    _data[new_idx_in_data] = item;

    return;
  }

 public:
  bool try_push(void* const item) {
    const std::lock_guard<LockType> lock(_lock);

    if (_front + capcity == _back) {
      return false;
    }

    push_common(item);

    return true;
  }

  void push(void* const item) {
    std::unique_lock<LockType> lock(_lock);

    if (capcity == _back - _front) {
      _fill.wait(lock, [&] { return _front + capcity != _back; });
    }

    push_common(item);

    if (_front + 1 == _back) {
      _empty.notify_one();
    }
  }

 private:
  void pop_common(void** item) {
    uint64_t idx_in_data = _front % capcity;
    ++_front;

    assert(idx_in_data < _data.size());
    *item = _data[idx_in_data];
  }

 public:
  bool try_pop(void** item) {
    const std::lock_guard<LockType> lock(_lock);

    if (_front == _back) {
      return false;
    }

    pop_common(item);

    return true;
  }

  void pop(void** item) {
    std::unique_lock<LockType> lock(_lock);

    if (_front == _back) {
      _empty.wait(lock, [&] { return _front != _back; });
    }

    pop_common(item);

    if (_back - _front + 1 == capcity) {
      _fill.notify_one();
    }
  }

 private:
  uint64_t _front;
  uint64_t _back;
  std::condition_variable_any _empty;
  std::condition_variable_any _fill;
  LockType _lock;
  ::sun::data_structure::array<void*> _data;
};

}  // namespace sun::sync

#endif  // SUN_SYNC_QUEUE_H_