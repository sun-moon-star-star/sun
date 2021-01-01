/**
 * @file proposer.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_PAXOS_PROPOSER_H_
#define SUN_PROTOCOL_PAXOS_PROPOSER_H_

#include "sun/protocol/paxos/accepter.h"
#include "sun/protocol/paxos/code.h"

#include <cstdint>

namespace sun::protocol::paxos {

class proposer {
 public:
  uint64_t id;  // proposer 唯一标识

  proposer(uint64_t id) : id(id) {}

  /**
   * @brief prepare阶段 向accepter发送协议请求
   */
  error propose(std::vector<accepter_ptr>& accepters,
                std::vector<outcome_ptr>& outcomes, proposal_ptr& proposal,
                uint64_t& max_set_value, proposal_sign_ptr proposal_sign);

  /**
   * @brief accept阶段 向accepter发送协议请求
   */
  error commit(std::vector<accepter_ptr>& accepters,
               std::vector<outcome_ptr>& outcomes, proposal_ptr& proposal);

};  // class proposer

using proposer_ptr = std::shared_ptr<proposer>;

proposer_ptr create_proposer();

}  // namespace sun::protocol::paxos

#endif  // SUN_PROTOCOL_PAXOS_PROPOSER_H_