/**
 * @file array_test.cc
 * @author sun-moon-star-star
 */

#include "sun/data_structure/array.h"

#include <gtest/gtest.h>

class ArrayTest : public testing::Test {};

using sun::data_structure::array;

TEST_F(ArrayTest, test_int) {
  array<int> a;
  ASSERT_EQ(a.size(), 0llu);

  ASSERT_EQ(a.push_back(1), 0);

  ASSERT_EQ(a.size(), 1llu);
  ASSERT_EQ(a.alloc_size(), 8llu);

  ASSERT_EQ(a.push_back(2), 0);
  ASSERT_EQ(a.push_back(3), 0);
  ASSERT_EQ(a.push_back(4), 0);
  ASSERT_EQ(a.push_back(5), 0);
  ASSERT_EQ(a.push_back(6), 0);
  ASSERT_EQ(a.push_back(7), 0);
  ASSERT_EQ(a.push_back(8), 0);
  ASSERT_EQ(a.push_back(9), 0);

  ASSERT_EQ(a.alloc_size(), 12llu);
  ASSERT_EQ(a.size(), 9llu);

  ASSERT_EQ(a.pop_back(), 0);
  ASSERT_EQ(a.alloc_size(), 12llu);
  ASSERT_EQ(a.size(), 8llu);

  a.fixed();
  ASSERT_EQ(a.alloc_size(), 8llu);
}

TEST_F(ArrayTest, test_capcity) {
  array<int> a(0);
  ASSERT_EQ(a.size(), 0llu);
  ASSERT_EQ(a.push_back(1), -1);

  array<int> b(2);
  ASSERT_EQ(b.size(), 0llu);
  ASSERT_EQ(b.push_back(1), 0);
  ASSERT_EQ(b.push_back(2), 0);
  ASSERT_EQ(b.push_back(3), -1);
  ASSERT_EQ(b.size(), 2llu);
  ASSERT_EQ(b.alloc_size(), 2llu);
}

struct A {
  A() { ++construct_cnt; }

  ~A() { ++destruct_cnt; }

  A(const A&) { ++copy_construct_cnt; }

  A(A&&) { ++move_construct_cnt; }

  A& operator=(const A&) {
    ++assign_cnt;
    return *this;
  }

  A& operator=(A&&) {
    ++move_assign_cnt;
    return *this;
  }

  static int construct_cnt;
  static int destruct_cnt;
  static int copy_construct_cnt;
  static int move_construct_cnt;
  static int assign_cnt;
  static int move_assign_cnt;
};

int A::construct_cnt = 0llu;
int A::destruct_cnt = 0llu;
int A::copy_construct_cnt = 0llu;
int A::move_construct_cnt = 0llu;
int A::assign_cnt = 0llu;
int A::move_assign_cnt = 0llu;

TEST_F(ArrayTest, test_struct) {
  {
    array<A> a;
    A b;
    ASSERT_EQ(A::construct_cnt, 1llu);

    a.push_back(std::move(b));
    ASSERT_EQ(a.alloc_size(), 8llu);
    ASSERT_EQ(a.size(), 1llu);
    ASSERT_EQ(A::copy_construct_cnt, 0llu);
    ASSERT_EQ(A::move_construct_cnt, 1llu);
  }

  ASSERT_EQ(A::destruct_cnt, 2llu);

  {
    array<A> a;
    A b, c;
    ASSERT_EQ(A::construct_cnt, 3llu);

    a.push_back(b);
    ASSERT_EQ(A::copy_construct_cnt, 1llu);
    ASSERT_EQ(A::move_construct_cnt, 1llu);

    a[0] = c;
    ASSERT_EQ(A::assign_cnt, 1llu);
    a[1] = std::move(c);  // error use !!!
    ASSERT_EQ(A::move_assign_cnt, 1llu);
    ASSERT_EQ(a.alloc_size(), 8llu);
    ASSERT_EQ(a.size(), 1llu);
  }

  ASSERT_EQ(A::destruct_cnt, 5llu);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
