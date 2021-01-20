/**
 * @file queue.h
 * @author sun-moon-star-star
 */

#ifndef SUN_SYNC_QUEUE_H_
#define SUN_SYNC_QUEUE_H_

#include "sun/data_structure/array.h"

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
template <typename T = void*, typename LockType = std::mutex>
struct queue final {
  const uint32_t capcity;

  queue(const uint32_t capcity = 4294967296u)
      : capcity(capcity), _front(0llu), _back(0llu), _data(1llu * capcity) {}

  NO_COPYABLE(queue);

  uint32_t size() const {
    const std::lock_guard<LockType> lock(_lock);
    return static_cast<uint32_t>(_back - _front);
  }

  uint32_t size_with_no_lock() const {
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
  void push_common(const T& item) {
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
  bool try_push(const T& item) {
    if (_front + capcity == _back) {
      return false;
    }

    const std::unique_lock<LockType> lock(_lock);

    if (_front + capcity == _back) {
      return false;
    }

    push_common(item);

    if (_front + 1 == _back) {
      lock.unlock();
      _empty.notify_one();
    }

    return true;
  }

  bool try_push_sync_with(const T& item, std::function<void(void)> func) {
    if (_front + capcity == _back) {
      return false;
    }

    const std::unique_lock<LockType> lock(_lock);

    if (_front + capcity == _back) {
      return false;
    }

    push_common(item);

    if (_front + 1 == _back) {
      lock.unlock();
      _empty.notify_one();
    }

    func();

    return true;
  }

  void push(const T& item) {
    std::unique_lock<LockType> lock(_lock);

    while (capcity == _back - _front) {
      _fill.wait(lock, [&] { return _front + capcity != _back; });
    }

    push_common(item);

    if (_front + 1 == _back) {
      lock.unlock();
      _empty.notify_one();
    }
  }

 private:
  void pop_common(T* item) {
    uint64_t idx_in_data = _front % capcity;
    ++_front;

    assert(idx_in_data < _data.size());
    *item = _data[idx_in_data];
  }

 public:
  bool try_pop(T* item) {
    if (_front == _back) {
      return false;
    }

    const std::unique_lock<LockType> lock(_lock);

    if (_front == _back) {
      return false;
    }

    pop_common(item);

    if (_back - _front + 1 == capcity) {
      lock.unlock();
      _fill.notify_one();
    }

    return true;
  }

  bool try_pop_sync_witch(T* item, std::function<void(void)> func) {
    if (_front == _back) {
      return false;
    }

    const std::unique_lock<LockType> lock(_lock);

    if (_front == _back) {
      return false;
    }

    pop_common(item);

    if (_back - _front + 1 == capcity) {
      lock.unlock();
      _fill.notify_one();
    }

    func();

    return true;
  }

  void pop(T* item) {
    std::unique_lock<LockType> lock(_lock);

    while (_front == _back) {
      _empty.wait(lock, [&] { return _front != _back; });
    }

    pop_common(item);

    if (_back - _front + 1 == capcity) {
      lock.unlock();
      _fill.notify_one();
    }
  }

 private:
  uint64_t _front;
  uint64_t _back;
  std::condition_variable_any _empty;
  std::condition_variable_any _fill;
  LockType _lock;
  ::sun::data_structure::array<T> _data;
};

}  // namespace sun::sync

#endif  // SUN_SYNC_QUEUE_H_