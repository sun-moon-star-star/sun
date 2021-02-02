/**
 * @file caller.h
 * @author sun-moon-star-star
 */

#ifndef SUN_CALLER_CALLER_H_
#define SUN_CALLER_CALLER_H_

#include <string>

namespace sun {

struct request {
  uint64_t len;
  uint64_t alloc;
  char* data;
};

struct response {
  uint64_t len;
  uint64_t alloc;
  char* data;
};

// �����ⲿ��������(�����洢)ͳһ�ӿ�
struct caller {
  virtual void operator()(const request& req, response* const resp) = 0;
  virtual void operator()(request&& req, response* const resp) = 0;
};

}  // namespace sun

#endif  // SUN_CALLER_CALLER_H_