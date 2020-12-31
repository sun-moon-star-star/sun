/**
 * @file code.h
 * @author sun-moon-star-star
 */

#ifndef SUN_PROTOCOL_PAXOS_CODE_H_
#define SUN_PROTOCOL_PAXOS_CODE_H_

#include <cstdint>

namespace sun::protocol::paxos {

using proposer_code = int64_t;
using accepter_code = int64_t;
using learner_code = int64_t;

namespace code {

namespace proposer {

// 本提议是最新的或者提出的是最大的值
const proposer_code ok = 0;

// 本提议提出的值不是最大的 accepter返回的最大value的数据
const proposer_code use = 1;

}  // namespace proposer

namespace accepter {

// prepare阶段，accepter向proposer返回成功
const accepter_code promise = 1;

// prepare阶段，accepter向proposer返回失败
const accepter_code obsolete = 2;

// accept阶段, accepter向proposer返回成功
const accepter_code accept = -1;

// accept阶段, accepter向proposer返回失败
const accepter_code update = -2;

}  // namespace accepter

namespace learner {

// 本提议是最新的或者提出的是最大的值
const learner_code exist = 0;

// 本提议提出的值不是最大的 accepter返回的最大value的数据
const learner_code no_exist = 1;

}  // namespace learner

}  // namespace code

}  // namespace sun::protocol::paxos

#endif  // SUN_PROTOCOL_PAXOS_CODE_H_