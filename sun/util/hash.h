/**
 * @file hash.h
 * @author sun-moon-star-star
 */

#ifndef SUN_UTIL_HASH_H_
#define SUN_UTIL_HASH_H_

#include <cstdint>
#include <string>
#include <type_traits>

namespace sun::util {

namespace __1 {

template <typename T, uint8_t bytes = sizeof(T)>
struct get_bytes {
  typedef uint64_t type;
};

template <typename T>
struct get_bytes<T, 1> {
  typedef uint8_t type;
};

template <typename T>
struct get_bytes<T, 2> {
  typedef uint16_t type;
};

template <typename T>
struct get_bytes<T, 4> {
  typedef uint32_t type;
};

#ifndef _LIBCPP_HAS_NO_INT128
template <typename T>
struct get_bytes<T, 16> {
  typedef __uint128_t type;
};
#endif

template <typename T, uint8_t bytes = sizeof(T)>
struct get_hash_code_len {
  typedef uint64_t type;
};

#ifndef _LIBCPP_HAS_NO_INT128
template <typename T>
struct get_hash_code_len<T, 16> {
  typedef __uint128_t type;
};
#endif

template <class T, uint8_t bytes, std::size_t N>
struct get_hash_code_len<T[N], bytes> {
  typedef get_hash_code_len<T> type;
};

union ptr_getter {
  const void* ptr;
  uint64_t code;
};  // union ptr_getter

const uint64_t HASH_FACTOR = 11131;

}  // namespace __1

/**
 * @brief hash_code 默认按字节累加
 */
template <typename T, typename V = void>
struct hash_code_helper {
  hash_code_helper(T&& t) {
    uint64_t len = sizeof(t);
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&t);
    code = 0;
    for (uint64_t i = 0; i < len; ++i, ++ptr) {
      code = code * __1::HASH_FACTOR + (*ptr);
    };
  }
  uint64_t get() const { return code; }

 private:
  uint64_t code;
};

template <typename T>
struct hash_code_helper<
    T, std::enable_if_t<std::is_integral_v<std::remove_reference_t<T>> ||
                        std::is_floating_point_v<std::remove_reference_t<T>>>> {
  typedef typename __1::get_hash_code_len<T>::type code_type;

  hash_code_helper(T&& t) {
    __1::ptr_getter getter;
    getter.ptr = &t;
    code = 1llu * (*reinterpret_cast<const typename __1::get_bytes<T>::type*>(
                      getter.ptr));
  }

  code_type get() const { return code; }

 private:
  code_type code;
};

template <typename T, std::size_t N>
struct hash_code_helper<T (&)[N]> {
  typedef typename __1::get_hash_code_len<T>::type code_type;

  hash_code_helper(T (&t)[N]) {
    code = 0;
    for (std::size_t i = 0; i < N; ++i) {
      code = code * __1::HASH_FACTOR + hash_code_helper<T&>(t[i]).get();
    }
  }

  code_type get() const { return code; }

 private:
  code_type code;
};

template <typename T, std::size_t N>
struct hash_code_helper<T(&&)[N]> {
  typedef typename __1::get_hash_code_len<T>::type code_type;

  hash_code_helper(T(&&t)[N]) {
    code = 0;
    for (std::size_t i = 0; i < N; ++i) {
      code = code * __1::HASH_FACTOR +
             hash_code_helper<T&&>(std::move(t[i])).get();
    }
  }

  code_type get() const { return code; }

 private:
  code_type code;
};

template <std::size_t N>
struct hash_code_helper<char (&)[N]> {
  typedef typename __1::get_hash_code_len<char>::type code_type;

  hash_code_helper(char (&t)[N]) {
    code = 0;
    for (std::size_t i = 0; t[i] != '\0'; ++i) {
      code = code * __1::HASH_FACTOR + hash_code_helper<char&>(t[i]).get();
    }
  }

  code_type get() const { return code; }

 private:
  code_type code;
};

template <std::size_t N>
struct hash_code_helper<char(&&)[N]> {
  typedef typename __1::get_hash_code_len<char>::type code_type;

  hash_code_helper(char(&&t)[N]) {
    code = 0;
    for (std::size_t i = 0; t[i] != '\0'; ++i) {
      code = code * __1::HASH_FACTOR +
             hash_code_helper<char&&>(std::move(t[i])).get();
    }
  }

  code_type get() const { return code; }

 private:
  code_type code;
};

template <typename T>
struct hash_code_helper<
    T, std::enable_if_t<std::is_same_v<
           std::string, std::remove_cv_t<std::remove_reference_t<T>>>>> {
  typedef typename __1::get_hash_code_len<char>::type code_type;

  hash_code_helper(T&& t) {
    code = 0;
    for (std::size_t i = 0; t[i] != '\0'; ++i) {
      code =
          code * __1::HASH_FACTOR + hash_code_helper<const char&>(t[i]).get();
    }
  }

  code_type get() const { return code; }

 private:
  code_type code;
};

template <typename T>
decltype(auto) hash_code(T&& t) {
  return hash_code_helper<T&&>(std::forward<T>(t)).get();
}

uint64_t hash_code(const char* const ptr) {
  uint64_t code = 0;
  for (std::size_t i = 0; ptr[i] != '\0'; ++i) {
    code = code * __1::HASH_FACTOR + hash_code(ptr[i]);
  }
  return code;
}

}  // namespace sun::util

#endif  // SUN_UTIL_HASH_H_