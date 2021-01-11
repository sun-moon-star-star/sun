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
        _refuse_task(false) {}

  ~task_queue() {
    std::thread t;
    while (_threads.size() != 0) {
      _threads.pop_back(&t);
      t.join();
    }
  }

 private:
  bool run_one_task() {
    bool get_task = false;

    if (_urgent_tasks.count()) {
      get_task = _urgent_tasks.try_pop(&t);
    }

    if (get_task == false && _priority_tasks.size()) {
      get_task = _priority_tasks.pop(&t);
    }

    if (get_task == false && _normal_tasks.size()) {
      get_task = _normal_tasks.try_pop(&t);
    }

    if (get_task) {
      t.func(t.data);
      return true;
    }

    return false;
  }

  // thread
  void core_run() {
    while (_is_stop.load(std::memory_order::memory_order_acquire)) {
      task t;

      {
        std::unique_lock<LockType> lock(_lock);
        _idle.wait(lock, [&] { return _task_num > 0; });
      }

      run_one_task();
    }
  }

  void extra_run() {
    while (_is_stop.load(std::memory_order::memory_order_acquire)) {
      if (_task_num < _core_task_num) {
        break;
      }

      run_one_task();
    }
  }

  bool push_common_pre() {
    if (_is_shundown.load(std::memory_order::memory_order_acquire)) {
      return false;
    }

    if (_is_stop.load(std::memory_order::memory_order_acquire)) {
      return false;
    }

    if (_task_num >= _max_task_num) {
      return false;
    }

    {
      const std::unique_lock<LockType> lock(_lock);
      if (_task_num >= _max_task_num) {
        return false;
      }
      if (++_task_num == 1) {
        lock.unlock();
        _idle.notify_one();
      }
    }

    return true;
  }

  bool push_common(const task& t, queue<task>* q) {
    if (!push_common_pre()) {
      return false;
    }
    q->push(t);
    return true;
  }

 public:
  bool push(const task& t) { return push_common(t, _normal_tasks); }

  bool push_urgent(const task& t) { return push_common(t, _urgent_tasks); }

  bool push(const uint8_t priority, const task& t) {
    if (!push_common_pre()) {
      return false;
    }
    _priority_tasks.push(priority, t);
    return true;
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
  LockType _lock;
  std::condition_variable_any _idle;
  queue<task> _urgent_tasks;
  heap<uint8_t, task> _priority_tasks;
  queue<task> _normal_tasks;
  list<std::thread> _threads;
}  // namespace sun::sync
__attribute__((__packed__));  // class task_queue

}  // namespace sun::sync

#endif  // SUN_SYNC_TASK_QUEUE_H_