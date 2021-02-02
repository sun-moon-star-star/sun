/**
 * @file protocol.h
 * @author sun-moon-star-star
 */

#ifndef SUN_RPC_PROTOCOL_H_
#define SUN_RPC_PROTOCOL_H_

namespace sun::rpc {

struct header {
  char magic[8] = "LUROMING";
  uint64_t version = 1.0;
  uint64_t header_size;  // header的大小
} __attribute__((__packed__));

struct meta {
  uint64_t meta_size;  // meta的大小
} __attribute__((__packed__));

struct body {
  uint64_t body_size;
  char[] data;
} __attribute__((__packed__));

}  // namespace sun::rpc

#endif  //  SUN_RPC_PROTOCOL_H_