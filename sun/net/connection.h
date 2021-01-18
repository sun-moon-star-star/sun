/**
 * @file connection.h
 * @author sun-moon-star-star
 * @ref https://github.com/redis/redis/blob/unstable/src/connection.h
 */

#ifndef SUN_NET_CONNECTION_H_
#define SUN_NET_CONNECTION_H_

#include "sun/net/code.h"

namespace sun::net {

enum class connection_status : int {
  NONE,
  CONNECTING,
  ACCEPTING,
  CONNECTED,
  CLOSED,
  ERROR
};

typedef void (*connection_callback_func)(connection* conn);

struct connection_type;

struct connection {
  connection_type* type;
  connection_status state;
  short int flags;
  short int refs;
  int last_errno;
  void* private_data;
  connection_callback_func conn_handler;
  connection_callback_func write_handler;
  connection_callback_func read_handler;
  int fd;
};

struct connection_type {
  // void (*ae_handler)(struct aeEventLoop* el, int fd, void* clientData,
  //                    int mask);
  int (*connect)(connection* conn, const char* addr, int port,
                 const char* source_addr,
                 connection_callback_func connect_handler);
  int (*write)(connection* conn, const void* data, size_t data_len);
  int (*read)(connection* conn, void* buf, size_t buf_len);
  void (*close)(connection* conn);
  int (*accept)(connection* conn, connection_callback_func accept_handler);
  int (*set_write_handler)(connection* conn, connection_callback_func handler,
                           int barrier);
  int (*set_read_handler)(connection* conn, connection_callback_func handler);
  const char* (*get_last_error)(connection* conn);
  int (*blocking_connect)(connection* conn, const char* addr, int port,
                          long long timeout);
  ssize_t (*sync_write)(connection* conn, char* ptr, ssize_t size,
                        long long timeout);
  ssize_t (*sync_read)(connection* conn, char* ptr, ssize_t size,
                       long long timeout);
  ssize_t (*sync_readline)(connection* conn, char* ptr, ssize_t size,
                           long long timeout);
  int (*get_type)(connection* conn);
};

int conn_block(connection* conn);
int conn_non_block(connection* conn);
int conn_enable_tcp_no_delay(connection* conn);
int conn_disable_tcp_no_delay(connection* conn);
int conn_keep_alive(connection* conn, int interval);
int conn_send_timeout(connection* conn, long long ms);
int conn_recv_timeout(connection* conn, long long ms);
int conn_peer_to_string(connection* conn, char* ip, size_t ip_len, int* port);
int conn_format_fd_addr(connection* conn, char* buf, size_t buf_len,
                        int fd_to_str_type);
int conn_sock_name(connection* conn, char* ip, size_t ip_len, int* port);
const char* conn_get_info(connection* conn, char* buf, size_t buf_len);

}  // namespace sun::net

#endif  // SUN_NET_CONNECTION_H_