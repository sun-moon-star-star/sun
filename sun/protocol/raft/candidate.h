/**
 * @file candidate.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_RAFT_CANDIDATE_H_
#define SUN_PROTOCOL_RAFT_CANDIDATE_H_

#include "sun/protocol/raft/server.h"

#include <cstdint>

namespace sun::protocol::raft {

class candidate {
 public:
  node_id id;

 private:
  uint64_t _election_timeout;
  uint64_t _votes_count;
};  // class candidate

}  // namespace sun::protocol::raft

#endif  // SUN_PROTOCOL_RAFT_CANDIDATE_H_