/**
 * @file accepter.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_PAXOS_ACCEPTER_H_
#define SUN_PROTOCOL_PAXOS_ACCEPTER_H_

#include "sun/protocol/paxos/code.h"
#include "sun/protocol/paxos/struct.h"

#include <memory>
#include <unordered_map>

namespace sun::protocol::paxos {

class accepter {
 public:
  uint64_t id;  // accepter 唯一标识

  accepter(const uint64_t& id) : id(id) {}

  outcome_ptr prepare(proposal_sign_ptr proposal);

  outcome_ptr accept(proposal_ptr proposal);

  proposal_ptr get_proposal(uint64_t proposal_id);

 private:
  void save_proposal(proposal_ptr proposal);

  void set_proposal_value(uint64_t proposal_id, uint64_t proposal_value);

    // 协议不存在返回0
  uint64_t get_proposal_value(uint64_t proposal_id);

 private:
  // proposal_id -> proposal_value
  std::unordered_map<uint64_t, uint64_t> _proposal_signs;
  // proposal_id  -> proposal
  std::unordered_map<uint64_t, proposal_ptr> _proposals;

};  // class accepter

using accepter_ptr = std::shared_ptr<accepter>;

accepter_ptr create_accepter();

}  // namespace sun::protocol::paxos

#endif  // SUN_PROTOCOL_PAXOS_ACCEPTER_H_