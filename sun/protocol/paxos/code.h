/**
 * @file code.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_PAXOS_CODE_H_
#define SUN_PROTOCOL_PAXOS_CODE_H_

#include <cstdint>

namespace sun::protocol::paxos {

using error = uint32_t;

namespace code {

const error ok = 0;
const error fail = 1;
const error fail_communication = 2;

}  // namespace code

}  // namespace sun::protocol::paxos

#endif  // SUN_PROTOCOL_PAXOS_CODE_H_