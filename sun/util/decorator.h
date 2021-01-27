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

struct decorator_link : public decorator {
  decorator_link() {}

  explicit decorator_link(std::initializer_list<decorator> executors) {
    _executors.alloc_n(executors.size());
    for (auto& it : executors) {
      if (&it != this) {
        _executors.push_back(it);
      }
    }
  }

  decorator& with(const decorator& executor) {
    if (&executor != this) {
      _executors.push_back(executor);
    }
    return *this;
  }

  void execute() override {
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