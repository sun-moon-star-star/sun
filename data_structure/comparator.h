/**
 * @file comparator.h
 * @author sun-moon-star-star
 */

#ifndef SUN_DATA_STRUCTURE_COMPARATOR_H_
#define SUN_DATA_STRUCTURE_COMPARATOR_H_

namespace sun {

namespace data_structure {

/**
 * 默认比较小于函数
 */
template <typename T>
struct less {
    bool operator()(const T& x, const T& y) const {
        return x < y;
    }
};

/**
 * 默认比较大于函数
 */
template <typename T>
struct greater {
    bool operator()(const T& x, const T& y) const {
        return !(x < y);
    }
};

} // namespace data_structure

} // namespace sun

#endif // SUN_DATA_STRUCTURE_COMPARATOR_H_