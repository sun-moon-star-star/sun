/**
 * @file proposer.cc
 * @author sun-moon-star-star
 */

#include "sun/protocol/paxos/proposer.h"

#include "sun/protocol/paxos/code.h"
#include "sun/protocol/paxos/config.h"

namespace sun::protocol::paxos {

error proposer::propose(std::vector<accepter_ptr>& accepters,
                        std::vector<outcome_ptr>& outcomes,
                        proposal_ptr& proposal, uint64_t& max_set_value,
                        proposal_sign_ptr proposal_sign) {
  uint64_t max_value = proposal_sign->value;
  uint64_t promise_count = 0;
  uint64_t response_count = 0;
  accepter_ptr max_accepter = nullptr;

  max_set_value = proposal_sign->value;

  for (auto accepter : accepters) {
    outcome_ptr outcome = accepter->prepare(proposal_sign);

    outcomes.push_back(outcome);
    if (outcome->code == code::ok) {
      ++promise_count;
      ++response_count;
    } else if (outcome->code == code::fail) {
      if (outcome->accept_value > max_value) {
        max_value = outcome->accept_value;
        max_accepter = accepter;
      }
      if (outcome->promise_value > max_set_value) {
        max_set_value = outcome->promise_value;
      }
      ++response_count;
    } else if (outcome->code == code::fail_communication) {
      // log
    }
  }

  if (promise_count >= config::commit_need_num) {
    return code::ok;
  }

  if (response_count < config::commit_need_num) {
    return code::fail_communication;
  }

  proposal = max_accepter->get_proposal(proposal_sign->id);
  return code::fail;
}

error proposer::commit(std::vector<accepter_ptr>& accepters,
                       std::vector<outcome_ptr>& outcomes,
                       proposal_ptr& proposal) {
  uint64_t commit_count = 0;
  uint64_t response_count = 0;
  for (int i = 0; i < outcomes.size(); ++i) {
    if (outcomes[i]->code == code::fail_communication ||
        outcomes[i]->accept_hashcode != proposal->hashcode) {
      outcome_ptr outcome = accepters[i]->accept(proposal);
      if (outcome->code == code::ok) {
        ++commit_count;
        ++response_count;
      } else if (outcome->code == code::fail) {
        ++response_count;
      }
    }
  }

  if (commit_count >= config::commit_need_num) {
    return code::ok;
  }

  if (response_count < config::commit_need_num) {
    return code::fail_communication;
  }

  return code::fail;
}

proposer_ptr create_proposer() {
  const uint64_t proposer_id = sun::util::common::random::random<uint64_t>();
  return std::make_shared<proposer>(proposer_id);
}

}  // namespace sun::protocol::paxos