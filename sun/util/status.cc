/**
 * @file status.cc
 * @author sun-moon-star-star
 * @ref https://github.com/abseil/abseil-cpp/blob/master/absl/status/status.h
 */

#include "sun/util/status.h"

namespace sun {

std::string to_string(status_code code) {
  switch (code) {
    case status_code::ok:
      return "OK";
    case status_code::cancelled:
      return "CANCELLED";
    case status_code::unknown:
      return "UNKNOWN";
    case status_code::invalid_argument:
      return "INVALID_ARGUMENT";
    case status_code::deadline_exceeded:
      return "DEADLINE_EXCEEDED";
    case status_code::not_found:
      return "NOT_FOUND";
    case status_code::already_exists:
      return "ALREADY_EXISTS";
    case status_code::permission_denied:
      return "PERMISSION_DENIED";
    case status_code::unauthenticated:
      return "UNAUTHENTICATED";
    case status_code::resource_exhausted:
      return "RESOURCE_EXHAUSTED";
    case status_code::failed_precondition:
      return "FAILED_PRECONDITION";
    case status_code::aborted:
      return "ABORTED";
    case status_code::out_of_range:
      return "OUT_OF_RANGE";
    case status_code::unimplemented:
      return "UNIMPLEMENTED";
    case status_code::internal:
      return "INTERNAL";
    case status_code::unavailable:
      return "UNAVAILABLE";
    case status_code::data_loss:
      return "DATA_LOSS";
    default:
      return "";
  }
}

status_code __status::map_to_local_code(int value) {
  status_code code = static_cast<status_code>(value);
  switch (code) {
    case status_code::ok:
    case status_code::cancelled:
    case status_code::unknown:
    case status_code::invalid_argument:
    case status_code::deadline_exceeded:
    case status_code::not_found:
    case status_code::already_exists:
    case status_code::permission_denied:
    case status_code::resource_exhausted:
    case status_code::failed_precondition:
    case status_code::aborted:
    case status_code::out_of_range:
    case status_code::unimplemented:
    case status_code::internal:
    case status_code::unavailable:
    case status_code::data_loss:
    case status_code::unauthenticated:
      return code;
    default:
      return status_code::unknown;
  }
}

std::ostream& operator<<(std::ostream& os, status_code code) {
  return os << to_string(code);
}

status::status(status_code code, std::string msg)
    : _rep(code_to_inlined_rep(code)) {
  if (code != status_code::ok && !msg.empty()) {
    _rep = new_rep(code, msg);
  }
}

void status::unref_non_inlined(uintptr_t rep) {
  __status::status_rep* r = rep_to_pointer(rep);
  // Fast path: if ref==1, there is no need for a RefCountDec (since
  // this is the only reference and therefore no other thread is
  // allowed to be mucking with r).
  if (r->ref.load(std::memory_order_acquire) == 1 ||
      r->ref.fetch_sub(1, std::memory_order_acq_rel) - 1 == 0) {
    delete r;
  }
}

int status::raw_code() const {
  if (is_inlined(_rep)) {
    return static_cast<int>(inlined_rep_to_code(_rep));
  }
  __status::status_rep* rep = rep_to_pointer(_rep);
  return static_cast<int>(rep->code);
}

status_code status::code() const {
  return __status::map_to_local_code(raw_code());
}

uintptr_t status::new_rep(status_code code, std::string_view msg) {
  __status::status_rep* rep = new __status::status_rep;
  rep->ref.store(1, std::memory_order_relaxed);
  rep->code = code;
  rep->message.assign(msg.data(), msg.size());
  return pointer_to_rep(rep);
}

status unknown_error(std::string_view message) {
  return status(status_code::unknown);
}

status invalid_argument_error(std::string_view message) {
  return status(status_code::invalid_argument);
}

status deadline_exceeded_error(std::string_view message) {
  return status(status_code::deadline_exceeded);
}

status not_found_error(std::string_view message) {
  return status(status_code::not_found);
}

status already_exists_error(std::string_view message) {
  return status(status_code::already_exists);
}

status permission_denied_error(std::string_view message) {
  return status(status_code::permission_denied);
}

status resource_exhausted_error(std::string_view message) {
  return status(status_code::resource_exhausted);
}

status failed_precondition_error(std::string_view message) {
  return status(status_code::failed_precondition);
}

status aborted_error(std::string_view message) {
  return status(status_code::aborted);
}

status out_of_range_error(std::string_view message) {
  return status(status_code::out_of_range);
}

status unimplemented_error(std::string_view message) {
  return status(status_code::unimplemented);
}

status internal_error(std::string_view message) {
  return status(status_code::internal);
}

status unavailable_error(std::string_view message) {
  return status(status_code::unavailable);
}

status data_loss_error(std::string_view message) {
  return status(status_code::data_loss);
}

status unauthenticated_error(std::string_view message) {
  return status(status_code::unauthenticated);
}

}  // namespace sun