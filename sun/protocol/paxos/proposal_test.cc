/**
 * @file proposal_test.cc
 * @author sun-moon-star-star
 */

#include "sun/protocol/paxos/proposal.h"

#include <gtest/gtest.h>

class ProposalTest : public testing::Test {};

TEST_F(ProposalTest, test_proposal) {
  sun::protocol::paxos::proposal_ptr proposal =
      sun::protocol::paxos::create_proposal(123456789llu, 1234llu, 0llu);
  ASSERT_NE(proposal, nullptr);
  ASSERT_EQ(proposal->id, 123456789llu);
  ASSERT_EQ(proposal->value, 1234llu);
  ASSERT_EQ(proposal->len, 0llu);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}