/**
 * @file protocol.h
 * @author sun-moon-star-star
 */

#ifndef SUN_RPC_PROTOCOL_H_
#define SUN_RPC_PROTOCOL_H_

#include <cstdint>
#include <memory>

namespace sun::rpc {

struct header {
  char magic[8] = {'L', 'U', 'R', 'O', 'M', 'I', 'N', 'G'};
  uint64_t version = 1;
  uint64_t header_size;  // header的大小
  char data[];
} __attribute__((__packed__));

struct meta {
  uint64_t meta_size;  // meta的大小
  char data[];
} __attribute__((__packed__));

struct body {
  uint64_t body_size;
  char data[];
} __attribute__((__packed__));

using header_ptr = std::shared_ptr<header>;
using meta_ptr = std::shared_ptr<meta>;
using body_ptr = std::shared_ptr<body>;

}  // namespace sun::rpc

#endif  //  SUN_RPC_PROTOCOL_H_