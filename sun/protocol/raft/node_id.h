/**
 * @file node_id.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_RAFT_NODE_ID_H_
#define SUN_PROTOCOL_RAFT_NODE_ID_H_

#include <cstdint>

namespace sun::protocol::raft {

struct node_id {
  uint64_t id;  // ½Úµãid
};

}  // namespace sun::protocol::raft

#endif  // SUN_PROTOCOL_RAFT_NODE_ID_H_