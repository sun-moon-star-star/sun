/**
 * @file singleton.h
 * @author sun-moon-star-star
 */

#ifndef SUN_UTIL_SINGLETON_H_
#define SUN_UTIL_SINGLETON_H_

#include <atomic>
#include <mutex>

namespace sun::util {

template <typename T>
class singleton final {
 public:
  static T* GetInstance() {
    T* tmp = _instance.load(std::memory_order::memory_order_acquire);
    if (tmp == nullptr) {
      std::lock_guard<std::mutex> lock(_mutex);
      tmp = instance_.load(std::memory_order::memory_order_relaxed);
      if (tmp == nullptr) {
        tmp = new T;
        _instance.store(tmp, std::memory_order::memory_order_release);
      }
    }
    return _instance;
  }

 protected:
  static std::mutex _mutex;
  static std::atomic<T*> _instance;
  Singleton() {}
  Singleton(const Singleton&);
  Singleton& operator=(const Singleton&);
};

template <typename T>
std::atomic<T*> Singleton<T>::_instance = nullptr;

template <typename T>
std::mutex Singleton<T>::_mutex;

}  // namespace sun::util

#endif  // SUN_UTIL_SINGLETON_H_