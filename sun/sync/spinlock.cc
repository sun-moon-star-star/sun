/**
 * @file spinlock.cc
 * @author sun-moon-star-star
 */

#include "sun/sync/spinlock.h"

namespace sun::sync {

spinlock::spinlock() : _lock(false) {}

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
