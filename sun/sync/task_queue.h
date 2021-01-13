/**
 * @file task_queue.h
 * @author sun-moon-star-star
 * @note priority
 */

#ifndef SUN_SYNC_TASK_QUEUE_H_
#define SUN_SYNC_TASK_QUEUE_H_

#include "sun/sync/heap.h"
#include "sun/sync/list.h"
#include "sun/sync/queue.h"

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <thread>

namespace sun::sync {

namespace __task_queue {

template <typename T>
class queue {
  queue() : _state(0llu) {}

  bool push(uint8_t priority, const T& t) {
    priority = min(priority, 63);

    auto func = [&_state, &_size]() {
      _state.fetch_or(1llu << priority,
                      std::memory_order::memory_order_release);
      _size.fetch_add(1llu, std::memory_order::memory_order_release);
    };

    if (_tasks[priority].try_push_sync_with(t, func)) {
      return true;
    }

    return false;
  }

  bool pop(T* t) {
    uint64_t state = _state.load(std::memory_order::memory_order_acquire);
    if (state == 0llu) {
      return false;
    }

    uint8_t priority =
        (state & 4294967295llu)
            ? __builtin_ctz(state & 4294967295llu)
            : __builtin_ctz(((state >> 32) & 4294967295llu) + 32);

    auto func = [&_state, &_size, &_tasks, &priority]() {
      _size.fetch_sub(1llu, std::memory_order::memory_order_release);
      if (_tasks[priority].size_with_no_lock() == 0) {
        _state.fetch_xor(1llu << priority,
                         std::memory_order::memory_order_release);
      }
    };

    if (_tasks[priority].try_pop_sync_with(t, func)) {
      return true;
    }

    return false;
  }

  uint64_t size() const {
    return _size.load(std::memory_order::memory_order_acquire);
  }

 private:
  atomic_uint64_t _size;
  atomic_uint64_t _state;
  queue<T> _tasks[64];
};

}  // namespace __task_queue

template <typename LockType = spinlock>
class task_queue {
 public:
  using call = std::function<void(void*)>;

  using task = struct {
    void* data;
    call func;
  };

  task_queue(const uint16_t core_thread_num, const uint16_t max_thread_num,
             const uint32_t core_task_num, const uint32_t max_task_num)
      : _core_thread_num(core_thread_num),
        _max_thread_num(max_thread_num),
        _core_task_num(core_task_num),
        _max_task_num(max_task_num),
        _task_num(0u),
        _is_stop(false),
        _refuse_task(false),
        _thread_num(0) {}

  ~task_queue() {
    std::thread t;
    while (_threads.size() != 0) {
      _threads.pop_back(&t);
      if (t.joinable()) {
        t.join();
      }
    }
  }

 private:
  bool run_one_task() {
    bool get_task = false;

    if (get_task) {
      t.func(t.data);
      return true;
    }

    return false;
  }

  // core thread
  void core_run() {
    while (_is_stop.load(std::memory_order::memory_order_acquire)) {
      task t;

      {
        std::unique_lock<LockType> lock(_lock);
        _idle.wait(lock, [&] { return _task_num > 0; });
      }

      run_one_task();
    }

    thread_num.fetch_sub(1, std::memory_order::memory_order_release);
  }

  // max thread
  void extra_run() {
    while (_is_stop.load(std::memory_order::memory_order_acquire)) {
      if (_task_num < _core_task_num) {
        break;
      }

      run_one_task();
    }

    if (_task_num > 0) {
      run_one_task();
    }

    thread_num.fetch_sub(1, std::memory_order::memory_order_release);
  }

  bool push_common_before() {
    if (_is_shundown.load(std::memory_order::memory_order_acquire)) {
      return false;
    }

    if (_is_stop.load(std::memory_order::memory_order_acquire)) {
      return false;
    }

    if (_task_num >= _max_task_num) {
      return false;
    }
    return true;
  }

  void push_common_after() {
    _idle.notify_one();

    uint16_t thread_num =
        _thread_num.load(std::memory_order::memory_order_acquire);

    if (thread_num < _core_thread_num) {
      const std::unique_lock<LockType> lock(_lock);
      thread_num = _thread_num.load(std::memory_order::memory_order_relaxed);

      if (thread_num < _core_thread_num && thread_num < _task_num) {
        std::thread t(&task_queue::core_run, this);
        _threads.push(t);
        thread_num.fetch_add(1, std::memory_order::memory_order_relaxed);
      }
    }

    thread_num = _thread_num.load(std::memory_order::memory_order_acquire);
    if (thread_num < _max_thread_num) {
      const std::unique_lock<LockType> lock(_lock);

      thread_num = _thread_num.load(std::memory_order::memory_order_relaxed);
      if (thread_num < _max_thread_num && thread_num < _task_num) {
        std::thread t(&task_queue::extra_run, this);
        t.detach();
        thread_num.fetch_add(1, std::memory_order::memory_order_relaxed);
      }
    }
  }

 public:
  bool push(uint8_t priority, const task& t) {
    if (!push_common_before()) {
      return false;
    }

    {
      const std::unique_lock<LockType> lock(_lock);
      if (_task_num >= _max_task_num) {
        return false;
      }
      ++_task_num;
      _tasks->push(priority, t);
    }

    push_common_after();
    retrun true;
  }

  void shundown() {
    _is_shundown.store(true, std::memory_order::memory_order_release);
  }

  void stop() { _is_stop.store(true, std::memory_order::memory_order_release); }

 private:
  const uint16_t _core_thread_num;
  const uint16_t _max_thread_num;
  const uint32_t _core_task_num;
  const uint32_t _max_task_num;
  uint32_t _task_num;
  std::atomic_bool _is_stop;
  std::atomic_bool _is_shundown;
  std::atomic_uint16_t _thread_num;
  LockType _lock;
  std::condition_variable_any _idle;
  __task_queue::queue<task> _tasks;
  list<std::thread> _threads;
} __attribute__((__packed__));  // class task_queue

}  // namespace sun::sync

#endif  // SUN_SYNC_TASK_QUEUE_H_