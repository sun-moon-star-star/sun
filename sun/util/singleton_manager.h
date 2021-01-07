/**
 * @file singleton_manager.h
 * @author sun-moon-star-star
 */

#include "sun/data_structure/hash_table.h"
#include "sun/util/singleton.h"

#include <functional>

namespace sun::util {

struct singleton_manager final {
 public:
  typedef void (*task)(void*);

  void register_type(void* ptr, task t) { _task.set(ptr, t); }

  ~singleton_manager() {
    for (auto it = _task.get_iterator(); it.valid(); ++it) {
      void* data = it->key;
      task t = it->value;
      t(data);
    }
  }

 private:
  std::hash_table<void*, task> _task;
};  // class singleton_manager

}  // namespace sun::util