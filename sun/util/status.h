/**
 * @file status.h
 * @author sun-moon-star-star
 * @ref https://github.com/abseil/abseil-cpp/blob/master/std/status/status.h
 */

#ifndef SUN_UTIL_STATUS_H_
#define SUN_UTIL_STATUS_H_

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

std::string to_string(status_code code);

std::ostream& operator<<(std::ostream& os, status_code code);

struct status final {
  status();
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

  std::string to_string() const;

  friend void swap(status& a, status& b);

 private:
  uintptr_t _rep;
};  // struct status

status ok_status();

std::ostream& operator<<(std::ostream& os, const status& x);

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

}  // namespace sun

#endif  // SUN_UTIL_STATUS_H_