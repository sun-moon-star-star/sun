/**
 * @file connection.h
 * @author sun-moon-star-star
 */

#ifndef SUN_NET_CONNECTION_H_
#define SUN_NET_CONNECTION_H_

#include "sun/net/code.h"

namespace sun::net {

class connection {
 public:
  error set_block(int32_t non_block);

  error set_keepalive(int32_t interval);

 private:
  int32_t _fd;

};  // class connection

}  // namespace sun::net

#endif  // SUN_NET_CONNECTION_H_