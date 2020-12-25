/**
 * @file struct_test.cc
 * @author sun-moon-star-star
 */
#include "sun/util/struct.h"

#include <string>

#include <gtest/gtest.h>

class StructTest : public testing::Test {};

TEST_F(StructTest, test_struct) {
  struct x {
    int m1;
    int m2;
  };

  x a{1, 2}, b{3, 4};

  sun::util::update_struct_member_field<x, int, &x::m1>(&a, &b);

  ASSERT_EQ(a.m1, 3);
  ASSERT_EQ(a.m2, 2);
  ASSERT_EQ(b.m1, 3);
  ASSERT_EQ(b.m2, 4);

  sun::util::update_struct_member_field<x, int, &x::m2>(&a, &b);
  ASSERT_EQ(a.m1, 3);
  ASSERT_EQ(a.m2, 4);
  ASSERT_EQ(b.m1, 3);
  ASSERT_EQ(b.m2, 4);
}

struct y {
  int m1;
  std::string m2;
  int m3;
  char m4;
  std::string m5;
};

template <>
void sun::util::update_struct_member_field<y, int, &y::m1>(
    y* const obj, y const* const value) {
  obj->m1 = value->m1;
  obj->m2 = value->m2;
}

template <>
void sun::util::update_struct_member_field<y, int, &y::m3>(
    y* const obj, y const* const value) {
  obj->m3 = value->m3;
  obj->m4 = value->m4;
}

TEST_F(StructTest, test_struct_2) {
  y a{1, "one", 2, 'a', "five"};
  y b{3, "three", 4, 'b', "wu"};

  sun::util::update_struct_member_field<y, int, &y::m1>(&a, &b);

  ASSERT_EQ(a.m1, 3);
  ASSERT_EQ(a.m2, "three");

  sun::util::update_struct_member_field<y, int, &y::m3>(&b, &a);

  ASSERT_EQ(b.m3, 2);
  ASSERT_EQ(b.m4, 'a');

  sun::util::update_struct_member_field<y, std::string, &y::m5>(&a, &b);
  ASSERT_EQ(a.m5, "wu");
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}