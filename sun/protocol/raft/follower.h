/**
 * @file follower.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_RAFT_FOLLOWER_H_
#define SUN_PROTOCOL_RAFT_FOLLOWER_H_

#include "sun/protocol/raft/leader.h"

#include <cstdint>

namespace sun::protocol::raft {
class follower {
 public:
  const uint64_t id;  // equal to server.id

  follower(const uint64_t id) : id(id) {}

  friend class leader;

 private:
  uint64_t _write_idx;
  uint64_t _commit_idx;
  leader_ptr _leader;
};  // class follower

using follower_ptr = std::shared_ptr<follower>;

follower_ptr create_follower(const uint64_t id);

}  // namespace sun::protocol::raft

#endif  // SUN_PROTOCOL_RAFT_FOLLOWER_H_