/**
 * @file message.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_RAFT_MESSAGE_H_
#define SUN_PROTOCOL_RAFT_MESSAGE_H_

#include <cstdint>

namespace sun::protocol::raft {

struct message {
  uint64_t id;   // 日志id
  uint64_t len;  // 日志长度
  char[] data;   // 日志数据
};

using message_ptr = std::shared_ptr<message>;

}  // namespace sun::protocol::raft

#endif  // SUN_PROTOCOL_RAFT_MESSAGE_H_