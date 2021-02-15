/**
 * @file event_loop.h
 * @author sun-moon-star-star
 * @ref https://github.com/redis/redis/blob/unstable/src/ae.h
 */

#ifndef SUN_NET_EVENT_LOOP_H_
#define SUN_NET_EVENT_LOOP_H_

#include <atomic>

namespace sun::net {

struct event;

class channel;

// todo(rpc)
struct event_loop {
  std::atomic_bool stop;
  int max_size;
  void* private_data;
  event* events;
  channel active_channels;
};

}  // namespace sun::net

#endif  // SUN_NET_EVENT_LOOP_H_