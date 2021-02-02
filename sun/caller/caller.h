/**
 * @file caller.h
 * @author sun-moon-star-star
 */

#ifndef SUN_CALLER_CALLER_H_
#define SUN_CALLER_CALLER_H_

#include <string>

namespace sun {

struct request_data {
  uint64_t len;
  uint64_t alloc;
  char* data;
};

struct response_data {
  uint64_t len;
  uint64_t alloc;
  char* data;
};

// �����ⲿ��������(�����洢)ͳһ�ӿ�
struct caller {
  virtual void operator()(const request_data& req,
                          response_data* const resp) = 0;
  virtual void operator()(request_data&& req, response_data* const resp) = 0;
};

}  // namespace sun

#endif  // SUN_CALLER_CALLER_H_