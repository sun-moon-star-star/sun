/**
 * @file heap.h
 * @author sun-moon-star-star
 */

#ifndef SUN_DATA_STRUCTURE_HEAP_H_
#define SUN_DATA_STRUCTURE_HEAP_H_

#include <cstdlib> // malloc
#include <cassert> // assert
#include <vector> // std::vector

#include "comparator.h" // sun::data_structure::less

namespace sun {

namespace data_structure {

template <typename KeyType,
    typename KeyCompareFunc = greater<KeyType> >
class heap {
private:
    bool compare_node(const KeyType& x, const KeyType& y) const {
        return _key_compare_func(x, y);
    }

public:
    heap() {}

private:
    void up(unsigned idx) {
        while (idx) {
            unsigned fa = (idx + 1) / 2 - 1;

            if (compare_node(_elements[fa], _elements[idx])) {
                break;
            }

            KeyType tmp = _elements[idx];
            _elements[idx] = _elements[fa];
            _elements[fa] = tmp;
            idx = fa;
        }
    }

    void down(unsigned idx) {
        int ch1;
        while (ch1 = (idx + 1) * 2 - 1, ch1 < _elements.size()) {
            // select max from child
            unsigned max_idx = ch1;
            int ch2 = ch1 + 1;
        
            if (ch2 < _elements.size() 
                && compare_node(_elements[ch2], _elements[max_idx])) {
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
    void init(Iterator it,unsigned size) {
        assert(_elements.size() == 0);

        _elements.reserve(size);
        for (unsigned i = 0; i != size; ++i, ++it) {
            _elements.push_back(*it);
        }

        unsigned idx = _elements.size() / 2 - 1;
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

    unsigned size() const {
        return _elements.size();
    }

private:
    unsigned _size;
    std::vector<KeyType> _elements;
    KeyCompareFunc _key_compare_func;
}; // class heap

} // namespace data_structure

} // namespace sun

#endif // SUN_DATA_STRUCTURE_HEAP_H_