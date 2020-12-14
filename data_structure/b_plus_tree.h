/**
 * @file b_plus_tree.h
 * @author sun-moon-star-star
 * @todo search in node is too slowly
 */

#ifndef SUN_DATA_STRUCTURE_B_PLUS_TREE_H_
#define SUN_DATA_STRUCTURE_B_PLUS_TREE_H_

#include <memory>

#include "util/common.h" // unlikely
#include "data_structure/comparator.h" // sun::data_structure::less

namespace sun {

namespace data_structure {

struct b_plus_tree_node_index {
    unsigned int nxt : 16;
};

template <typename KeyType, typename ValueType, unsigned m>
struct b_plus_tree_leaf_node {
    unsigned int num : 16;
    unsigned int head : 16;
    unsigned int free : 16;
    b_plus_tree_node_index idx[m];
    b_plus_tree_leaf_node* nxt;
    KeyType key[m];
    ValueType ch[m];
}; // struct b_plus_tree_leaf_node

template <typename KeyType, typename ValueType, unsigned m>
struct b_plus_tree_node {
    unsigned int num : 16;
    unsigned int head : 16;
    unsigned int free : 16;
    b_plus_tree_node_index idx[m];
    union {
        b_plus_tree_node *no_leaf;
        b_plus_tree_leaf_node<KeyType, ValueType, m> *leaf;
    } ch[m];
    KeyType key[m]; // parent store max_key
}; // struct b_plus_tree_node

// 2 < m < 65535
template <typename KeyType, typename ValueType, unsigned m,
    typename KeyCompareFunc = less<KeyType>,
    typename = typename std::enable_if<(m > 2) && (m < 65535)>::type>
class b_plus_tree {
private:
    using node = b_plus_tree_node<KeyType, ValueType, m>;
    using leaf_node = b_plus_tree_leaf_node<KeyType, ValueType, m>;
    const unsigned int END = 65535;

public:
    b_plus_tree(const unsigned int fill_min = (m + 1) / 2):
        _fill_min(std::min(std::max(2, fill_min), m), _size(0), _depth(0), _root(nullptr) {
    }

    // todo
    b_plus_tree(const b_plus_tree& other) {

    }

    // todo
    virtual b_plus_tree& operator=(const b_plus_tree& other) {
        return *this;
    }

    // todo
    virtual ~b_plus_tree() {

    }

private:
    bool compare_node(const KeyType& x, const KeyType& y) const {
        return _key_compare_func(x, y);
    }

    template <typename NodeType, bool is_leaf = std::is_same<NodeType, leaf_node>::value,
        typename = std::enable_if <std::is_same<NodeType, node>::value
                                    || std::is_same<NodeType, leaf_node>::value>::type>
    void node_ch_setter(NodeType* node, unsigned int idx, void* ptr) {
        if (is_leaf) {
            node->ch[idx] = *reinterpret_cast<ValueType*>(ptr);
        } else {
            node->ch[idx].leaf = reinterpret_cast<leaf_node*>(ptr);
        }
    }

    template <typename NodeType>
    typename std::enable_if <std::is_same<NodeType, node>::value
                            || std::is_same<NodeType, leaf_node>::value, NodeType*>::type
    create_new_node_common() {
        NodeType* new_node = reinterpret_cast<NodeType*>(malloc(sizeof(NodeType)));

        new_node->head = END;
        new_node->free = new_node->num = 0;

        for (int i = 0; i < m - 1; ++i) {
            new_node->idx[i].nxt = i + 1;
        }
        new_node->idx[m - 1].nxt = END;

        return new_node;
    }

    leaf_node* create_new_leaf_node() {
        leaf_node* new_node = create_new_node_common<leaf_node>();
        new_node->nxt = nullptr;
        return new_node;
    }

    node* create_new_no_leaf_node() {
        return create_new_node_common<node>();
    }

    template <typename NodeType, bool is_leaf = std::is_same<NodeType, leaf_node>::value>
    typename std::enable_if <std::is_same<NodeType, node>::value
                            || std::is_same<NodeType, leaf_node>::value, NodeType*>::type
    create_new_node() {
        if (is_leaf) {
            return reinterpret_cast<NodeType*>(create_new_leaf_node());
        }
        return reinterpret_cast<NodeType*>(create_new_no_leaf_node());
    }

    template <typename NodeType>
    std::enable_if <std::is_same<NodeType, node>::value
                    || std::is_same<NodeType, leaf_node>::value, unsigned int>::type
    alloc_data_in_node_common(NodeType* node_item, const KeyType& key) {
        ++node_item->num;

        unsigned int idx = node_item->free;
        node_item->free = node_item->idx[idx].nxt;

        node_item->key[idx] = key;
        return idx;
    }

    /**
     * @before: leaf has space(leaf->num < m)
     * @after: adjust head idx in leaf node
     */
    unsigned int alloc_data_in_leaf_node(leaf_node* leaf, const KeyType& key, void* ptr) {
        unsigned int idx = alloc_data_in_node_common<leaf_node>(leaf, key);

        leaf->ch[idx] = *reinterpret_cast<ValueType*>(ptr);
        ++_size;

        return idx;
    }

    /**
     * @before: no_leaf has space(no_leaf->num < m)
     * @after: adjust head idx in no_leaf node, set ch[idx]
     * @return idx(new node index in no_leaf node)
     */
    unsigned int alloc_data_in_no_leaf_node(node* no_leaf, const KeyType& key, void* ptr) {
        unsigned int idx = alloc_data_in_node_common<node>(no_leaf, key);

        no_leaf->ch[idx].leaf = reinterpret_cast<leaf_node*>(ptr);
        
        return idx;
    }

    template <typename NodeType, bool is_leaf = std::is_same<NodeType, leaf_node>::value>
    typename std::enable_if <std::is_same<NodeType, node>::value
                            || std::is_same<NodeType, leaf_node>::value, unsigned int>::type
    alloc_data_in_node(NodeType* node, const KeyType& key, void* ptr) {
        if (is_leaf) {
            return alloc_data_in_leaf_node(node, key, ptr);
        }
        return alloc_data_in_no_leaf_node(node, key, ptr);
    }

    template <typename NodeType>
    unsigned int insert_into_node(NodeType* node, const KeyType& key, void* ptr) {
        unsigned int last = END;
        unsigned int at = node->head;
        for (; at != END; last = at, at = node->idx[at].nxt) {
            if (compare_node(key, node->key[at])) {
                if (compare_node(node->key[at], key)) {
                    // equal, modify already exist node
                    node->key[at] = key;
                    return;
                }
                break;
            }
        }

        // alloc new
        unsigned int idx = alloc_data_in_node<NodeType>(node, key, value);
        if (unlikely(last == END)) {
            leaf->idx[idx].nxt = leaf->head;
            leaf->head = idx;
        } else {
            leaf->idx[idx].nxt = at;
            leaf->idx[last].nxt = idx;
        }

    }

    void insert_when_less_then_m(const KeyType& key, const ValueType& value) {
        if (_size == 0) {
            _depth = 1;
            _root.leaf = _root.leaf || create_new_node<leaf_node>();
        }

        return insert_into_node<leaf_node>(_root.leaf, key, value);
    }

    /**
     * get at m / 2 - 1 node index
     */
    template <typename NodeType>
    typename std::enable_if <std::is_same<NodeType, node>::value
                            || std::is_same<NodeType, leaf_node>::value, unsigned int>::type
    get_node_half_last(NodeType* item) {
        unsigned int idx = item->head;
        for (int i = 0; i < m / 2 - 1; ++i) {
            idx = item->idx[idx].nxt;
        }
        return idx;
    }

    /**
     * @brief split no leaf node and copy child
     * @before: no_leaf->num == m, fa has space and fa.ch[idx] is point to no_leaf
     */
    node* split_no_leaf_node_unit(node* no_leaf, node* fa, unsigned int idx) {
        node* new_node = create_new_node<node>();

        unsigned int last = get_node_half_last<node>(no_leaf);

        unsigned int idx = no_leaf->idx[last].nxt;
        no_leaf->free = idx;
        no_leaf->idx[last].nxt = END;
        no_leaf->num = m / 2;

        new_node->head = 0;
        new_node->idx[m - m / 2].nxt = END; 
        new_node->free = m - m / 2;
        new_node->num = m - m / 2;

        unsigned int insert_idx = 0;
        for (int i = m / 2; i < m; ++i) {
            new_node->key[insert_idx] = no_leaf->key[idx];
            new_node->ch[insert_idx].no_leaf = no_leaf->ch[idx].no_leaf;
            idx = no_leaf->idx[idx].nxt;
        }

        ++fa->num;
        unsigned int new_idx_in_fa = fa->free;
        fa->free = fa->idx[new_idx_in_fa];
        fa->idx[new_idx_in_fa].nxt = fa->idx[idx].nxt;
        fa->idx[idx].nxt = new_idx_in_fa;        

        return new_node;
    }

    /**
     * @brief split no leaf node and copy data
     * @before: leaf->num == m, fa has space(fa->num < m) and fa.ch[idx] is point to leaf
     */
    leaf_node* split_leaf_node_unit(leaf_node* leaf, node* fa, unsigned int idx) {
        leaf_node* new_node = create_new_node<leaf_node>();

        unsigned int last = get_node_half_last<leaf_node>(leaf);
        
        leaf->free = idx;
        leaf->idx[last].nxt = END;
        leaf->num = m / 2;

        leaf->head = 0;
        leaf->idx[m - m / 2].nxt = END; 
        leaf->free = m - m / 2;
        leaf->num = m - m / 2;

        unsigned int insert_idx = 0;
        for (int i = m / 2; i < m; ++i) {
            new_node->key[insert_idx] = leaf->key[idx];
            new_node->value[insert_idx] = leaf->value[idx];
            idx = leaf->idx[idx].nxt;
        }

        ++fa->num;
        unsigned int new_idx_in_fa = fa->free;
        fa->free = fa->idx[new_idx_in_fa];
        fa->idx[new_idx_in_fa].nxt = fa->idx[idx].nxt;
        fa->idx[idx].nxt = new_idx_in_fa;        

        return new_node;
    }

    leaf_node* split_when_root_is_leaf_node() {
        node* root = create_new_no_leaf_node();
        
        unsigned int new_idx = root->free;
        root->free = root->idx[new_idx].nxt;
        root->idx[new_idx].nxt = root->head;
        root->head = new_idx;
        root->key[] =

    }

public:
    /**
     * @brief 插入新的数据: 若key存在则覆盖, key不存在则新增
     * @param[in] key 插入节点的key
     * @param[in] value 插入节点的value
     */
    void insert(const KeyType& key, const ValueType& value) {
        // 总结点数小于m
        if(_size < m) {
            return insert_when_less_then_m(key, value);
        }

        node* fa[_depth - 1];
        unsigned int fa_idx[_depth - 1];
        leaf_node* leaf;
        unsigned int leaf_idx;

        node* cur = _root.no_leaf;
        for (unsigned int i = 0; i < _depth - 1; ++i) {
            fa[i] = cur;
            unsigned int j = cur->head;
            for(; j != END; j = cur->idx[j].nxt) {
                if (compare_node(key, cur->key[j])) {
                    fa_idx[i] = j;
                    break;
                }
            }
            cur = cur->ch[j].no_leaf;
        }

        leaf = reinterpret_cast<leaf_node*>(cur);
        if (leaf->num < m) {
            return insert_into_leaf_node(leaf, key, value);
        }

        if (_depth = 1) {
            // leaf is leaf

        }

    }

    /**
     * @brief 根据key删除数据
     * @param[in] key 插入节点的key
     * @param[in] value 删除数据的value
     */
    void delete(const KeyType& key, ValueType* value) {

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