/**
 * @file struct.cc
 * @author sun-moon-star-star
 */

#include "sun/protocol/paxos/struct.h"

namespace sun::protocol::paxos {

proposal_sign_ptr create_proposal_sign(const uint64_t& id,
                                       const uint64_t& value) {
  void* ptr = malloc(sizeof(proposal_sign));

  uint64_t* ptr_ui64 = reinterpret_cast<uint64_t*>(ptr);
  *ptr_ui64 = id;
  *(++ptr_ui64) = value;

  return proposal_sign_ptr(reinterpret_cast<proposal_sign*>(ptr),
                           [](proposal_sign* ptr) { free(ptr); });
}

proposal_ptr create_proposal(const uint64_t& id, const uint64_t& value,
                             const uint64_t& len) {
  void* ptr = malloc(sizeof(proposal) + len);

  uint64_t* ptr_ui64 = reinterpret_cast<uint64_t*>(ptr);
  *ptr_ui64 = id;
  *(++ptr_ui64) = value;
  *(++ptr_ui64) = len;

  return proposal_ptr(reinterpret_cast<proposal*>(ptr),
                      [](proposal* ptr) { free(ptr); });
}

outcome_ptr create_outcome(const uint64_t& id, const uint64_t& value,
                           const uint64_t& accepter_value,
                           const accepter_code& code) {
  void* ptr = malloc(sizeof(outcome));

  uint64_t* ptr_ui64 = reinterpret_cast<uint64_t*>(ptr);
  *(ptr_ui64) = id;
  *(++ptr_ui64) = value;
  *(++ptr_ui64) = accepter_value;

  accepter_code* ptr_code = reinterpret_cast<accepter_code*>(++ptr_ui64);
  *ptr_code = code;

  return outcome_ptr(reinterpret_cast<outcome*>(ptr),
                     [](outcome* ptr) { free(ptr); });
}

}  // namespace sun::protocol::paxos