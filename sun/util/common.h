/**
 * @file common.h
 * @author sun-moon-star-star
 */

#ifndef SUN_UTIL_COMMON_H_
#define SUN_UTIL_COMMON_H_

#include <ctime>
#include <random>

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

#define NO_CREATABLE(class)     \
  class() = delete;             \
  class(const class&) = delete; \
  class& operator=(const class&) = delete;

#define NO_COPYABLE(class)      \
  class(const class&) = delete; \
  class& operator=(const class&) = delete;

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

namespace sun::util::common::random {

std::default_random_engine default_random_engine;

void init_random_engine_with_time() { default_random_engine.seed(time(NULL)); }

template <typename T = uint64_t>
std::uniform_int_distribution<T> random_getter;

template <typename T = uint64_t>
T random() {
  return random_getter<T>(default_random_engine);
}

}  // namespace sun::util::common::random

#endif  // SUN_UTIL_COMMON_H_