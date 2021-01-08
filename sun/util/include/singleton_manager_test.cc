/**
 * @file singleton_manager_test.cc
 * @author sun-moon-star-star
 */

#define USE_SINGLETON_DELETE

#include "sun/util/singleton.h"

#include <iostream>

int main(int argc, char** argv) {
  using sun::util::singleton;

  struct A {
    A() { std::cout << "A()" << std::endl; }
    ~A() { std::cout << "~A()" << std::endl; }
  };

  singleton<A>::get_instance();
  return 0;
}