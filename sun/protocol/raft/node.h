/**
 * @file node.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_RAFT_NODE_H_
#define SUN_PROTOCOL_RAFT_NODE_H_

#include <cstdint>
#include <string>

namespace sun::protocol::raft {

struct node_id {
  uint64_t id;  // 节点id
};

struct address {
  std::string host;
  uint32_t port;
};

struct node_endpoint {
  node_id id;
  address addr;
};

struct node_replicating_state {};

struct node_member {
  node_id id;  // 节点id
  node_endpoint endpoint;
  node_replicating_state replicating_state;
};

class node_group {
 public:
  node_member self;  // 节点本身
  ::sun::protocol::array<node_member> members;

  bool exist_member() { return true; }

  bool find_member(const node_id& id, node_member* member) {
    for (uint64_t i = 0; i < _members.size(); ++i) {
      if (_members[i].id == id) {
        member = &_members[i];
        return true;
      }
    }
    return false;
  }
};

}  // namespace sun::protocol::raft

#endif  // SUN_PROTOCOL_RAFT_NODE_H_