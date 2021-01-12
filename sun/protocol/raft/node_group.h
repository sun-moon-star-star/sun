/**
 * @file node_group.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_RAFT_NODE_GROUP_H_
#define SUN_PROTOCOL_RAFT_NODE_GROUP_H_

#include "sun/data_structure/hash_table.h"
#include "sun/protocol/raft/node_id.h"

namespace sun::protocol::raft {

struct node_member {
  node_id id;  // 节点id
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

#endif  // SUN_PROTOCOL_RAFT_NODE_GROUP_H_