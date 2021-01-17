/**
 * @file scheduler.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_RAFT_SCHEDULER_H_
#define SUN_PROTOCOL_RAFT_SCHEDULER_H_

#include "sun/util/common.h"
#include "sun/util/random.h"

namespace sun::protocol::raft {

class election_timeout {};

class scheduler {
 public:
  virtual void stop() = 0;
  virtual void schedule_election_timeout() = 0;

};  // class scheduler

class default_scheduler : public scheduler {
 public:
  default_scheduler(uint32_t min_election_timeout,
                    uint32_t max_election_timeout,
                    uint32_t log_replication_delay,
                    uint32_t log_replication_interval)
      : _min_election_timeout(min_election_timeout),
        _max_election_timeout(max_election_timeout),
        _log_replication_delay(log_replication_delay),
        _log_replication_interval(log_replication_interval) {
    if (_min_election_timeout == 0) {
      _min_election_timeout = 1;
    }

    if (_max_election_timeout < _min_election_timeout) {
      _max_election_timeout = _min_election_timeout;
    }

    if (_log_replication_interval == 0) {
      _log_replication_interval = 1;
    }
  }

  void schedule_election_timeout() override {
    uint32_t timeout = ::sun::util::random::random<uint32_t>() %
                           (_max_election_timeout - _min_election_timeout + 1) +
                       _min_election_timeout;
  }

 private:
  uint32_t _min_election_timeout;      // 最小选举超时间隔:ms
  uint32_t _max_election_timeout;      // 最大选举超时间隔:ms
  uint32_t _log_replication_delay;     // 初次日志复制延迟时间
  uint32_t _log_replication_interval;  // 日志复制间隔:ms
};

}  // namespace sun::protocol::raft

#endif  // SUN_PROTOCOL_RAFT_SCHEDULER_H_