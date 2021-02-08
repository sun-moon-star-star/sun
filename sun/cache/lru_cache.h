/**
 * @file lru_cache.h
 * @author sun-moon-star-star
 */

#ifndef SUN_CACHE_LRU_CACHE_H_
#define SUN_CACHE_LRU_CACHE_H_

#include <functional>

namespace sun::cache {

template <typename T>
class lru_cache {
 private:
  using lru_node = struct {
    uint64_t expire_time;     // 可以是毫秒
    std::shared_ptr<T> data;  // 零拷贝
  };

  using expire_func = std::function<void(T*)>;
};  // class lru_cache

}  // namespace sun::cache

#endif  // SUN_CACHE_LRU_CACHE_H_