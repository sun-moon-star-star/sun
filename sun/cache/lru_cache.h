/**
 * @file lru_cache.h
 * @author sun-moon-star-star
 */

#ifndef SUN_CACHE_LRU_CACHE_H_
#define SUN_CACHE_LRU_CACHE_H_

namespace sun::cache {

namespace __lru_cache {

struct lru_node {
  uint64_t expire_time;
};

}  // namespace __lru_cache

template <typename T>
class lru_cache {
 private:
};  // class lru_cache

};  // namespace sun::cache

#endif  // SUN_CACHE_LRU_CACHE_H_