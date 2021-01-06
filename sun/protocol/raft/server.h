/**
 * @file server.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_RAFT_SERVER_H_
#define SUN_PROTOCOL_RAFT_SERVER_H_

#include "sun/protocol/raft/code.h"

#include <cstdint>

namespace sun::protocol::raft {

class server {
 public:
  const uint64_t id;

  server(const uint64_t id) : id(id) {}

  void run() {}

};  // class server

using server_ptr = std::shared_ptr<server>;

create_server() {}

}  // namespace sun::protocol::raft

#endif  // SUN_PROTOCOL_RAFT_SERVER_H_