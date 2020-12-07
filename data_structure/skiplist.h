/**
 * @file skiplist.h
 * @author sun-moon-star-star
 */

#ifndef SUN_DATA_STRUCTURE_SKIPLIST_H_
#define SUN_DATA_STRUCTURE_SKIPLIST_H_

#include <ctime>
#include <functional>
#include <random>

#include "comparator.h" // sun::data_structure::less

namespace sun {

namespace data_structure {

namespace skiplist_space {

const unsigned MAX_LEVEL = 64;
std::default_random_engine default_random_engine;
std::uniform_int_distribution<unsigned> random_getter(0,1);

void init_random_engine_with_time() {
    default_random_engine.seed(time(NULL));
}

unsigned random_level() {
    unsigned level = 1;
        while (level < MAX_LEVEL
            && random_getter(default_random_engine) == 1) {
            ++level;
        }
    return level;
}

} // namespace skiplist

template <typename KeyType, typename ValueType>
struct skiplist_node {
    KeyType key;
    ValueType value;
    skiplist_node* pre;
    skiplist_node** forward;
};

template <typename KeyType, typename ValueType,
    typename KeyCompareFunc = less<KeyType> >
class skiplist final {
private:
    using node_type = skiplist_node<KeyType, ValueType>;

    bool compare_node(node_type* node, const KeyType& key) const {
        return node != nullptr && _key_compare_func(node->key, key);
    }

public:
    skiplist() : _size(0), _head(nullptr), _tail(nullptr) {
        _head = reinterpret_cast<node_type*>(malloc(sizeof(node_type)));
        _head->forward = new node_type*[skiplist_space::MAX_LEVEL];
        _head->pre = nullptr;
        memset(_head->forward, 0x0, sizeof(node_type*) * skiplist_space::MAX_LEVEL);
    }
    
    ~skiplist() {
        node_type* cur = _head->forward[0];
        while (_size--) {
            node_type* nxt = cur->forward[0];
            delete[] cur->forward;
            delete cur;
            cur = nxt;
        }
        delete[] _head->forward;
        free(_head);
    }

    void swap(skiplist *list) {
        std::swap(list->_size, this->_size);
        std::swap(list->_head, this->_head);
        std::swap(list->_tail, this->_tail);
    }

    unsigned size() const {
        return _size; 
    }

    /**
     * @brief 查询跳表中的元素
     * @param[in] key 被查找节点的key
     * @param[out] value 用于获取查询节点的value
     * @return bool 节点存在并获取相应的value成功返回true，否则返回false
     */
    bool search(const KeyType& key, ValueType* value) const {
        node_type* cur = _head;
        for (int i = skiplist_space::MAX_LEVEL - 1; i >= 0; --i) {
            while (compare_node(cur->forward[i], key)) {
                cur = cur->forward[i];
            }
        }
        
        cur = cur->forward[0];
        if (cur == nullptr || cur->key != key) {
            return false;
        }
        *value = cur->value;
        return true;
    }

    /**
     * @brief 跳表中插入新的节点：若key不存在则插入；若key存在则更新value
     * @param[in] key 插入节点的key
     * @param[in] value 插入节点的value
     */
    void insert(const KeyType& key, const ValueType& value) {
        node_type* update_nodes[skiplist_space::MAX_LEVEL] = {0};
        node_type* cur = _head;
        for (int i = skiplist_space::MAX_LEVEL - 1; i >= 0; --i) {
            while (compare_node(cur->forward[i], key)) {
                cur = cur->forward[i];
            }
            update_nodes[i] = cur;
        }
        if (cur->forward[0] != nullptr && cur->forward[0]->key == key) {
            cur->forward[0]->value = value;
            return;
        }
        unsigned new_node_level = skiplist_space::random_level();
        node_type* new_node = new node_type {
            .key = key,
            .value = value,
            .pre = cur,
            .forward = new node_type*[new_node_level]
        };
        if (cur->forward[0] != nullptr) {
            cur->forward[0]->pre = new_node;
        } else {
            _tail = new_node;
        }
        for (int i = new_node_level - 1; i >= 0; --i) {
            new_node->forward[i] = update_nodes[i]->forward[i];
            update_nodes[i]->forward[i] = new_node;
        }
        ++_size;
    }

    /**
     * @brief 删除跳表中的元素
     * @param[in] key 要删除节点的key
     * @param[out] value 用于获取删除节点的value
     * @return bool 节点存在并删除成功返回true，否则返回false
     */
    bool erase(const KeyType& key, ValueType *value) {
        node_type* update_nodes[skiplist_space::MAX_LEVEL] = {0};
        node_type* cur = _head;
        for (int i = skiplist_space::MAX_LEVEL - 1; i >= 0; --i) {
            while (compare_node(cur->forward[i], key)) {
                cur = cur->forward[i];
            }
            update_nodes[i] = cur;
        }
        cur = cur->forward[0];
        if (cur == nullptr || cur->key != key) {
            return false;
        }
        for (int i = 0; i < skiplist_space::MAX_LEVEL; ++i) {
            if (update_nodes[i]->forward[i] != cur) {
                break;
            }
            update_nodes[i]->forward[i] = cur->forward[i];
        }
        --_size;
        *value = cur->value;
        if (cur == _tail) {
            _tail = _size == 0 ? nullptr : cur->pre;
        }
        delete cur;
        return true;
    }

    /**
     * @brief 遍历跳表中的元素
     * @param[in] func 每个跳表节点的操作函数
     * @param[in] reverse 默认值为false，表示正向遍历，为true表示反向遍历
     */
    void for_each(std::function<void(KeyType*, ValueType*)> func, bool reverse = false) const {
        if (reverse) {
            for (node_type* it = _tail ; it != _head; it = it->pre) {
                func(&(it->key), &(it->value));
            }
        } else {
            for (node_type* it = _head->forward[0] ; it != nullptr; it = it->forward[0]) {
                func(&(it->key), &(it->value));
            }
        }   
    }
    
private:
    unsigned _size;
    node_type* _head;
    node_type* _tail;
    KeyCompareFunc _key_compare_func;
};

} // namespace data_structure

} // namespace sun

#endif // SUN_DATA_STRUCTURE_SKIPLIST_H_
