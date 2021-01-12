/**
 * @file leader.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_RAFT_LEADER_H_
#define SUN_PROTOCOL_RAFT_LEADER_H_

#include "sun/data_structure/array.h"
#include "sun/protocol/raft/follower.h"
#include "sun/protocol/raft/node_id.h"

#include <cstdint>

namespace sun::protocol::raft {

namespace __leader {

struct follower_data {
  uint64_t next_idx;
  uint64_t match_idx;
  follower_ptr follower;
};

}  // namespace __leader

class leader {
 public:
  node_id id;  // equal to server.id

  leader() {}

 private:
  uint64_t _term;          // 当前任期
  uint64_t _next_idx;      // 下一次写入的位置
  uint64_t _commit_idx;    // 已经提交的位置
  uint64_t _last_applied;  // 已经应用的位置
  ::sun::data_structure::array<__leader::follower_data> _followers;
};  // class leader

using leader_ptr = std::shared_ptr<leader>;

leader_ptr create_leader(const uint64_t id);

}  // namespace sun::protocol::raft

#endif  // SUN_PROTOCOL_RAFT_LEADER_H_