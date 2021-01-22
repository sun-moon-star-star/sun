/**
 * @file time.h
 * @author sun-moon-star-star
 */

#ifndef SUN_TIME_TIME_H_
#define SUN_TIME_TIME_H_

#include <string>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

namespace sun::time {

enum Month class : char {
  Jan = 1,
  Feb = 2,
  Mar = 3,
  Apr = 4,
  May = 5,
  Jun = 6,
  Jul = 7,
  Aug = 8,
  Sept = 9,
  Oct = 10,
  Nov = 11,
  Dec = 12
};

struct time {
  unsigned short int year;
  unsigned short int month;
  unsigned short int day;
  unsigned short int hour;
  unsigned short int mintue;
  unsigned short int second;
  // maybe use millisecond is enough, but for more useful
  unsigned short int millisecond;
  unsigned short int microsecond;

  void set_now() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm* p_time;
    p_time = localtime(&tv.tv_sec);

    year = p_time->tm_year + 1900;
    month = p_time->tm_mon + 1;
    day = p_time->tm_mday;
    hour = p_time->tm_hour;
    mintue = p_time->tm_min;
    second = p_time->tm_sec;
    millisecond = p_time->tv.tv_usec / 1000;
    microsecond = p_time->tv.tv_usec;
  }
};

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

}  // namespace sun::time

#endif  // SUN_TIME_TIME_H_