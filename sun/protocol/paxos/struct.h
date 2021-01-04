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

  proposal_sign(const uint64_t id, const uint64_t value)
      : id(id), value(value) {}
};

using proposal_sign_ptr = std::shared_ptr<proposal_sign>;

proposal_sign_ptr create_proposal_sign(const uint64_t id, const uint64_t value);

/**
 * @struct proposal
 * @brief proposal提出的一次提议
 * @note 在accept阶段，proposal向accepter发送请求时携带的数据
 */
struct proposal final : public proposal_sign {
  uint64_t len;       // 提议携带的数据的长度
  void* buffer;       // 提议携带的数据
  uint64_t hashcode;  // 标识数据的唯一性

  proposal(const uint64_t id, const uint64_t value, const uint64_t len,
           void* const buffer, const uint64_t hashcode)
      : proposal_sign(id, value),
        len(len),
        buffer(buffer),
        hashcode(hashcode) {}

  ~proposal() {
    if (buffer != nullptr) {
      free(buffer);
      buffer = nullptr;
    }
  }

  NO_COPYABLE(proposal);
};  // class proposer

using proposal_ptr = std::shared_ptr<proposal>;

proposal_ptr create_proposal(const uint64_t id, const uint64_t value,
                             const uint64_t len, void* const buffer,
                             const uint64_t hashcode);

/**
 * @struct outcome
 * @brief 最终的结果
 * @note 在prepare/accept阶段，accepter向proposal告知本协议是否被接受
 */
struct outcome : public proposal_sign {
  uint64_t promise_value;    // accepter目前已被设置的value
  uint64_t accept_value;     // accepter目前保证的value
  uint64_t accept_hashcode;  // accepter目前保证的value对应的hashcode
  error code;

  outcome(const uint64_t id, const uint64_t value, const uint64_t promise_value,
          const uint64_t accept_value, const uint64_t accept_hashcode,
          const error code)
      : proposal_sign(id, value),
        promise_value(promise_value),
        accept_value(accept_value),
        accept_hashcode(accept_hashcode),
        code(code) {}
};

using outcome_ptr = std::shared_ptr<outcome>;

outcome_ptr create_outcome(const uint64_t id, const uint64_t value,
                           const uint64_t promise_value,
                           const uint64_t accept_value,
                           const uint64_t accept_hashcode, const error code);

}  // namespace sun::protocol::paxos

#endif  // SUN_PROTOCOL_PAXOS_STRUCT_H_