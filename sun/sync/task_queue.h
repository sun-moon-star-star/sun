/**
 * @file task_queue.h
 * @author sun-moon-star-star
 */

#ifndef SUN_SYNC_TASK_QUEUE_H_
#define SUN_SYNC_TASK_QUEUE_H_

#include "sun/data_structure/array.h"
#include "sun/sync/spinlock.h"

#include <array>
#include <cstdint>
#include <functional>
#include <mutex>

namespace sun::sync {

/**
 * @brief task_queue
 * @param <typename LockType> LockType must have lock(), unlock() and all
 *     initial operation in construct function
 */
template <typename LockType = spinlock>
struct task_queue {
  using task = std::function<void(void*)>;

  const uint32_t capcity;

  task_queue(const uint32_t capcity)
      : capcity(capcity), _task(1llu * capcity), _front(0), _back(0) {}

  uint32_t count() const {
    const std::lock_guard<LockType> lock(_lock);
    return static_cast<uint32_t>(_back - _front);
  }

  bool empty() const {
    const std::lock_guard<LockType> lock(_lock);
    return _front == _back;
  }

  void fixed() {
    const std::lock_guard<LockType> lock(_lock);

    ::sun::data_structure::array<task> task(1llu * capcity);
    for (uint64_t i = _front; i < _back; ++i) {
      task.push_back(std::move(task[i % capcity]));
    }
    _back -= _front;
    _front = 0llu;
    _task.swap(task);
  }

  bool try_push(const task& item) {
    const std::lock_guard<LockType> lock(_lock);

    uint32_t cnt = static_cast<uint32_t>(_back - _front);
    if (cnt == capcity) {
      return false;
    } else if (_back == UINT64_MAX - 1) {
      fixed();
    }

    uint64_t new_idx_in_task = _back % capcity;
    ++_back;

    if (new_idx_in_task == _task.size()) {
      _task.push_back(item);
      return true;
    }

    assert(new_idx_in_task < _task.size());
    _task[new_idx_in_task] = item;

    return true;
  }

  bool try_pop(task* item) {
    const std::lock_guard<LockType> lock(_lock);

    if (_front == _back) {
      return false;
    }

    uint64_t idx_in_task = _front % capcity;
    ++_front;

    assert(idx_in_task < _task.size());
    *item = _task[idx_in_task];

    return true;
  }

 private:
  ::sun::data_structure::array<task> _task;
  uint64_t _front;
  uint64_t _back;
  LockType _lock;
};

}  // namespace sun::sync

#endif  // SUN_SYNC_TASK_QUEUE_H_