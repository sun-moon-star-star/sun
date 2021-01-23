/**
 * @file timer.h
 * @author sun-moon-star-star
 */

#ifndef SUN_TIME_TIMER_H_
#define SUN_TIME_TIMER_H_

#include <string>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

namespace sun {

struct timer {
  explicit timer(time_t* lifetime = nullptr) : _lifetime(lifetime) {
    if (_lifetime != nullptr) {
      gettimeofday(&_lifetime_start, nullptr);
    }
    gettimeofday(&_start, nullptr);
  }

  ~timer() {
    if (_lifetime != nullptr) {
      struct timeval end;
      gettimeofday(&end, nullptr);
      time_t diff = (end.tv_sec - _lifetime_start.tv_sec) * kUsec +
                    end.tv_usec - _lifetime_start.tv_usec;
      *_lifetime += diff;
    }

    void reset() { gettimeofday(&_start, nullptr); }

    time_t elapse() {
      struct timeval end;
      gettimeofday(&end, nullptr);
      time_t diff =
          (end.tv_sec - _start.tv_sec) * 1000000 + end.tv_usec - _start.tv_usec;
      return diff;
    }
  }

 private:
  struct timeval _lifetime_start;  // timer的创建时间
  struct timeval _start;           // 设置时间
  time_t* _lifetime;  // 析构函数中会调用,设置整个timer的时间周期赋值给_lifetime
};

}  // namespace sun

#endif  //  SUN_TIME_TIMER_H_