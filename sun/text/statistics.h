/**
 * @file statistics.h
 * @author sun-moon-star-star
 */

#ifndef SUN_TEXT_STATISTICS_H_
#define SUN_TEXT_STATISTICS_H_

#include <string>

namespace sun::text {

class filter;

using filter_ptr = std::shared_ptr<filter>;

class filter {
 public:
  static filter_ptr make_filter(const std::string& content) {
    filter_ptr obj(new filter(content));
    return obj;
  }

  static filter_ptr make_filter(std::string&& content) {
    filter_ptr obj(new filter(std::move(content)));
    return obj;
  }

  static filter_ptr make_filter(const std::string& content, filter_ptr nxt) {
    filter_ptr obj(new filter(content, nxt));
    return obj;
  }

  static filter_ptr make_filter(std::string&& content, filter_ptr nxt) {
    filter_ptr obj(new filter(std::move(content), nxt));
    return obj;
  }

 private:
  explicit filter(const std::string& content)
      : _content(content), _nxt(nullptr) {}

  explicit filter(std::string&& content)
      : _content(std::move(content)), _nxt(nullptr) {}

  filter(const std::string& content, filter_ptr nxt)
      : _content(content), _nxt(nxt) {}

  filter(std::string&& content, filter_ptr nxt)
      : _content(std::move(content)), _nxt(nxt) {}

 public:
  filter_ptr static link(const std::string& content, filter_ptr other) {
    filter_ptr obj(new filter(content, other));
    return obj;
  }

  bool match(const std::string& text) const {
    if (!text_match(text)) {
      return false;
    }

    if (_nxt == nullptr) {
      return true;
    }
    return _nxt->match(text);
  }

 private:
  inline bool text_match(const std::string& text) const {
    return text.find(_content) != std::string::npos;
  }

 public:
  const std::string _content;
  filter_ptr _nxt;
};  //

}  // namespace sun::text

#endif  //  SUN_TEXT_STATISTICS_H_