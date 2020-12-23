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
    b_plus_tree(const unsigned int fill_min = (m + 1) / 3):
        _fill_min(std::min(std::max(2, fill_min), (m + 1) / 2), _size(0), _depth(0), _root(nullptr) {
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
    bool equal(const KeyType& x, const KeyType& y) {
        return _key_compare_func(x, y) == _key_compare_func(y, x);
    }

    bool less(const KeyType& x, const KeyType& y) {
        return _key_compare_func(x, y);
    }

    bool le(const KeyType& x, const KeyType& y) const {
        return less(x, y) || equal(x, y);
    }

    template <typename NodeType, bool is_leaf = std::is_same<NodeType, leaf_node>::value,
        typename = std::enable_if <std::is_same<NodeType, node>::value
                                    || std::is_same<NodeType, leaf_node>::value>::type>
    void node_ch_setter(NodeType* node_item, unsigned int idx, const void* const ptr) {
        if (is_leaf) {
            node_item->ch[idx] = *reinterpret_cast<const ValueType* const>(ptr);
        } else {
            node_item->ch[idx].leaf = reinterpret_cast<const leaf_node* const>(ptr);
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
    unsigned int alloc_data_in_leaf_node(leaf_node* leaf, const KeyType& key, const void* const ptr) {
        unsigned int idx = alloc_data_in_node_common<leaf_node>(leaf, key);

        node_ch_setter<leaf_node>(leaf, idx, ptr);
        ++_size;

        return idx;
    }

    /**
     * @before: no_leaf has space(no_leaf->num < m)
     * @after: adjust head idx in no_leaf node, set ch[idx]
     * @return idx(new node index in no_leaf node)
     */
    unsigned int alloc_data_in_no_leaf_node(node* no_leaf, const KeyType& key, const void* const ptr) {
        unsigned int idx = alloc_data_in_node_common<node>(no_leaf, key);

        node_ch_setter<node>(no_leaf, idx, ptr);

        return idx;
    }

    template <typename NodeType, bool is_leaf = std::is_same<NodeType, leaf_node>::value>
    typename std::enable_if <std::is_same<NodeType, node>::value
                            || std::is_same<NodeType, leaf_node>::value, unsigned int>::type
    alloc_data_in_node(NodeType* node, const KeyType& key, const void* const ptr) {
        if (is_leaf) {
            return alloc_data_in_leaf_node(node, key, ptr);
        }
        return alloc_data_in_no_leaf_node(node, key, ptr);
    }

    template <typename NodeType, bool is_leaf = std::is_same<NodeType, leaf_node>::value>>
    unsigned int insert_into_node(NodeType* node_item, const KeyType& key, const void* const ptr) {
        unsigned int last = END;
        unsigned int at = node_item->head;
        for (; at != END; last = at, at = node_item->idx[at].nxt) {
            if (le(key, node_item->key[at])) {
                if (eq(node_item->key[at], key)) {
                    node_ch_setter<NodeType>(node_item, key, value);
                    return;
                }
                break;
            }
        }

        // alloc new
        unsigned int idx = alloc_data_in_node<NodeType>(node_item, key, ptr);
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

        return insert_into_node<leaf_node>(_root.leaf, key, reinterpret_cast<const void* const>(&value));
    }

    /**
     * get at m / 2 - 1 node index
     */
    template <typename NodeType>
    typename std::enable_if <std::is_same<NodeType, node>::value
                            || std::is_same<NodeType, leaf_node>::value, unsigned int>::type
    get_node_half_last(NodeType* node_item) {
        unsigned int idx = node_item->head;
        for (int i = 0; i < m / 2 - 1; ++i) {
            idx = node_item->idx[idx].nxt;
        }
        return idx;
    }

    /**
     * @brief split no leaf node and copy child
     * @before: node->num == m, fa has space and fa.ch[idx] is point to node
     */
    template <typename NodeType>
    typename std::enable_if <std::is_same<NodeType, node>::value
                            || std::is_same<NodeType, leaf_node>::value, NodeType*>::type
    split_node_unit(const KeyValue& key, NodeType* node_item, node* fa, unsigned int at_fa, unsigned int* last, unsigned int* key_new_idx) {
        NodeType* new_node = create_new_node<NodeType>();


        *last = get_node_half_last<NodeType>(node_item);
        unsigned int idx = new_node->idx[*last].nxt;

        new_node->free = idx;
        new_node->idx[*last].nxt = END;
        new_node->num = m / 2;

        new_node->head = 0;
        new_node->idx[m - m / 2 - 1].nxt = END; 
        new_node->free = m - m / 2;
        new_node->num = m - m / 2;

        unsigned int insert_idx = 0;
        for (int i = m / 2; i < m; ++i, ++insert_idx) {
            new_node->key[insert_idx] = new_node->key[idx];
            node_ch_setter<NodeType>(new_node, insert_idx, &node_item->ch[idx]);
            if (le(key, new_node->key[insert_idx])) {
                *key_new_idx = insert_idx;
            }
            idx = node_item->idx[idx].nxt;
        }

        ++fa->num;
        unsigned int new_idx_in_fa = fa->free;
        fa->free = fa->idx[new_idx_in_fa];
        fa->idx[new_idx_in_fa].nxt = fa->idx[at_fa].nxt;
        fa->idx[at_fa].nxt = new_idx_in_fa;
        fa->ch[new_idx_in_fa].no_leaf = new_node;

        KeyValue tmp = fa->key[at_fa];
        fa->key[at_fa] = fa->key[*last];
        fa->key[new_idx_in_fa] = tmp;

        return new_node;
    }

    
    template <typename NodeType>
    typename std::enable_if <std::is_same<NodeType, node>::value
                            || std::is_same<NodeType, leaf_node>::value, NodeType*>::type

    NodeType* split_root(const KeyValue& key,unsigned int* old_last_idx, unsigned int *key_new_idx) {
        ++_depth;
        node* new_root = create_new_node<node>();
        
        unsigned int new_idx = root->free;
        root->free = root->idx[new_idx].nxt;
        root->idx[new_idx].nxt = root->head;
        root->head = new_idx;
        root->key[new_idx] = _root.leaf->key[_root.leaf->head];

        NodeType* node = reinterpret_cast<NodeType*>(_root.ptr);
        _root.no_leaf = new_root; 

        return split_node_unit<NodeType>(key, node, new_root, new_idx, old_last_idx, key_new_idx);
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
        } else if(_size == m && _depth == 1) {
            for (unsigned int i = _root.leaf->head 
                    ;i != END; i = _root.leaf->idx[i].nxt) {
                if (_root->leaf->key[i]) {
                    // exist key: set and return
                    _root->leaf->ch[i] = value;
                    return;
                }
            }
            split_root<leaf_node>();
            return insert(key, value);
        }

        node* fa[_depth - 1];
        unsigned int fa_idx[_depth - 1];
        const unsigned int fa_len = _depth - 1;
        leaf_node* leaf;
        unsigned int leaf_idx;

        node* cur = _root.no_leaf;
        for (unsigned int i = 0; i < fa_len; ++i) {
            fa[i] = cur;
            unsigned int j = cur->head;
            for(; j != END; j = cur->idx[j].nxt) {
                if (le(key, cur->key[j])) {
                    fa_idx[i] = j;
                    break;
                }
            }
            cur = cur->ch[j].no_leaf;
        }

        leaf = reinterpret_cast<leaf_node*>(cur);
        if (leaf->num < m) {
            return insert_into_node<leaf_node>(leaf, key, reinterpret_cast<const void *const>(value));
        }

        if (fa[fa_len - 1]->num < m) {
            leaf_node* new_node = split_node_unit<leaf_node>(leaf, fa[fa_len - 1], fa_idx[fa_len - 1]]);
            return insert(key, value);
        }

        unsigned int last_idx = fa_len - 1;
        while (last_idx >= 0 && fa[last_idx]->num == m) {
            --last_idx;
        }

        if (last_idx == -1) {
            // root split
            node* old_root = _root.no_leaf;
            unsigned int last = END;
            unsigned int new_idx = END;
            node* new_node = split_root<node>(key, &last, &new_idx);
            if (le(key, old_root->key[last])) {
                fa[0] = old_root;
                fa_idx[0] = last;
            } else {
                fa[0] = new_node;
                fa_idx[0] = new_idx;
            }
            last_idx = 0;
        }

        for (; last_idx < fa_len; ++last_idx) {
            node* old_node = fa[fa_idx[last_idx]];
            unsigned int last = END;
            unsigned int new_idx = END;
            node* new_node =  split_node_unit<node>(key, fa[fa_idx[last_idx]], fa[last_idx], last_idx, &last, &key_new_idx);
            if (last_idx == fa_len - 1) {
                break;
            }
            if (le(key, old_node->key[last])) {
                fa[last_idx + 1] = old_node;
                fa_idx[last_idx + 1] = last;
            } else {
                fa[last_idx + 1] = new_node;
                fa_idx[last_idx + 1] = new_idx;
            }
        }
    }

    /**
     * @brief 根据key删除数据
     * @param[in] key 插入节点的key
     * @param[in] value 删除数据的value
     * @return 存在key返回true 不存在key返回false
     */
    bool erase(const KeyType& key, ValueType* value) {
        
    }

    /**
     * @brief 调整b+树 某节点失调 < fill_min
     */
    void adjust() {

    }

    /**
     * @brief 整棵树重建 填满
     */ 
    void rebuild() {

    }

private:
    const unsigned int _fill_min;
    unsigned int _size;
    unsigned int _depth; // 记录树的深度
    union {
        node* no_leaf;
        leaf_node *leaf;
        void* ptr;
    } _root; 
}; // class b_plus_tree

} // namespace data_structure

} // namespace sun

#endif // SUN_DATA_STRUCTURE_B_PLUS_TREE_H_