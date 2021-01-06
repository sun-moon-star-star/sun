/**
 * @file leader.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_RAFT_LEADER_H_
#define SUN_PROTOCOL_RAFT_LEADER_H_

#include "sun/protocol/raft/follower.h"

#include <cstdint>

namespace sun::protocol::raft {

class leader {
 public:
  const uint64_t id;  // equal to server.id

  leader(const uint64_t id) : id(id) {}

 private:
  uint64_t _term;
  uint64_t _write_idx;
  uint64_t _commit_idx;
  std::set<follower_ptr> _follower;

};  // class leader

using leader_ptr = std::shared_ptr<leader>;

leader_ptr create_leader(const uint64_t id);

}  // namespace sun::protocol::raft

#endif  // SUN_PROTOCOL_RAFT_LEADER_H_