/**
 * @file hash_test.cc
 * @author sun-moon-star-star
 */

#include "sun/util/hash.h"

#include <string>

#include <gtest/gtest.h>

class HashTest : public testing::Test {};

using sun::util::hashcode;

TEST_F(HashTest, test_int) {
  ASSERT_EQ(hashcode(990303), 990303llu);
  ASSERT_EQ(hashcode(990303u), 990303llu);
  ASSERT_EQ(hashcode(990303ll), 990303llu);
  ASSERT_EQ(hashcode(990303llu), 990303llu);

#ifndef _LIBCPP_HAS_NO_INT128
  uint64_t i64 = __LONG_LONG_MAX__;
  __uint128_t i128 = i64;

  ASSERT_EQ(hashcode(i64), hashcode(i128));
  ASSERT_EQ(typeid(hashcode(i64)), typeid(uint64_t));

  i128 *= 1000;
  i64 *= 1000;

  ASSERT_NE(hashcode(i64), hashcode(i128));
#endif
}

TEST_F(HashTest, test_float) {
  ASSERT_EQ(hashcode(99.0303f), hashcode(99.0303f));
  ASSERT_NE(hashcode(99.0303f), hashcode(99.0303));
  ASSERT_EQ(hashcode(99.0303), hashcode(99.0303));

  float f = 99.0303;
  double d = 99.0303;

  ASSERT_NE(hashcode(f), hashcode(99.0303));
  ASSERT_NE(hashcode(d), hashcode(99.0303f));
}

TEST_F(HashTest, test_char_pointer) {
  char str[] = "hello";
  std::string str2 = "hello";
  const std::string str3 = "hello";
  std::string& str4 = str2;
  const std::string& str5 = "hello";

  ASSERT_EQ(hashcode("hello"), hashcode("hello"));
  ASSERT_EQ(hashcode("hello"), hashcode(std::string("hello")));
  ASSERT_EQ(hashcode("hello"), hashcode(str));
  ASSERT_EQ(hashcode("hello"), hashcode(str2));
  ASSERT_EQ(hashcode("hello"), hashcode(str3));
  ASSERT_EQ(hashcode("hello"), hashcode(str4));
  ASSERT_EQ(hashcode("hello"), hashcode(str5));
  ASSERT_NE(hashcode("hello"), hashcode("world"));
}

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
  A& b = a;
  const A c = a;
  const A& d = a;

  uint64_t hashcode_v = hashcode(1);
  hashcode_v += hashcode(9903.03);
  hashcode_v += hashcode("zhao");

  ASSERT_EQ(hashcode(a), hashcode_v);
  ASSERT_EQ(hashcode(a), hashcode(b));
  ASSERT_EQ(hashcode(a), hashcode(c));
  ASSERT_EQ(hashcode(a), hashcode(d));
}

TEST_F(HashTest, test_struct_array) {
  A a(1, 9903.03, "zhao");
  A b(2, 0208.21, "yu");

  A array[] = {a, b};
  A array2[2] = {A{1, 9903.03, "zhao"}, A{2, 0208.21, "yu"}};

  ASSERT_EQ(hashcode(array), hashcode(array2));
}

TEST_F(HashTest, test_struct_not_define_hashcode) {
  char buffer[1024] = {0};
  char buffer2[1024] = {0};
  memset(buffer2, 0xff, sizeof(buffer2));

  struct lu {
    int i;
    long long k;
  };
  lu* x = new (buffer) lu{1, 2};
  lu* y = new (buffer2) lu{1, 2};
  ASSERT_NE(hashcode(x), hashcode(y));

  struct zhao {
    int i, j;
    long long k;
  } x2{1, 2, 3}, y2{1, 2, 3}, z2{3, 2, 1};
  ASSERT_EQ(hashcode(x2), hashcode(y2));
  ASSERT_NE(hashcode(x2), hashcode(z2));

  zhao* x3 = new (buffer) zhao{1, 2, 3};
  zhao* y3 = new (buffer) zhao{1, 2, 3};
  ASSERT_EQ(hashcode(x3), hashcode(y3));
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}