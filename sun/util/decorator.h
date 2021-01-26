/**
 * @file decorator.h
 * @author sun-moon-star-star
 */

#ifndef SUN_UTIL_DECORATOR_H_
#define SUN_UTIL_DECORATOR_H_

#include "sun/data_structure/array.h"
#include "sun/util/common.h"

#include <functional>

namespace sun::util {

struct decorator {
  virtual void execute() {}
  virtual void get_result(void*) {}
  virtual ~decorator() {}
};

struct decorator_link {
  decorator_link() {}

  explicit decorator_link(std::initializer_list<decorator> executors) {
    _executors.alloc_n(executors.size());
    for (auto& it : executors) {
      _executors.push_back(it);
    }
  }

  decorator_link& with(const decorator& executor) {
    _executors.push_back(executor);
    return *this;
  }

  virtual void execute() {
    for (int i = 0; i < _executors.size(); ++i) {
      _executors[i].execute();
    }
  }

  decorator& operator[](uint64_t idx) { return _executors[idx]; }

 private:
  ::sun::data_structure::array<decorator> _executors;
};

}  // namespace sun::util

#endif  // SUN_UTIL_DECORATOR_H_