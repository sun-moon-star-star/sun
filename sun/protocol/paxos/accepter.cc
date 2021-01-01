/**
 * @file accepter.cc
 * @author sun-moon-star-star
 */

#include "sun/protocol/paxos/accepter.h"

namespace sun::protocol::paxos {

outcome_ptr accepter::prepare(proposal_sign_ptr proposal) {
  const uint64_t proposal_id = proposal->id;
  const uint64_t proposal_new_value = proposal->value;
  uint64_t proposal_old_value = this->get_proposal_value(proposal_id);

  accepter_code code;
  if (proposal_old_value < proposal_new_value) {
    set_proposal_value(proposal_id, proposal_new_value);
    proposal_old_value = proposal_new_value;
    code = code::accepter::promise;
  } else {
    code = code::accepter::obsolete;
  }

  return create_outcome(proposal_id, proposal_new_value, proposal_old_value,
                        code);
}

outcome_ptr accepter::accept(proposal_ptr proposal) {
  const uint64_t proposal_id = proposal->id;
  const uint64_t proposal_new_value = proposal->value;
  uint64_t proposal_old_value = this->get_proposal_value(proposal_id);

  accepter_code code;
  if (proposal_old_value == proposal_new_value) {
    save_proposal(proposal);
    code = code::accepter::accept;
  } else {
    code = code::accepter::update;
  }

  return create_outcome(proposal_id, proposal_new_value, proposal_old_value,
                        code);
}

void accepter::save_proposal(proposal_ptr proposal) {
  _proposals[proposal->id] = proposal;
}

void accepter::set_proposal_value(uint64_t proposal_id,
                                  uint64_t proposal_value) {
  _proposal_signs[proposal_id] = proposal_value;
}

proposal_ptr accepter::get_proposal(uint64_t proposal_id) {
  auto it = _proposals.find(proposal_id);
  if (it == _proposals.end()) {
    return nullptr;
  }
  return it->second;
}

// 协议不存在返回0
uint64_t accepter::get_proposal_value(uint64_t proposal_id) {
  auto it = _proposal_signs.find(proposal_id);
  if (it == _proposal_signs.end()) {
    return 0llu;
  }
  return it->second;
}

accepter_ptr create_accepter() {
  // todo(random)
  static uint64_t global_accepter_id = 0;
  return accepter_ptr(new accepter(++global_accepter_id));
}

}  // namespace sun::protocol::paxos