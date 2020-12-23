/**
 * @file statistics.h
 * @author sun-moon-star-star
 */

#ifndef SUN_TEXT_STATISTICS_H_
#define SUN_TEXT_STATISTICS_H_

#include <string>
#include <unordered_set>
#include <unordered_map>

namespace sun {

namespace text {



class filter {
public:
    finder(const std::string& content):
        _content(content), _nxt(nullptr) {}

    finder& link(const std::string& content) {
        if (_nxt) {
            delete _nxt;
        }
        _nxt = new finder(content);
        return *_nxt;
    }

    bool operator()(const std::string& text) const {
        finder* cur = const_cast<finder*>(this);
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
    finder* _nxt;
}; // 


class saver {
public:
    finder(const std::string& regex):
        _regex(regex), _nxt(nullptr) {}

    finder& link(const std::string& regex) {
        if (_nxt) {
            delete _nxt;
        }
        _nxt = new finder(regex);
        return *_nxt;
    }

    bool operator()(const std::string& text) const {
        finder* cur = const_cast<finder*>(this);
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
}; // class saver


class statistics {
public:

};

} // namespace text

} // namespace sun
