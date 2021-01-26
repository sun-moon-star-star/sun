/**
 * @file decorator_test.cc
 * @author sun-moon-star-star
 */

#include "sun/util/decorator.h"

#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int main() {
  struct logger : public sun::util::decorator {
    logger(const std::string& str) : str(str) {}
    logger(std::string&& str) : str(std::move(str)) {}

    virtual void execute() override { std::cout << str << std::endl; }
    std::string str;
  };

  struct timer : public sun::util::decorator {
    timer() { gettimeofday(&lifetime_start, nullptr); }

    virtual void execute() override {
      sleep(1000);
      gettimeofday(&lifetime_end, nullptr);
    }

    virtual void get_result(void* data) override {
      uint64_t diff = (lifetime_end.tv_sec - lifetime_start.tv_sec) * 1000000 +
                      lifetime_end.tv_usec - lifetime_start.tv_usec;

      std::cout << lifetime_start.tv_usec << std::endl;
      std::cout << lifetime_end.tv_usec << std::endl;
      *reinterpret_cast<uint64_t*>(data) = *reinterpret_cast<uint64_t*>(&data);
    }

    struct timeval lifetime_start;  // timer的创建时间
    struct timeval lifetime_end;    // timer的创建时间
  };

  sun::util::decorator_link tasks{logger{"hello world"}, timer{}};

  tasks.execute();
  uint64_t diff;
  tasks[1].get_result(&diff);
  printf("%llu\n", diff);
  std::cout << "timer: " << diff << " us" << std::endl;
  return 0;
}