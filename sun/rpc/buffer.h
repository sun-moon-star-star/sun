/**
 * @file buffer.h
 * @author sun-moon-star-star
 */

#ifndef SUN_RPC_BUFFER_H_
#define SUN_RPC_BUFFER_H_

#include <cstdint>
#include <iostream>
#include <memory>

namespace sun::rpc {

struct buffer {
  buffer() = delete;

  uint32_t size() const { return end_idx_ - begin_idx_; }

  uint32_t move_head(uint32_t len) {
    if (begin_idx_ + len <= alloc_size) {
      begin_idx_ += len;
      return len;
    }

    uint32_t move_len = alloc_size - begin_idx_;
    begin_idx_ += alloc_size;
    return move_len;
  }

  uint32_t move_tail(uint32_t len) {
    if (end_idx_ - len >= 0) {
      end_idx_ -= len;
      return len;
    }

    uint32_t move_len = end_idx_;
    end_idx_ = 0;
    return move_len;
  }

  const char* data() const { return data_ + begin_idx_; }

  const uint32_t alloc_size;

 private:
  uint32_t begin_idx_;
  uint32_t end_idx_;
  char data_[];
};

using buffer_ptr = std::shared_ptr<buffer>;

buffer_ptr create_buffer(uint32_t size) {
  buffer_ptr buffer(
      reinterpret_cast<struct buffer*>(malloc(sizeof(buffer) + size)));
  uint32_t* u32p = const_cast<uint32_t*>(&buffer->alloc_size);
  *u32p = size;
  *(++u32p) = 0;
  *(++u32p) = size;
  return buffer;
}
}  // namespace sun::rpc

#endif  // SUN_RPC_BUFFER_H_