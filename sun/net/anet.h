/**
 * @file anet.h
 * @author sun-moon-star-star
 * @ref https://github.com/redis/redis/blob/unstable/src/anet.h
 */

#ifndef SUN_NET_ANET_H_
#define SUN_NET_ANET_H_

namespace sun::net {

#define ANET_OK 0
#define ANET_ERR -1

int anet_tcp_connect(char* err, const char* addr, int port);
int anet_tcp_non_block_connect(char* err, const char* addr, int port);
int anet_tcp_non_block_bind_connect(char* err, const char* addr, int port,
                                    const char* source_addr);
int anet_tcp_non_block_best_effort_bind_connect(char* err, const char* addr,
                                                int port,
                                                const char* source_addr);
int anet_unix_connect(char* err, const char* path);
int anet_unix_non_block_connect(char* err, const char* path);
int anet_read(int fd, char* buf, int count);
int anet_resolve(char* err, char* host, char* ipbuf, size_t ipbuf_len);
int anet_resolve_ip(char* err, char* host, char* ipbuf, size_t ipbuf_len);
int anet_tcp_server(char* err, int port, char* bindaddr, int backlog);
int anet_tcp6_server(char* err, int port, char* bindaddr, int backlog);
int anet_unix_server(char* err, char* path, mode_t perm, int backlog);
int anet_tcp_accept(char* err, int serversock, char* ip, size_t ip_len,
                    int* port);
int anet_unix_accept(char* err, int serversock);
int anet_write(int fd, char* buf, int count);
int anet_non_block(char* err, int fd);
int anet_block(char* err, int fd);
int anet_enable_tcp_no_delay(char* err, int fd);
int anet_disable_tcp_no_delay(char* err, int fd);
int anet_tcp_keep_alive(char* err, int fd);
int anet_send_timeout(char* err, int fd, long long ms);
int anet_recv_timeout(char* err, int fd, long long ms);
int anet_fd_to_string(int fd, char* ip, size_t ip_len, int* port,
                      int fd_to_str_type);
int anet_keep_alive(char* err, int fd, int interval);
int anet_format_addr(char* fmt, size_t fmt_len, char* ip, int port);
int anet_format_fd_addr(int fd, char* buf, size_t buf_len, int fd_to_str_type);

}  // namespace sun::net

#endif  // SUN_NET_ANET_H_