/**
 * @file hash_test.cc
 * @author sun-moon-star-star
 */

#include "sun/util/hash.h"

#include <string>

#include <gtest/gtest.h>

class HashTest : public testing::Test {};

using sun::util::hash_code;

TEST_F(HashTest, test_int) {
  ASSERT_EQ(hash_code(990303), 990303llu);
  ASSERT_EQ(hash_code(990303u), 990303llu);
  ASSERT_EQ(hash_code(990303ll), 990303llu);
  ASSERT_EQ(hash_code(990303llu), 990303llu);

#ifndef _LIBCPP_HAS_NO_INT128
  uint64_t i64 = __LONG_LONG_MAX__;
  __uint128_t i128 = i64;

  ASSERT_EQ(hash_code(i64), hash_code(i128));
  ASSERT_EQ(typeid(hash_code(i64)), typeid(uint64_t));

  i128 *= 1000;
  i64 *= 1000;

  ASSERT_NE(hash_code(i64), hash_code(i128));
#endif
}

TEST_F(HashTest, test_float) {
  ASSERT_EQ(hash_code(99.0303f), hash_code(99.0303f));
  ASSERT_NE(hash_code(99.0303f), hash_code(99.0303));
  ASSERT_EQ(hash_code(99.0303), hash_code(99.0303));

  float f = 99.0303;
  double d = 99.0303;

  ASSERT_NE(hash_code(f), hash_code(99.0303));
  ASSERT_NE(hash_code(d), hash_code(99.0303f));
}

TEST_F(HashTest, test_char_pointer) {
  char str[] = "hello";
  std::string str2 = "hello";
  const std::string str3 = "hello";
  std::string& str4 = str2;
  const std::string& str5 = "hello";

  ASSERT_EQ(hash_code("hello"), hash_code("hello"));
  ASSERT_EQ(hash_code("hello"), hash_code(std::string("hello")));
  ASSERT_EQ(hash_code("hello"), hash_code(str));
  ASSERT_EQ(hash_code("hello"), hash_code(str2));
  ASSERT_EQ(hash_code("hello"), hash_code(str3));
  ASSERT_EQ(hash_code("hello"), hash_code(str4));
  ASSERT_EQ(hash_code("hello"), hash_code(str5));
  ASSERT_NE(hash_code("hello"), hash_code("world"));
}

struct A {
  A(int i, double d, std::string str) : i(i), d(d), str(str) {}
  uint64_t hash_code() const {
    uint64_t code = ::hash_code(i);
    code += ::hash_code(d);
    code += ::hash_code(str);
    return code;
  }

  int i;
  double d;
  std::string str;
};

template <typename T>
struct sun::util::hash_code_helper<
    T, std::enable_if_t<
           std::is_same_v<A, std::remove_cv_t<std::remove_reference_t<T>>>>> {
  hash_code_helper(T&& t) : code(t.hash_code()) {}

  uint64_t get() const { return code; }

 private:
  uint64_t code;
};

TEST_F(HashTest, test_struct) {
  A a(1, 9903.03, "zhao");
  A& b = a;
  const A c = a;
  const A& d = a;

  uint64_t hash_code_v = hash_code(1);
  hash_code_v += hash_code(9903.03);
  hash_code_v += hash_code("zhao");

  ASSERT_EQ(hash_code(a), hash_code_v);
  ASSERT_EQ(hash_code(a), hash_code(b));
  ASSERT_EQ(hash_code(a), hash_code(c));
  ASSERT_EQ(hash_code(a), hash_code(d));
}

TEST_F(HashTest, test_struct_array) {
  A a(1, 9903.03, "zhao");
  A b(2, 0208.21, "yu");

  A array[] = {a, b};
  A array2[2] = {A{1, 9903.03, "zhao"}, A{2, 0208.21, "yu"}};

  ASSERT_EQ(hash_code(array), hash_code(array2));
}

TEST_F(HashTest, test_struct_not_define_hash_code) {
  char buffer[1024] = {0};
  char buffer2[1024] = {0};
  memset(buffer2, 0xff, sizeof(buffer2));

  struct lu {
    int i;
    long long k;
  };
  lu* x = new (buffer) lu{1, 2};
  lu* y = new (buffer2) lu{1, 2};
  ASSERT_NE(hash_code(x), hash_code(y));

  struct zhao {
    int i, j;
    long long k;
  } x2{1, 2, 3}, y2{1, 2, 3}, z2{3, 2, 1};
  ASSERT_EQ(hash_code(x2), hash_code(y2));
  ASSERT_NE(hash_code(x2), hash_code(z2));

  zhao* x3 = new (buffer) zhao{1, 2, 3};
  zhao* y3 = new (buffer) zhao{1, 2, 3};
  ASSERT_EQ(hash_code(x3), hash_code(y3));
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}