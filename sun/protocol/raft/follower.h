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
  uint64_t _term;          // ��ǰ����
  uint64_t _index_idx;     // ��һ��д���λ��
  uint64_t _commit_idx;    // �Ѿ��ύ��λ��
  uint64_t _last_applied;  // �Ѿ�Ӧ�õ�λ��
  leader_ptr _leader;
};  // class follower

using follower_ptr = std::shared_ptr<follower>;

follower_ptr create_follower(const uint64_t id);

}  // namespace sun::protocol::raft

#endif  // SUN_PROTOCOL_RAFT_FOLLOWER_H_