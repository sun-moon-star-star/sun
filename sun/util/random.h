/**
 * @file hash.h
 * @author sun-moon-star-star
 */
#ifndef SUN_UTIL_RANDOM_H_
#define SUN_UTIL_RANDOM_H_

#include <cstdint>
#include <random>

namespace sun::util::random {

std::default_random_engine default_random_engine;

void init_random_engine_with_time() { default_random_engine.seed(time(NULL)); }

template <typename T = uint64_t>
std::uniform_int_distribution<T> random_getter;

template <typename T = uint64_t>
T random() {
  return random_getter<T>(default_random_engine);
}

}  // namespace sun::util::random

#endif  // SUN_UTIL_RANDOM_H_