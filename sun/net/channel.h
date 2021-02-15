/**
 * @file channel.h
 * @author sun-moon-star-star
 */

#ifndef SUN_NET_CHANNEL_H_
#define SUN_NET_CHANNEL_H_

#include <cstdint>

namespace sun::net {

class event_loop;

struct channel {
 public:
  typedef std::function<void()> event_callback;
  typedef std::function<void(int64_t)> read_event_callback;

 private:
  event_loop* _loop;
  const int _fd;
  int _events;

  read_event_callback read_callback;
  event_callback _writeCallback;
  event_callback _closeCallback;
  event_callback _errorCallback;
};

}  // namespace sun::net

#endif  // SUN_NET_CHANNEL_H_