/**
 * @file follower.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_RAFT_FOLLOWER_H_
#define SUN_PROTOCOL_RAFT_FOLLOWER_H_

#include "sun/protocol/raft/leader.h"
#include "sun/protocol/raft/node_id.h"

#include <cstdint>

namespace sun::protocol::raft {
class follower {
 public:
  node_id id;  // equal to server.id

  follower() {}

 private:
  uint64_t _term;          // 当前任期
  uint64_t _index_idx;     // 下一次写入的位置
  uint64_t _commit_idx;    // 已经提交的位置
  uint64_t _last_applied;  // 已经应用的位置
  leader_ptr _leader;
  node_id _voted_for;  // 投票过的节点
  uint64_t _election_timeout;
};  // class follower

using follower_ptr = std::shared_ptr<follower>;

follower_ptr create_follower(const uint64_t id);

}  // namespace sun::protocol::raft

#endif  // SUN_PROTOCOL_RAFT_FOLLOWER_H_