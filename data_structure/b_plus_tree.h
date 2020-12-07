/**
 * @file b_plus_tree.h
 * @author sun-moon-star-star
 */

#ifndef SUN_DATA_STRUCTURE_B_PLUS_TREE_H_
#define SUN_DATA_STRUCTURE_B_PLUS_TREE_H_

#include <memory>

#include "comparator.h" // sun::data_structure::less

namespace sun {

namespace data_structure {

struct b_plus_tree_node_index {
    unsigned int pre : 16;
    unsigned int nxt : 16;
};

template <typename KeyType, typename ValueType, unsigned m>
struct b_plus_tree_leaf_node {
    unsigned int num : 16;
    unsigned int head : 16;
    unsigned int free : 16;
    b_plus_tree_node_index idx[m];
    KeyType key[m];
    ValueType ch[m];
    b_plus_tree_leaf_node* pre, nxt;
}; // struct b_plus_tree_leaf_node

template <typename KeyType, typename ValueType, unsigned m>
struct b_plus_tree_node {
    unsigned int num : 16;
    unsigned int head : 16;
    unsigned int free : 16;
    b_plus_tree_node_index idx[m];
    KeyType key[m]; // parent store max_key
    union {
        b_plus_tree_node *no_leaf;
        b_plus_tree_leaf_node<KeyType,ValueType,m> *leaf;
    } ch[m];
}; // struct b_plus_tree_node

// 2 < m < 65535
template <typename KeyType, typename ValueType, unsigned m,
    typename KeyCompareFunc = less<KeyType>,
    typename = typename std::enable_if<(m > 2) && (m < 65535)>::type >
class b_plus_tree {
private:
    using node = b_plus_tree_node<KeyType, ValueType, m>;
    using leaf_node = b_plus_tree_leaf_node<KeyType, ValueType, m>;

public:
    b_plus_tree(const unsigned int fill_min = (m + 1) / 2):
        _fill_min(fill_min), _size(0), _depth(0), _root(nullptr) {
        assert(fill_min <= m);
    }

private:
    bool compare_node(const KeyType& x, const KeyType& y) const {
        return _key_compare_func(x, y) || _key_compare_func(x, y) == _key_compare_func(y, x);
    }

    node* create_new_no_leaf_node() {
        node* new_node = new node {
            .num = 0,
            .head = 65535,
            .free = 0
        };

        // 2 < m < 65535 
        new_node->idx[0].pre = new_node->idx[m - 1].nxt = 65535;
        new_node->idx[1].pre = 0;
        new_node->idx[m - 2].nxt = m - 1;
        for (unsigned i = 1; i < m - 1; ++i) {
            new_node->idx[i - 1].nxt = i;
            new_node->idx[i + 1].pre = i;
        }
    }

    void insert_first_node(const KeyType& key, const ValueType& value) {
        _size = 1;
        _depth = 1;
        _root.leaf = _root.leaf || new leaf_node;

        _root.leaf.num = 1;
        _root.leaf.head = 0;
        _root.leaf.pre = nullptr;
        _root.leaf.nxt = nullptr;
    
        _root.leaf->key[0] = key;
        _root.leaf->value[0] = value;
    }

    /**
     * @brief only call when sum of data nodes < _fill_min && > 0, 
     * new data will insert into first leaf node
     */
    void insert_first_leaf(const KeyType& key, const ValueType& value) {
        ++_size;
        ++_root.leaf.num;      

    }

    /**
     * @brief 插入新的数据: 若key存在则覆盖, key不存在则新增
     * @param[in] key 插入节点的key
     * @param[in] value 插入节点的value
     */
    void insert(const KeyType& key, const ValueType& value) {
        // 总节点为0
        if (_size == 0) {
            return insert_first_node(key, value);
        }

        // 总结点不足m/2
        if (_size < m) {
            return insert_first_leaf(key, value);
        }

        node* fa[_depth - 1];
        unsigned int fa_idx[_depth - 1];
        leaf_node* leaf;
        unsigned int leaf_idx;

        node* cur = _root;
        for (unsigned i = 0; i < _depth - 1; ++i) {
            fa[i] = cur;
            unsigned int idx = cur->head;
            for (unsigned j = 0; j < cur->num; ++j) {
                if (compare_node(key, cur->key[idx])) {
                    fa_idx[i] = idx;
                    cur = cur->ch[idx].no_leaf;
                    break;
                }
                idx = cur->idx[idx].nxt;
            }
        }   
    }

private:
    const unsigned int _fill_min;
    unsigned int _size;
    unsigned int _depth; // 记录树的深度
    union {
        node* no_leaf;
        leaf_node *leaf;
    } _root; 
}; // class b_plus_tree

} // namespace data_structure

} // namespace sun

#endif // SUN_DATA_STRUCTURE_B_PLUS_TREE_H_