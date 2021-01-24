/**
 * @file decorator.h
 * @author sun-moon-star-star
 */

#ifndef SUN_UTIL_DECORATOR_H_
#define SUN_UTIL_DECORATOR_H_

#include "sun/util/common.h"

#include <functional>

namespace sun::util {

struct decorator {
  virtual void execute(void*) = 0;
};

}  // namespace sun::util

#endif  // SUN_UTIL_DECORATOR_H_