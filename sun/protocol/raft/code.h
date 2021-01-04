/**
 * @file code.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_RAFT_CODE_H_
#define SUN_PROTOCOL_RAFT_CODE_H_

#include <cstdint>

namespace sun::protocol::raft {

using error = uint32_t;

namespace code {

const error ok = 0;
const error fail = 1;
const error fail_communication = 2;

}  // namespace code

}  // namespace sun::protocol::raft

#endif  // SUN_PROTOCOL_RAFT_CODE_H_