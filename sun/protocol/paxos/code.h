/**
 * @file code.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_PAXOS_CODE_H_
#define SUN_PROTOCOL_PAXOS_CODE_H_

#include <cstdint>

namespace sun::protocol::paxos {

using error = uint64_t;

namespace code {
const error success = 0;
const error failure = 1;
const error fail_communication = 2;

}  // namespace code

}  // namespace sun::protocol::paxos

#endif  // SUN_PROTOCOL_PAXOS_CODE_H_