/**
 * @file task_queue.h
 * @author sun-moon-star-star
 */

#ifndef SUN_SYNC_TASK_QUEUE_H_
#define SUN_SYNC_TASK_QUEUE_H_

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

  task_queue(const uint32_t capcity) : capcity(capcity), _size(0) {}

  uint32_t size() const { return _size; }

  void push(const task& item) {}

 private:
  _task;
  LockType _lock;
  uint32_t _size;
};

}  // namespace sun::sync

#endif  // SUN_SYNC_TASK_QUEUE_H_