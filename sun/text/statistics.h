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

#define FILTER_NONE 0
#define FILTER_AND 1
#define FILTER_OR 2

class filter {
 public:
  static filter_ptr make_filter(const std::string& content) {
    return filter_ptr(new filter(content));
  }

  static filter_ptr make_filter(std::string&& content) {
    return filter_ptr(new filter(std::move(content)));
  }

 private:
  explicit filter(const std::string& content)
      : _type(FILTER_NONE), _content(content), _nxt(nullptr) {}

  explicit filter(std::string&& content)
      : _type(FILTER_NONE), _content(std::move(content)), _nxt(nullptr) {}

 public:
  static filter_ptr and_filter(const std::string& content, filter_ptr other) {
    filter_ptr obj(new filter(content));
    obj->_type = FILTER_AND;
    obj->_nxt = other;
    return obj;
  }

  static filter_ptr or_filter(const std::string& content, filter_ptr other) {
    filter_ptr obj(new filter(content));
    obj->_type = FILTER_OR;
    obj->_nxt = other;
    return obj;
  }

 private:
  bool and_match(const std::string& text) const {
    if (!text_match(text)) {
      return false;
    }

    if (_nxt == nullptr) {
      return true;
    }

    return _nxt->match(text);
  }

  bool or_match(const std::string& text) const {
    if (text_match(text)) {
      return true;
    }

    if (_nxt == nullptr) {
      return false;
    }

    return _nxt->match(text);
  }

 public:
  bool match(const std::string& text) const {
    if (_type == FILTER_AND) {
      return and_match(text);
    } else if (_type == FILTER_OR) {
      return or_match(text);
    }

    return text_match(text);
  }

 private:
  inline bool text_match(const std::string& text) const {
    return text.find(_content) != std::string::npos;
  }

 public:
  uint64_t _type;
  const std::string _content;
  filter_ptr _nxt;
};  //

}  // namespace sun::text

#endif  //  SUN_TEXT_STATISTICS_H_