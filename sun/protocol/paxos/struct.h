/**
 * @file struct.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_PAXOS_STRUCT_H_
#define SUN_PROTOCOL_PAXOS_STRUCT_H_

#include "sun/protocol/paxos/code.h"
#include "sun/util/common.h"

#include <cstdint>
#include <memory>

namespace sun::protocol::paxos {

/**
 * @struct proposal_sign
 * @note 在prepare阶段，proposal向accepter发送请求时携带的数据
 */
struct proposal_sign {
  uint64_t id;     // 提议的针对对象
  uint64_t value;  // 提议提出的值

  NO_CREATABLE(proposal_sign);
};

using proposal_sign_ptr = std::shared_ptr<proposal_sign>;

proposal_sign_ptr create_proposal_sign(const uint64_t& id,
                                       const uint64_t& value);

/**
 * @struct proposal
 * @brief proposal提出的一次提议
 * @note 在accept阶段，proposal向accepter发送请求时携带的数据
 */
struct proposal : public proposal_sign {
  const uint64_t len;  // 提议携带的数据的长度
  char buffer[];       // 提议携带的数据

  NO_CREATABLE(proposal);
};  // class proposer

using proposal_ptr = std::shared_ptr<proposal>;

proposal_ptr create_proposal(const uint64_t& id, const uint64_t& value,
                             const uint64_t& len);

/**
 * @struct outcome
 * @brief 最终的结果
 * @note 在prepare/accept阶段，accepter向proposal告知本协议是否被接受
 */
struct outcome {
  uint64_t id;              // 提议的针对对象
  uint64_t value;           // 提议提出的值
  uint64_t accepter_value;  // accepter目前保证的value
  const accepter_code code;

  NO_CREATABLE(outcome);
};

using outcome_ptr = std::shared_ptr<outcome>;

outcome_ptr create_outcome(const uint64_t& id, const uint64_t& value,
                           const uint64_t& accepter_value,
                           const accepter_code& code);

}  // namespace sun::protocol::paxos

#endif  // SUN_PROTOCOL_PAXOS_STRUCT_H_