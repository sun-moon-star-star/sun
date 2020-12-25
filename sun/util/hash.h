/**
 * @file hash.h
 * @author sun-moon-star-star
 */

#ifndef SUN_UTIL_HASH_H_
#define SUN_UTIL_HASH_H_

#include <cstdint>

namespace sun {

namespace util {

using hash_code_t = std::uint64_t;

template <typename T>
hash_code_t hash_code() {
  hash_code_t code = 0;
  return code;
}

}  // namespace util

}  // namespace sun

#endif  // SUN_UTIL_HASH_H_