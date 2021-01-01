/**
 * @file accepter_test.cc
 * @author sun-moon-star-star
 */

#include "sun/protocol/paxos/accepter.h"

#include <gtest/gtest.h>

class AccepterTest : public testing::Test {};

TEST_F(AccepterTest, test_accepter) {
  using sun::protocol::paxos::accepter_ptr;
  using sun::protocol::paxos::create_accepter;

  accepter_ptr accepter = create_accepter();
  accepter_ptr accepter_2 = create_accepter();
  ASSERT_NE(accepter->id, accepter_2->id);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}