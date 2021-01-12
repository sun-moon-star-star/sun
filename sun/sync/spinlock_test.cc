/**
 * @file spinlock_test.cc
 * @author sun-moon-star-star
 */

#include "sun/sync/spinlock.h"

#include <thread>

#include <gtest/gtest.h>

class SpinlockTest : public testing::Test {};

using sun::sync::spinlock;

TEST_F(SpinlockTest, test_case) {
  spinlock lock;

  ASSERT_TRUE(lock.try_lock());
  ASSERT_TRUE(lock.locked());
  ASSERT_FALSE(lock.unlocked());
  ASSERT_FALSE(lock.try_lock());

  std::thread t([&lock]() { ASSERT_FALSE(lock.try_lock()); });
  t.join();

  lock.unlock();
  ASSERT_FALSE(lock.locked());
  ASSERT_TRUE(lock.unlocked());
  t = std::thread([&lock]() { ASSERT_TRUE(lock.try_lock()); });
  t.join();

  ASSERT_FALSE(lock.try_lock());
  lock.unlock();
  ASSERT_FALSE(lock.locked());
  ASSERT_TRUE(lock.unlocked());
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
