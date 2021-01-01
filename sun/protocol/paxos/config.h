/**
 * @file config.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_PAXOS_CONFIG_H_
#define SUN_PROTOCOL_PAXOS_CONFIG_H_

#include <cstdint>

namespace sun::protocol::paxos::config {

// todo(read from file)
const uint32_t accepter_num = 9;
const uint32_t commit_need_num = 5;
const uint32_t prepare_timeout_ms = 3000;  // proposer
const uint32_t commit_timeout_ms = 3000;   // proposer

}  // namespace sun::protocol::paxos::config

#endif  // SUN_PROTOCOL_PAXOS_CONFIG_H_