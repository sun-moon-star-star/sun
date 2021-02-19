/**
 * @file spinlock.cc
 * @author sun-moon-star-star
 */

#include "sun/sync/spinlock.h"

namespace sun::sync {

spinlock::spinlock() : _lock(false) {}

// 自旋锁设计的时候需要考虑到，在持有锁的时候没有被调度出去
// 使用内核的自旋锁

void spinlock::lock() {
  bool expect = false;
  while (!_lock.compare_exchange_weak(expect, true)) {
    expect = false;
  }
}

bool spinlock::try_lock() {
  bool expect = false;
  return _lock.compare_exchange_strong(expect, true);
}

void spinlock::unlock() { _lock.store(false, std::memory_order_release); }

bool spinlock::locked() const { return _lock.load(std::memory_order_acquire); }

bool spinlock::unlocked() const {
  return !_lock.load(std::memory_order_acquire);
}

}  // namespace sun::sync
