/**
 * @file connection.cc
 * @author sun-moon-star-star
 */

#include "sun/net/connection.h"

#include "sun/net/code.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <netinet/tcp.h>

namespace sun::net {

error connection::set_block(int non_block) {
  int flags;

  if ((flags = fcntl(_fd, F_GETFL)) == -1) {
    return code::fail_getfl;
  }

  if (non_block) {
    flags |= O_NONBLOCK;
  } else {
    flags &= ~O_NONBLOCK;
  }

  if (fcntl(_fd, F_SETFL, flags) == -1) {
    return code::fail_setfl;
  }

  return code::ok;
}

error connection::set_keepalive(int32_t interval) {
  int keepalive = 1;

  if (setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE, &keepalive,
                 sizeof(keepalive)) == -1) {
    return code::fail_setkeepalive;
  }

#ifdef __linux__
  keepalive = interval;
  if (setsockopt(_fd, IPPROTO_TCP, TCP_KEEPIDLE, &keepalive,
                 sizeof(keepalive)) < 0) {
    return code::fail_setkeepidle;
  }

  keepalive = interval / 3;
  if (keepalive == 0) keepalive = 1;
  if (setsockopt(_fd, IPPROTO_TCP, TCP_KEEPINTVL, &keepalive,
                 sizeof(keepalive)) < 0) {
    return code::fail_setkeepintvl;
  }

  keepalive = 3;
  if (setsockopt(_fd, IPPROTO_TCP, TCP_KEEPCNT, &keepalive, sizeof(keepalive)) <
      0) {
    return code::fail_setkeepcnt;
  }

#else
  ((void)interval);
#endif

  return code::ok;
}

}  // namespace sun::net