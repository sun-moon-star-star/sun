/**
 * @file status.h
 * @author sun-moon-star-star
 * @ref https://github.com/abseil/abseil-cpp/blob/master/std/status/status.h
 */

#ifndef SUN_UTIL_STATUS_H_
#define SUN_UTIL_STATUS_H_

#include <atomic>
#include <iostream>
#include <string>
#include <string_view>

namespace sun {

enum class status_code : int {
  ok = 0,
  cancelled = 1,
  unknown = 2,
  invalid_argument = 3,
  deadline_exceeded = 4,
  not_found = 5,
  already_exists = 6,
  permission_denied = 7,
  resource_exhausted = 8,
  failed_precondition = 9,
  aborted = 10,
  out_of_range = 11,
  unimplemented = 12,
  internal = 13,
  unavailable = 14,
  data_loss = 15,
  unauthenticated = 16
};

namespace __status {

struct status_rep {
  std::atomic<int32_t> ref;
  status_code code;
  std::string message;
};

status_code map_to_local_code(int value);

}  // namespace __status

std::string to_string(status_code code);

std::ostream& operator<<(std::ostream& os, status_code code);

struct status final {
  status();
  status(status_code code);
  status(status_code code, std::string msg);

  status(const status&);
  status& operator=(const status& x);

  status(status&&) noexcept;
  status& operator=(status&&);

  ~status();
  void update(const status& new_status);
  void update(status&& new_status);

  bool ok() const;
  status_code code() const;

  int raw_code() const;
  std::string message() const;

  friend bool operator==(const status&, const status&);
  friend bool operator!=(const status&, const status&);

  friend void swap(status& a, status& b);

 private:
  bool is_inlined(uintptr_t rep) const;

  void ref(uintptr_t rep);
  void unref(uintptr_t rep);
  void unref_non_inlined(uintptr_t rep);

  __status::status_rep* rep_to_pointer(uintptr_t rep) const;
  uintptr_t pointer_to_rep(__status::status_rep* rep) const;

  uintptr_t code_to_inlined_rep(status_code code) const;
  status_code inlined_rep_to_code(uintptr_t rep) const;

  bool is_moved_from(uintptr_t rep);
  uintptr_t moved_from_rep();

  uintptr_t new_rep(status_code code, std::string_view msg);

 private:
  uintptr_t _rep;
};  // struct status

status ok_status();

status cancelled_error(std::string_view message);
status unknown_error(std::string_view message);
status invalid_argument_error(std::string_view message);
status deadline_exceeded_error(std::string_view message);
status not_found_error(std::string_view message);
status already_exists_error(std::string_view message);
status permission_denied_error(std::string_view message);
status resource_exhausted_error(std::string_view message);
status failed_precondition_error(std::string_view message);
status aborted_error(std::string_view message);
status out_of_range_error(std::string_view message);
status unimplemented_error(std::string_view message);
status internal_error(std::string_view message);
status unavailable_error(std::string_view message);
status data_loss_error(std::string_view message);
status unauthenticated_error(std::string_view message);

inline bool operator==(const status& lhs, const status& rhs) {
  return lhs._rep == rhs._rep;
}

inline bool operator!=(const status& lhs, const status& rhs) {
  return !(lhs == rhs);
}

inline void swap(status& a, status& b) {
  using std::swap;
  swap(a._rep, b._rep);
}

inline bool status::is_moved_from(uintptr_t rep) {
  return is_inlined(rep) && (rep & 2) != 0;
}

inline uintptr_t status::moved_from_rep() {
  return code_to_inlined_rep(status_code::internal) | 2;  // move
}

inline bool status::is_inlined(uintptr_t rep) const { return (rep & 1) == 0; }

inline __status::status_rep* status::rep_to_pointer(uintptr_t rep) const {
  assert(!is_inlined(rep));
  return reinterpret_cast<__status::status_rep*>(rep - 1);
}

inline uintptr_t status::pointer_to_rep(__status::status_rep* rep) const {
  return reinterpret_cast<uintptr_t>(rep) + 1;
}

inline void status::ref(uintptr_t rep) {
  if (!is_inlined(rep)) {
    rep_to_pointer(rep)->ref.fetch_add(1, std::memory_order_relaxed);
  }
}

inline void status::unref(uintptr_t rep) {
  if (!is_inlined(rep)) {
    unref_non_inlined(rep);
  }
}

inline uintptr_t status::code_to_inlined_rep(status_code code) const {
  return static_cast<uintptr_t>(code) << 2;
}

inline status_code status::inlined_rep_to_code(uintptr_t rep) const {
  assert(is_inlined(rep));
  return static_cast<status_code>(rep >> 2);
}

inline status::status() : _rep(code_to_inlined_rep(status_code::ok)) {}

inline status::status(status_code code) : _rep(code_to_inlined_rep(code)) {}

inline status::status(const status& x) : _rep(x._rep) { ref(_rep); }

inline status::status(status&& x) noexcept : _rep(x._rep) {
  x._rep = moved_from_rep();
}

inline status& status::operator=(status&& x) {
  uintptr_t old_rep = _rep;
  _rep = x._rep;
  x._rep = moved_from_rep();
  unref(old_rep);
  return *this;
}

inline void status::update(const status& new_status) {
  if (ok()) {
    *this = new_status;
  }
}

inline void status::update(status&& new_status) {
  if (ok()) {
    *this = std::move(new_status);
  }
}

inline status::~status() { unref(_rep); }

inline status& status::operator=(const status& x) {
  uintptr_t old_rep = _rep;
  if (x._rep != old_rep) {
    ref(x._rep);
    _rep = x._rep;
    unref(old_rep);
  }
  return *this;
}

inline status ok_status() { return status(); }

inline status cancelled_error() { return status(status_code::cancelled); }

}  // namespace sun

#endif  // SUN_UTIL_STATUS_H_