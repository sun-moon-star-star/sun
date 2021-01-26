/**
 * @file array.h
 * @author sun-moon-star-star
 */

#ifndef SUN_DATA_STRUCTURE_ARRAY_H_
#define SUN_DATA_STRUCTURE_ARRAY_H_

#include "sun/util/common.h"

#include <cstdint>
#include <utility>
#include <vector>

namespace sun::data_structure {

template <typename T>
struct array final {
  explicit array(const uint64_t capcity = UINT64_MAX)
      : _capcity(capcity), _data(nullptr), _alloc_size(0), _use_size(0) {}

  array(const array& other)
      : _capcity(other._capcity),
        _data(nullptr),
        _alloc_size(other._alloc_size),
        _use_size(other._use_size) {
    _data = reinterpret_cast<T*>(malloc(sizeof(T) * _alloc_size));
    if (_data == nullptr) {
      // throw expection??
    }
    for (uint64_t i = 0; i < _use_size; ++i) {
      new (_data + i) T(other._data[i]);
    }
  }

  array& operator=(const array& other) {
    if (this == &other) {
      return *this;
    }

    if (_data != nullptr) {
      free(_data);
      _data = nullptr;
    }

    _capcity = other._capcity;
    _alloc_size = other._alloc_size;
    _use_size = other._use_size;
    _data = reinterpret_cast<T*>(malloc(sizeof(T) * _alloc_size));
    if (_data == nullptr) {
      // throw expection??
    }
    for (uint64_t i = 0; i < _use_size; ++i) {
      new (_data + i) T(other._data[i]);
    }
    return *this;
  }

  ~array() {
    if (_data != nullptr) {
      while (_use_size) {
        _data[--_use_size].~T();
      }

      free(_data);
      _alloc_size = 0;
      _data = nullptr;
    }
  }

  void swap(array* other) {
    std::swap(other->_capcity, this->_capcity);
    std::swap(other->_data, this->_data);
    std::swap(other->_use_size, this->_use_size);
    std::swap(other->_alloc_size, this->_alloc_size);
  }

  uint64_t capcity() const { return _capcity; }

  uint64_t size() const { return _use_size; }

  uint64_t alloc_size() const { return _alloc_size; }

  int fill_n(uint64_t n) {
    if (unlikely(n > _capcity - _use_size)) {
      return -1;
    }

    uint64_t last_use_size = _use_size;
    uint64_t last_alloc_size = _alloc_size;

    uint64_t alloc_size =
        min(max(last_alloc_size * 3 / 2, last_use_size + n), _capcity);
    T* new_data = reinterpret_cast<T*>(
        (_data == nullptr) ? malloc(sizeof(T) * alloc_size)
                           : realloc(_data, sizeof(T) * alloc_size));
    if (new_data == nullptr && alloc_size > last_use_size + n) {
      alloc_size = last_use_size + n;
      new_data = reinterpret_cast<T*>(
          (_data == nullptr) ? malloc(sizeof(T) * alloc_size)
                             : realloc(_data, sizeof(T) * alloc_size));
    }

    if (new_data == nullptr) {
      return -1;
    }
    _alloc_size = alloc_size;
    _data = new_data;

    for (uint64_t i = 0; i < n; ++i) {
      new (_data + i + last_use_size) T();
    }

    return 0;
  }

  int alloc_n(uint64_t n) {
    if (unlikely(n > _capcity - _alloc_size)) {
      return -1;
    }

    uint64_t alloc_size = _use_size + n;
    T* new_data = reinterpret_cast<T*>(
        (_data == nullptr) ? malloc(sizeof(T) * alloc_size)
                           : realloc(_data, sizeof(T) * alloc_size));

    if (new_data == nullptr) {
      return -1;
    }

    _alloc_size = alloc_size;
    _data = new_data;

    return 0;
  }

 private:
  template <typename P>
  int push_back_common(P&& t) {
    if (_data == nullptr) {
      _alloc_size = min(8, _capcity);
      _data = reinterpret_cast<T*>(malloc(sizeof(T) * _alloc_size));
    }

    if (_use_size < _alloc_size) {
      new (_data + (_use_size++)) T(std::forward<P>(t));
      return 0;
    } else if (_alloc_size < _capcity) {  // _use_size == _alloc_size
      uint64_t alloc_size = min(_alloc_size * 3 / 2, _capcity);

      T* data = reinterpret_cast<T*>(realloc(_data, sizeof(T) * alloc_size));
      // todo: 内存realloc失败如何处理
      if (data == nullptr) {
        data =
            reinterpret_cast<T*>(realloc(_data, sizeof(T) * (_use_size + 20)));
        if (data == nullptr) {
          data =
              reinterpret_cast<T*>(realloc(_data, sizeof(T) * (_use_size + 1)));
        }
      }

      if (data == nullptr) {
        return -1;
      }

      _alloc_size = alloc_size;
      _data = data;
      new (_data + (_use_size++)) T(std::forward<P>(t));
      return 0;
    }

    return -1;
  }

 public:
  int push_back(const T& t) { return push_back_common(t); }

  int push_back(T&& t) { return push_back_common(std::move(t)); }

  int pop_back() {
    if (_use_size == 0) {
      return -1;
    }
    _data[--_use_size].~T();
    return 0;
  }

  T& operator[](uint64_t idx) { return _data[idx]; }

  void fixed() {
    uint64_t alloc_size = _use_size;
    if (alloc_size == 0) {
      _alloc_size = 0;
      free(_data);
      _data = nullptr;
    } else {
      T* data = reinterpret_cast<T*>(realloc(_data, sizeof(T) * alloc_size));
      if (data != nullptr) {
        _alloc_size = alloc_size;
        _data = data;
      }
    }
  }

 private:
  uint64_t _capcity;
  T* _data;
  uint64_t _alloc_size;
  uint64_t _use_size;
};  // namespace sun::data_structure

}  // namespace sun::data_structure

#endif  // SUN_DATA_STRUCTURE_ARRAY_H_