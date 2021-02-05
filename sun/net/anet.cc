/**
 * @file anet.cc
 * @author sun-moon-star-star
 * @ref https://github.com/redis/redis/blob/unstable/src/anet.h
 */

#include "sun/net/anet.h"

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

namespace sun::net {

static void anet_set_error(char* err, const char* fmt, ...) {
  va_list ap;

  if (!err) return;
  va_start(ap, fmt);
  vsnprintf(err, ANET_ERR_LEN, fmt, ap);
  va_end(ap);
}

int anet_set_block(char* err, int fd, int non_block) {
  int flags;

  if ((flags = fcntl(fd, F_GETFL)) == -1) {
    anetSetError(err, "fcntl(F_GETFL): %s", strerror(errno));
    return ANET_ERR;
  }

  if (non_block)
    flags |= O_NONBLOCK;
  else
    flags &= ~O_NONBLOCK;

  if (fcntl(fd, F_SETFL, flags) == -1) {
    anetSetError(err, "fcntl(F_SETFL,O_NONBLOCK): %s", strerror(errno));
    return ANET_ERR;
  }
  return ANET_OK;
}

int anet_non_block(char* err, int fd) { return anet_non_block(err, fd, 1); }

int anet_block(char* err, int fd) { return anet_set_block(err, fd, 0); }

int anet_keep_alive(char* err, int fd, int interval) {
  int val = 1;

}  // namespace sun::net