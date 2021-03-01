/**
 * @file buffer_test.cc
 * @author sun-moon-star-star
 */

#include "sun/rpc/buffer.h"

#include "sun/util/common.h"

#include <string>

#include <gtest/gtest.h>

class BufferTest : public testing::Test {};

TEST_F(BufferTest, test_base) {
  auto buffer = ::sun::rpc::create_buffer(12);

  ASSERT_EQ(buffer->alloc_size, 12);
}

TEST_MAIN