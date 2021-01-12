/**
 * @file config.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_RAFT_CONFIG_H_
#define SUN_PROTOCOL_RAFT_CONFIG_H_

#include <cstdint>

namespace sun::protocol::raft {

struct config {
  uint32_t min_election_timeout = 3000;      // ��Сѡ�ٳ�ʱ���:ms
  uint32_t max_election_timeout = 4000;      // ���ѡ�ٳ�ʱ���:ms
  uint32_t log_replication_interval = 1000;  // ��־���Ƽ��:ms
};

config g_config;

void load_from_file() {}

}  // namespace sun::protocol::raft

#endif  // SUN_PROTOCOL_RAFT_CONFIG_H_