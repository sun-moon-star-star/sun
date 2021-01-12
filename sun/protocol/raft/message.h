/**
 * @file message.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_RAFT_MESSAGE_H_
#define SUN_PROTOCOL_RAFT_MESSAGE_H_

#include <cstdint>

namespace sun::protocol::raft {

struct message {
  uint64_t id;   // ��־id
  uint64_t len;  // ��־����
  char[] data;   // ��־����
};

using message_ptr = std::shared_ptr<message>;

}  // namespace sun::protocol::raft

#endif  // SUN_PROTOCOL_RAFT_MESSAGE_H_