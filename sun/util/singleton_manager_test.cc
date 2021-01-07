/**
 * @file singleton_manager_test.cc
 * @author sun-moon-star-star
 */

#include "sun/util/singleton_manager.h"

#include <iostream>

int main(int argc, char** argv) {
  using sun::util::g_singleton_manager;
  using sun::util::singleton;

  struct A {
    A() { std::cout << "A()" << std::endl; }
    ~A() { std::cout << "~A()" << std::endl; }
  };

  auto delete_fun = [](void* ptr) {
    A* a_ptr = reinterpret_cast<A*>(ptr);
    delete a_ptr;
  };

  g_singleton_manager.register_type(singleton<A>::get_instance(), delete_fun);
  return 0;
}