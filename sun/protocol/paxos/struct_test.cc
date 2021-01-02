/**
 * @file struct_test.cc
 * @author sun-moon-star-star
 */

#include "sun/protocol/paxos/struct.h"

#include "sun/protocol/paxos/code.h"

#include <gtest/gtest.h>

class StructTest : public testing::Test {};

TEST_F(StructTest, test_proposal_sign) {
  sun::protocol::paxos::proposal_sign_ptr proposal_sign =
      sun::protocol::paxos::create_proposal_sign(123456789llu, 1234llu);

  ASSERT_NE(proposal_sign, nullptr);
  ASSERT_EQ(proposal_sign->id, 123456789llu);
  ASSERT_EQ(proposal_sign->value, 1234llu);
}

TEST_F(StructTest, test_proposal) {
  sun::protocol::paxos::proposal_ptr proposal =
      sun::protocol::paxos::create_proposal(123456789llu, 1234llu, 0llu,
                                            nullptr, 0llu);
  ASSERT_NE(proposal, nullptr);
  ASSERT_EQ(proposal->id, 123456789llu);
  ASSERT_EQ(proposal->value, 1234llu);
  ASSERT_EQ(proposal->len, 0llu);
  ASSERT_EQ(proposal->buffer, nullptr);
  ASSERT_EQ(proposal->hashcode, 0llu);
}

TEST_F(StructTest, test_outcome) {
  sun::protocol::paxos::outcome_ptr outcome =
      sun::protocol::paxos::create_outcome(123456789llu, 1234llu, 1235llu,
                                           1235llu, 8765llu,
                                           sun::protocol::paxos::code::ok);
  ASSERT_NE(outcome, nullptr);

  ASSERT_EQ(outcome->id, 123456789llu);
  ASSERT_EQ(outcome->value, 1234llu);
  ASSERT_EQ(outcome->promise_value, 1235llu);
  ASSERT_EQ(outcome->accept_value, 1235llu);
  ASSERT_EQ(outcome->accept_hashcode, 8765llu);
  ASSERT_EQ(outcome->code, sun::protocol::paxos::code::ok);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}