/**
 * @file shared_state.h
 * @author sun-moon-star-star
 * @ref
 * https://github.com/boostorg/fiber/blob/develop/include/boost/fiber/future/detail/shared_state.hpp
 */

#ifndef SUN_UTIL_SHARED_STATE_H_
#define SUN_UTIL_SHARED_STATE_H_

#include <atomic>
#include <condition_variable>
#include <mutex>

class shared_state_base {
 private:
  std::atomic<uint64_t> use_count_{0};
  mutable condition_variable waiters_{};

 protected:
  mutable std::mutex mtx_{};
  bool ready_{false};
  std::exception_ptr except_{};
};

#endif  // SUN_UTIL_SHARED_STATE_H_