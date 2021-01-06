/**
 * @file code.h
 * @author sun-moon-star-star
 */

#ifndef SUN_NET_CODE_H_
#define SUN_NET_CODE_H_

#include <cstdint>

namespace sun::net {

using error = uint32_t;

namespace code {

const error ok = 0;

const error fail = 1;

const error fail_getfl = 2;  // fcntl(fd, F_GETFL) == -1

const error fail_setfl = 3;  // fcntl(fd, F_SETFL, flags) == -1

const error fail_setkeepalive = 4;  // setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE,
                                    // &keepalive, sizeof(keepalive)) == - 1

const error fail_setkeepidle = 5;  // setsockopt(_fd, IPPROTO_TCP, TCP_KEEPIDLE,
                                   // &keepalive, sizeof(keepalive)) < 0

const error fail_setkeepintvl = 6;  // setsockopt(_fd, IPPROTO_TCP,
                                    // TCP_KEEPINTVL, &keepalive,
                                    // sizeof(keepalive)) < 0

const error fail_setkeepcnt = 7;  // setsockopt(_fd, IPPROTO_TCP, TCP_KEEPCNT,
                                  // &keepalive, sizeof(keepalive)) <

}  // namespace code

}  // namespace sun::net

#endif  // SUN_NET_CODE_H_