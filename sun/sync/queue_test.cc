/**
 * @file queue_test.cc
 * @author sun-moon-star-star
 */

#include "sun/sync/queue.h"

#include <thread>

#include <gtest/gtest.h>

class QueueTest : public testing::Test {};

using sun::sync::queue;

TEST_F(QueueTest, test_case) {
  const int test_cnt = 100;

  uint64_t produce_cnt = 0llu;

  std::thread producer[test_cnt];
  std::thread consumer;

  queue<> q(1);

  auto produce_fun = [&q, &produce_cnt]() {
      void *ptr = nullptr;
      q.push(ptr);
      ++produce_cnt;
  };

  for (int i = 0; i < test_cnt; ++i) {
    producer[i] = std::thread(produce_fun);
  }

  void *ptr = nullptr;
  for (int i = 0;i < test_cnt; ++i) {
    void *data;
    int sum = produce_cnt;
    ASSERT_TRUE(sum == i || sum == i + 1);
    q.pop(&data);
    ASSERT_EQ(data, ptr);
  }

  for (int i = 0; i < test_cnt; ++i) {
    producer[i].join();
  }
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
