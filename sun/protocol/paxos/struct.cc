/**
 * @file struct.cc
 * @author sun-moon-star-star
 */

#include "sun/protocol/paxos/struct.h"

#include "sun/util/common.h"

namespace sun::protocol::paxos {

proposal_sign_ptr create_proposal_sign(const uint64_t& id,
                                       const uint64_t& value) {
  return std::make_shared<proposal_sign>(id, value);
}

proposal_ptr create_proposal(const uint64_t id, const uint64_t value,
                             const uint64_t len, void* const buffer,
                             const uint64_t hashcode) {
  return std::make_shared<proposal>(id, value, len, buffer, hashcode);
}

outcome_ptr create_outcome(const uint64_t& id, const uint64_t& value,
                           const uint64_t& promise_value,
                           const uint64_t& accept_value,
                           const uint64_t& accept_hashcode, const error& code) {
  return std::make_shared<outcome>(id, value, promise_value, accept_value,
                                   accept_hashcode, code);
}

}  // namespace sun::protocol::paxos