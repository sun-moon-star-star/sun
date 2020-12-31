/**
 * @file proposal.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_PAXOS_PROPOSAL_H_
#define SUN_PROTOCOL_PAXOS_PROPOSAL_H_

#include "sun/protocol/paxos/proposal.h"

#include <cstdint>
#include <memory>

namespace sun::protocol::paxos {

struct proposal {
  const uint64_t id;     // 提议的唯一标志
  const uint64_t value;  // 提议提出的值
  const uint64_t len;    // 提议携带的数据的长度
  char buffer[];         // 提议携带的数据

  proposal() = delete;
  proposal(const proposal&) = delete;
  proposal operator=(const proposal&) = delete;
};  // class proposer

using proposal_ptr = std::shared_ptr<proposal>;

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

}  // namespace sun::protocol::paxos

#endif  // SUN_PROTOCOL_PAXOS_PROPOSAL_H_