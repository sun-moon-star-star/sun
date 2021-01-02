/**
 * @file accepter.cc
 * @author sun-moon-star-star
 */

#include "sun/protocol/paxos/accepter.h"

#include "sun/util/common.h"

namespace sun::protocol::paxos {

outcome_ptr accepter::prepare(proposal_sign_ptr proposal_sign) {
  const uint64_t proposal_id = proposal_sign->id;
  const uint64_t proposal_new_value = proposal_sign->value;
  uint64_t promise_value = this->get_proposal_value(proposal_id);
  uint64_t hashcode = 0;
  uint64_t accept_value = 0;
  error code;

  if (promise_value) {
    proposal_ptr proposal = get_proposal(proposal_id);
    hashcode = proposal->hashcode;
    accept_value = proposal->value;
  }

  if (promise_value < proposal_new_value) {
    set_proposal_value(proposal_id, proposal_new_value);
    promise_value = proposal_new_value;
    code = code::ok;
  } else {
    code = code::fail;
  }

  return create_outcome(proposal_id, proposal_new_value, promise_value,
                        accept_value, hashcode, code);
}

outcome_ptr accepter::accept(proposal_ptr proposal) {
  const uint64_t proposal_id = proposal->id;
  const uint64_t proposal_new_value = proposal->value;
  uint64_t promise_value = this->get_proposal_value(proposal_id);
  uint64_t hashcode = 0;
  error code;

  if (promise_value) {
    hashcode = get_proposal(proposal_id)->hashcode;
  }

  if (promise_value <= proposal_new_value) {
    if (promise_value != proposal_new_value) {
      set_proposal_value(proposal_id, proposal_new_value);
    }
    save_proposal(proposal);
    code = code::ok;
  } else {
    code = code::fail;
  }

  return create_outcome(proposal_id, proposal_new_value, proposal_new_value,
                        proposal_new_value, hashcode, code);
}

void accepter::save_proposal(proposal_ptr proposal) {
  _proposals[proposal->id] = proposal;
}

void accepter::set_proposal_value(uint64_t proposal_id,
                                  uint64_t proposal_value) {
  _proposal_signs[proposal_id] = proposal_value;
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
  const uint64_t accepter_id = sun::util::common::random::random<uint64_t>();
  return std::make_shared<accepter>(accepter_id);
}

}  // namespace sun::protocol::paxos