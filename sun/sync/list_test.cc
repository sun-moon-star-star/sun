/**
 * @file list_test.cc
 * @author sun-moon-star-star
 */

#include "sun/sync/list.h"

#include <thread>

#include <gtest/gtest.h>

class ListTest : public testing::Test {};

using sun::sync::list;

TEST_F(ListTest, test_case) {
  list<> l;
  ASSERT_EQ(l.size(), 0llu);

  const int test_cnt = 1000;

  std::thread t[test_cnt];
  for (int i = 0; i < test_cnt; ++i) {
    t[i] = std::thread([&l]() { l.push_back(nullptr); });
  }

  for (int i = 0; i < test_cnt; ++i) {
    t[i].join();
  }

  ASSERT_EQ(l.size(), test_cnt);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
