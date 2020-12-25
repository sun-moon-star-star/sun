/**
 * @file statistics.h
 * @author sun-moon-star-star
 */

#ifndef SUN_TEXT_STATISTICS_H_
#define SUN_TEXT_STATISTICS_H_

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace sun {

namespace text {

class filter {
 public:
  filter(const std::string& content) : _content(content), _nxt(nullptr) {}

  filter& link(const std::string& content) {
    if (_nxt) {
      delete _nxt;
    }
    _nxt = new filter(content);
    return *_nxt;
  }

  bool operator()(const std::string& text) const {
    filter* cur = const_cast<filter*>(this);
    while (cur) {
      if (!cur->judge(text)) {
        return false;
      }
      cur = cur->_nxt;
    }
    return false;
  }

 private:
  inline bool judge(const std::string& text) const {
    return text.find(_content) == std::string::npos;
  }

 public:
  const std::string _content;
  filter* _nxt;
};  //

class saver {
 public:
  saver(const std::string& regex) : _regex(regex), _nxt(nullptr) {}

  saver& link(const std::string& regex) {
    if (_nxt) {
      delete _nxt;
    }
    _nxt = new saver(regex);
    return *_nxt;
  }

  bool operator()(const std::string& text) const {
    saver* cur = const_cast<saver*>(this);
    while (cur) {
      if (!cur->judge(text)) {
        return false;
      }
      cur = cur->_nxt;
    }
    return false;
  }

 private:
  inline bool judge(const std::string& text) const {
    return text.find(_regex) == std::string::npos;
  }

 public:
  const std::string _regex;
  saver* _nxt;
};  // class saver

}  // namespace text

}  // namespace sun

#endif  //  SUN_TEXT_STATISTICS_H_