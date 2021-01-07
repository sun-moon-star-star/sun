/**
 * @file singleton_test.cc
 * @author sun-moon-star-star
 */

#include "sun/util/singleton.h"

#include <string>

#include <gtest/gtest.h>

class SingletonTest : public testing::Test {};

using sun::util::singleton;

TEST_F(SingletonTest, test_singleton) {
  ASSERT_EQ(reinterpret_cast<void*>(singleton<int>::get_instance()),
            reinterpret_cast<void*>(singleton<int>::get_instance()));
  ASSERT_NE(reinterpret_cast<void*>(singleton<int>::get_instance()),
            reinterpret_cast<void*>(singleton<double>::get_instance()));
  ASSERT_EQ(reinterpret_cast<void*>(singleton<double>::get_instance()),
            reinterpret_cast<void*>(singleton<double>::get_instance()));
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}