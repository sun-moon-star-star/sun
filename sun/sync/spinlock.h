/**
 * @file spinlock.h
 * @author sun-moon-star-star
 */

#ifndef SUN_SYNC_SPINLOCK_H_
#define SUN_SYNC_SPINLOCK_H_

#include "sun/util/common.h"

#include <atomic>

namespace sun::sync {

struct spinlock {
  spinlock();

  NO_COPYABLE(spinlock);

  void lock();

  bool try_lock();

  void unlock();

  bool locked() const;

  bool unlocked() const;

 private:
  std::atomic_bool _lock;
};

}  // namespace sun::sync

#endif  // SUN_SYNC_SPINLOCK_H_