/**
 * @file time.h
 * @author sun-moon-star-star
 */

#ifndef SUN_TIME_TIME_H_
#define SUN_TIME_TIME_H_

#include "sun/util/common.h"

#include <string>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

namespace sun {

enum class month : char {
  jan = 1,
  feb = 2,
  mar = 3,
  apr = 4,
  may = 5,
  jun = 6,
  jul = 7,
  aug = 8,
  sept = 9,
  oct = 10,
  nov = 11,
  dec = 12,
};

enum class week : char {
  sun = 0,
  mon = 1,
  tue = 2,
  wed = 3,
  thu = 4,
  fri = 5,
  sat = 6,
};

std::string to_string(month m) {
  switch (m) {
    case month::jan:
      return "Jan";
    case month::feb:
      return "Feb";
    case month::mar:
      return "Mar";
    case month::apr:
      return "Apr";
    case month::may:
      return "May";
    case month::jun:
      return "Jun";
    case month::jul:
      return "Jul";
    case month::aug:
      return "Aug";
    case month::sept:
      return "Sept";
    case month::oct:
      return "Oct";
    case month::nov:
      return "Nov";
    case month::dec:
      return "Dec";
    default:
      return "";
  }
}

std::string to_string(week w) {
  switch (w) {
    case week::sun:
      return "Sun";
    case week::mon:
      return "Mon";
    case week::tue:
      return "Tue";
    case week::wed:
      return "Wed";
    case week::thu:
      return "Thu";
    case week::fri:
      return "Fri";
    case week::sat:
      return "Sat";
    default:
      return "";
  }
}

struct time {
  uint16_t year;
  uint8_t month;  /* months since January [1-12] */
  uint8_t day;    /* day of the month [1-31] */
  uint8_t hour;   /* hours since midnight [0-23] */
  uint8_t mintue; /* minutes after the hour [0-59] */
  uint8_t second; /* seconds after the minute [0-60] */
  // maybe use millisecond is enough, but for more useful
  uint16_t millisecond;
  uint16_t microsecond;

  uint8_t week() const {
    if (month == 1 || month == 2) {
      return (day + 1 + 2 * (month + 12) + 3 * (month + 13) / 5 + (year - 1) +
              (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400) %
             7;
    }

    return (day + 1 + 2 * month + 3 * (month + 1) / 5 + year + year / 4 -
            year / 100 + year / 400) %
           7;
  }

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
    millisecond = tv.tv_usec / 1000;
    microsecond = tv.tv_usec;
  }

  std::string month_string() {
    return to_string(static_cast<enum month>(month));
  }
} __attribute__((__packed__));

}  // namespace sun

#endif  // SUN_TIME_TIME_H_