/**
 * @file count_down_latch.h
 * @author sun-moon-star-star
 */

#ifndef SUN_SYNC_COUNT_DOWN_LATCH_H_
#define SUN_SYNC_COUNT_DOWN_LATCH_H_

#include "sun/util/common.h"

#include <condition_variable>
#include <mutex>

namespace sun::sync {

template <typename LockType = std::mutex>
class count_down_latch {
 public:
  NO_COPYABLE(count_down_latch);

  explicit count_down_latch(uint32_t count) : _count(count) {}

  void wait() {
    std::unique_lock<LockType> lock(_lock);

    while (_count > 0) {
      _condition.wait(lock, [&] { return _count == 0; });
    }
  }

  bool count_down() {
    const std::unique_lock<LockType> lock(_lock);
    if (_count == 0) {
      return false;
    }

    if (--_count == 0) {
      _lock.unlock();
      _condition.notify_all();
    }
    return true;
  }

  uint32_t get_count() const {
    const std::lock_guard<LockType> lock(_lock);
    return _count;
  }

 private:
  uint32_t _count;
  mutable LockType _lock;
  std::condition_variable_any _condition;
};  //  class count_down_latch

}  // namespace sun::sync

#endif  // SUN_SYNC_COUNT_DOWN_LATCH_H_