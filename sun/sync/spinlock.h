/**
 * @file spinlock.h
 * @author sun-moon-star-star
 */

#ifndef SUN_SYNC_SPINLOCK_H_
#define SUN_SYNC_SPINLOCK_H_

#include <atomic>

namespace sun::sync {

struct spinlock {
  spinlock();

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