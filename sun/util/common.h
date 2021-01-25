/**
 * @file common.h
 * @author sun-moon-star-star
 */

#ifndef SUN_UTIL_COMMON_H_
#define SUN_UTIL_COMMON_H_

#include <ctime>
#include <random>

/** @ref: https://en.wikipedia.org/wiki/Restrict */
#ifndef restrict
#define restrict
#endif

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#define IF_TRUE_RETURN(condition, statement) \
  do {                                       \
    if ((condition)) return (statement);     \
  } while (0)

#define IF_FALSE_RETURN(condition, statement) \
  do {                                        \
    if (!(condition)) return (statement);     \
  } while (0)

#define NO_CREATABLE(class)                \
  class() = delete;                        \
  class(const class&) = delete;            \
  class& operator=(const class&) = delete; \
  class(class &&) = delete;                \
  class& operator=(class&&) = delete;

#define NO_COPYABLE(class)                 \
  class(const class&) = delete;            \
  class& operator=(const class&) = delete; \
  class(class &&) = delete;                \
  class& operator=(class&&) = delete;

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

#define TEST_MAIN                         \
  int main(int argc, char** argv) {       \
    testing::InitGoogleTest(&argc, argv); \
    return RUN_ALL_TESTS();               \
  }

#endif  // SUN_UTIL_COMMON_H_