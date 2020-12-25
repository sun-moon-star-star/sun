/**
 * @file heap.h
 * @author sun-moon-star-star
 */

#ifndef SUN_DATA_STRUCTURE_HEAP_H_
#define SUN_DATA_STRUCTURE_HEAP_H_

#include "sun/data_structure/comparator.h"  // sun::data_structure::less

#include <cassert>  // assert
#include <cstdint>  // uint32_t
#include <vector>   // std::vector

namespace sun {

namespace data_structure {

template <typename KeyType, typename KeyCompareFunc = greater<KeyType> >
class heap {
 private:
  bool compare_node(const KeyType& x, const KeyType& y) const {
    return _key_compare_func(x, y);
  }

 public:
  heap() {}

 private:
  void up(uint32_t idx) {
    while (idx) {
      uint32_t fa = (idx + 1) / 2 - 1;

      if (compare_node(_elements[fa], _elements[idx])) {
        break;
      }

      KeyType tmp = _elements[idx];
      _elements[idx] = _elements[fa];
      _elements[fa] = tmp;
      idx = fa;
    }
  }

  void down(uint32_t idx) {
    int32_t ch1;
    while (ch1 = (idx + 1) * 2 - 1, ch1 < _elements.size()) {
      // select max from child
      uint32_t max_idx = ch1;
      int32_t ch2 = ch1 + 1;

      if (ch2 < _elements.size() &&
          compare_node(_elements[ch2], _elements[max_idx])) {
        max_idx = ch2;
      }

      if (compare_node(_elements[idx], _elements[max_idx])) {
        break;
      }

      // swap
      KeyType tmp = _elements[idx];
      _elements[idx] = _elements[max_idx];
      _elements[max_idx] = tmp;
      idx = max_idx;
    }
  }

 public:
  /**
   * @brief 初始化堆(仅在堆为空的时候允许)
   * @param[in] args 插入元素的值
   */
  template <typename Iterator>
  void init(Iterator it, uint32_t size) {
    assert(_elements.size() == 0);

    _elements.reserve(size);
    for (uint32_t i = 0; i != size; ++i, ++it) {
      _elements.push_back(*it);
    }

    uint32_t idx = _elements.size() / 2 - 1;
    while (true) {
      down(idx);
      if (idx == 0) {
        break;
      }
      --idx;
    }
  }

  /**
   * @brief 插入元素
   * @param[in] args 插入元素的值
   */
  template <class... Args>
  void push(Args&&... args) {
    _elements.emplace_back(std::forward<Args>(args)...);
    up(_elements.size() - 1);
  }

  /**
   * @brief 弹出最"大"的元素
   * @param[out] out 最大元素的值
   */
  void pop(KeyType* out) {
    *out = _elements[0];

    _elements[0] = _elements.back();
    _elements.pop_back();

    down(0);
  }

  uint32_t size() const { return _elements.size(); }

 private:
  uint32_t _size;
  std::vector<KeyType> _elements;
  KeyCompareFunc _key_compare_func;
};  // class heap

}  // namespace data_structure

}  // namespace sun

#endif  // SUN_DATA_STRUCTURE_HEAP_H_