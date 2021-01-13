/**
 * @file hash.h
 * @author sun-moon-star-star
 */

/*
#include "sun/util/hash.h"

struct A {
  A(int i, double d, std::string str) : i(i), d(d), str(str) {}
  uint64_t hashcode() const {
    uint64_t code = ::hashcode(i);
    code += ::hashcode(d);
    code += ::hashcode(str);
    return code;
  }

  int i;
  double d;
  std::string str;
};

register_hashcode(A);

TEST_F(HashTest, test_struct) {
  A a(1, 9903.03, "zhao");
  uint64_t code = ::sun::util::hashcode(a);
}
*/

#ifndef SUN_UTIL_HASH_H_
#define SUN_UTIL_HASH_H_

#include <cstdint>
#include <string>
#include <type_traits>

namespace sun::util {

namespace __hash {

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
struct get_hashcode_len {
  typedef uint64_t type;
};

#ifndef _LIBCPP_HAS_NO_INT128
template <typename T>
struct get_hashcode_len<T, 16> {
  typedef __uint128_t type;
};
#endif

template <class T, uint8_t bytes, std::size_t N>
struct get_hashcode_len<T[N], bytes> {
  typedef get_hashcode_len<T> type;
};

union ptr_getter {
  const void* ptr;
  uint64_t code;
};  // union ptr_getter

const uint64_t HASH_FACTOR = 11131;

}  // namespace __hash

/**
 * @brief hashcode
 */
template <typename T, typename V = void>
struct hashcode_helper {
  hashcode_helper(T&& t) {
    uint64_t len = sizeof(t);
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&t);
    code = 0;
    for (uint64_t i = 0; i < len; ++i, ++ptr) {
      code = code * __hash::HASH_FACTOR + (*ptr);
    };
  }
  uint64_t operator()() const { return code; }

 private:
  uint64_t code;
};

template <typename T>
struct hashcode_helper<
    T, std::enable_if_t<std::is_integral_v<std::remove_reference_t<T>> ||
                        std::is_floating_point_v<std::remove_reference_t<T>>>> {
  typedef typename __hash::get_hashcode_len<T>::type code_type;

  hashcode_helper(T&& t) {
    __hash::ptr_getter getter;
    getter.ptr = &t;
    code =
        1llu * (*reinterpret_cast<const typename __hash::get_bytes<T>::type*>(
                   getter.ptr));
  }

  code_type operator()() const { return code; }

 private:
  code_type code;
};

template <typename T, std::size_t N>
struct hashcode_helper<T (&)[N]> {
  typedef typename __hash::get_hashcode_len<T>::type code_type;

  hashcode_helper(T (&t)[N]) {
    code = 0;
    for (std::size_t i = 0; i < N; ++i) {
      code = code * __hash::HASH_FACTOR + hashcode_helper<T&>(t[i])();
    }
  }

  code_type operator()() const { return code; }

 private:
  code_type code;
};

template <typename T, std::size_t N>
struct hashcode_helper<T(&&)[N]> {
  typedef typename __hash::get_hashcode_len<T>::type code_type;

  hashcode_helper(T(&&t)[N]) {
    code = 0;
    for (std::size_t i = 0; i < N; ++i) {
      code =
          code * __hash::HASH_FACTOR + hashcode_helper<T&&>(std::move(t[i]))();
    }
  }

  code_type operator()() const { return code; }

 private:
  code_type code;
};

template <std::size_t N>
struct hashcode_helper<char (&)[N]> {
  typedef typename __hash::get_hashcode_len<char>::type code_type;

  hashcode_helper(char (&t)[N]) {
    code = 0;
    for (std::size_t i = 0; t[i] != '\0'; ++i) {
      code = code * __hash::HASH_FACTOR + hashcode_helper<char&>(t[i])();
    }
  }

  code_type operator()() const { return code; }

 private:
  code_type code;
};

template <std::size_t N>
struct hashcode_helper<char(&&)[N]> {
  typedef typename __hash::get_hashcode_len<char>::type code_type;

  hashcode_helper(char(&&t)[N]) {
    code = 0;
    for (std::size_t i = 0; t[i] != '\0'; ++i) {
      code = code * __hash::HASH_FACTOR +
             hashcode_helper<char&&>(std::move(t[i]))();
    }
  }

  code_type operator()() const { return code; }

 private:
  code_type code;
};

template <typename T>
struct hashcode_helper<
    T, std::enable_if_t<std::is_same_v<
           std::string, std::remove_cv_t<std::remove_reference_t<T>>>>> {
  typedef typename __hash::get_hashcode_len<char>::type code_type;

  hashcode_helper(T&& t) {
    code = 0;
    for (std::size_t i = 0; t[i] != '\0'; ++i) {
      code = code * __hash::HASH_FACTOR + hashcode_helper<const char&>(t[i])();
    }
  }

  code_type operator()() const { return code; }

 private:
  code_type code;
};

template <typename T>
decltype(auto) hashcode(T&& t) {
  return hashcode_helper<T&&>(std::forward<T>(t))();
}

uint64_t hashcode(const char* const ptr) {
  uint64_t code = 0;
  for (std::size_t i = 0; ptr[i] != '\0'; ++i) {
    code = code * __hash::HASH_FACTOR + hashcode(ptr[i]);
  }
  return code;
}

template <typename T>
struct hasher {
  uint64_t operator()(const T& t) { return hashcode(t); }
};  // hasher

#define register_hashcode(class_name)                                      \
  template <typename T>                                                    \
  struct sun::util::hashcode_helper<                                       \
      T, std::enable_if_t<std::is_same_v<                                  \
             class_name, std::remove_cv_t<std::remove_reference_t<T>>>>> { \
    hashcode_helper(T&& t) : code(t.hashcode()) {}                         \
                                                                           \
    uint64_t operator()() const { return code; }                           \
                                                                           \
   private:                                                                \
    uint64_t code;                                                         \
  }

}  // namespace sun::util

#endif  // SUN_UTIL_HASH_H_