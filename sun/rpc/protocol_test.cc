/**
 * @file protocol_test.cc
 * @author sun-moon-star-star
 */

#include "sun/rpc/protocol.h"

#include "sun/util/common.h"

#include <string>

#include <gtest/gtest.h>

class ProtocolTest : public testing::Test {};

TEST_F(ProtocolTest, test_base) {
  ASSERT_EQ(sizeof(sun::rpc::header), 24);
  ASSERT_EQ(sizeof(sun::rpc::meta), 8);
  ASSERT_EQ(sizeof(sun::rpc::body), 8);
}

TEST_MAIN