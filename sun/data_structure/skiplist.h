/**
 * @file skiplist.h
 * @author sun-moon-star-star
 */

#ifndef SUN_DATA_STRUCTURE_SKIPLIST_H_
#define SUN_DATA_STRUCTURE_SKIPLIST_H_

#include "sun/data_structure/comparator.h"  // sun::data_structure::less
#include "sun/util/common.h"

#include <cstdint>
#include <functional>

namespace sun {

namespace data_structure {

namespace skiplist_space {

const uint32_t MAX_LEVEL = 64;

void init_random_engine_with_time() {
  ::sun::util::common::random::init_random_engine_with_time();
}

uint32_t random_level() {
  uint32_t level = 1;
  while (level < MAX_LEVEL &&
         (::sun::util::common::random::random<>() & 1) == 1) {
    ++level;
  }
  return level;
}

}  // namespace skiplist_space

template <typename KeyType, typename ValueType>
struct skiplist_node {
  KeyType key;
  ValueType value;
  skiplist_node* pre;
  skiplist_node** forward;

  skiplist_node(const skiplist_node&) = delete;
  skiplist_node& operator=(const skiplist_node&) = delete;
};

template <typename KeyType, typename ValueType,
          typename KeyCompareFunc = less<KeyType> >
class skiplist final {
 private:
  using node_type = skiplist_node<KeyType, ValueType>;

 public:
  skiplist();

  skiplist(const skiplist& other);

  skiplist& operator=(const skiplist& other);

  ~skiplist();

 private:
  /**
   * @brief 比较函数:
   * node不为空，且调用函数的返回值与key的比较不为false
   * @param[in] node 比较的节点，霢�要判断节点是否为穄1�7
   * @param[in] key 被比较的key
   */
  bool compare_node(node_type* node, const KeyType& key) const;

 public:
  /**
   * @brief 清空跳表中的元素
   */
  void clear();

  /**
   * @brief 交换两个跳表
   * @param[in] list 被交换的跳表
   */
  void swap(skiplist* list);

  /**
   * @brief 获取跳表的元素个敄1�7
   * @param[out] 跳表的元素个敄1�7
   */
  uint32_t size() const;

  /**
   * @brief 查询跳表中的元素
   * @param[in] key 被查找节点的key
   * @param[out] value 用于获取查询节点的value
   * @return bool
   * 节点存在并获取相应的value成功返回true，否则返回false
   */
  bool search(const KeyType& key, ValueType* value) const;

  /**
   * @brief
   * 跳表中插入新的节点：若key不存在则插入；若key存在则更新value
   * @param[in] key 插入节点的key
   * @param[in] value 插入节点的value
   */
  void insert(const KeyType& key, const ValueType& value);

  /**
   * @brief 删除跳表中的元素
   * @param[in] key 要删除节点的key
   * @param[out] value 用于获取删除节点的value
   * @return bool 节点存在并删除成功返回true，否则返回false
   */
  bool erase(const KeyType& key, ValueType* value);

  /**
   * @brief 遍历跳表中的元素
   * @param[in] func 每个跳表节点的操作函敄1�7
   * @param[in] reverse
   * 默认值为false，表示正向遍历，为true表示反向遍历
   */
  void for_each(std::function<void(KeyType*, ValueType*)> func,
                bool reverse = false) const;

 private:
  uint32_t _size;
  node_type* _head;
  node_type* _tail;
  KeyCompareFunc _key_compare_func;
};  // class skiplist

template <typename KeyType, typename ValueType, typename KeyCompareFunc>
skiplist<KeyType, ValueType, KeyCompareFunc>::skiplist()
    : _size(0), _head(nullptr), _tail(nullptr) {
  _head = reinterpret_cast<node_type*>(malloc(sizeof(node_type)));
  _head->forward = new node_type*[skiplist_space::MAX_LEVEL];
  _head->pre = nullptr;
  memset(_head->forward, 0x0, sizeof(node_type*) * skiplist_space::MAX_LEVEL);
}

template <typename KeyType, typename ValueType, typename KeyCompareFunc>
skiplist<KeyType, ValueType, KeyCompareFunc>::skiplist(const skiplist& other)
    : skiplist() {
  for (node_type* it = other._tail; it != other._head; it = it->pre) {
    insert(it->key, it->value);
  }
}

template <typename KeyType, typename ValueType, typename KeyCompareFunc>
skiplist<KeyType, ValueType, KeyCompareFunc>&
skiplist<KeyType, ValueType, KeyCompareFunc>::operator=(const skiplist& other) {
  if (this == &other) {
    return *this;
  }
  clear();
  for (node_type* it = other._tail; it != other._head; it = it->pre) {
    insert(it->key, it->value);
  }
  return *this;
}

template <typename KeyType, typename ValueType, typename KeyCompareFunc>
skiplist<KeyType, ValueType, KeyCompareFunc>::~skiplist() {
  clear();
  delete[] _head->forward;
  free(_head);
}

template <typename KeyType, typename ValueType, typename KeyCompareFunc>
bool skiplist<KeyType, ValueType, KeyCompareFunc>::compare_node(
    node_type* node, const KeyType& key) const {
  return node != nullptr && _key_compare_func(node->key, key);
}

template <typename KeyType, typename ValueType, typename KeyCompareFunc>
void skiplist<KeyType, ValueType, KeyCompareFunc>::clear() {
  node_type* cur = _head->forward[0];
  while (_size) {
    _size--;
    node_type* nxt = cur->forward[0];
    delete[] cur->forward;
    delete cur;
    cur = nxt;
  }
  memset(_head->forward, 0x0, sizeof(node_type*) * skiplist_space::MAX_LEVEL);
}

template <typename KeyType, typename ValueType, typename KeyCompareFunc>
void skiplist<KeyType, ValueType, KeyCompareFunc>::swap(skiplist* list) {
  std::swap(list->_size, this->_size);
  std::swap(list->_head, this->_head);
  std::swap(list->_tail, this->_tail);
}

template <typename KeyType, typename ValueType, typename KeyCompareFunc>
uint32_t skiplist<KeyType, ValueType, KeyCompareFunc>::size() const {
  return _size;
}

template <typename KeyType, typename ValueType, typename KeyCompareFunc>
bool skiplist<KeyType, ValueType, KeyCompareFunc>::search(
    const KeyType& key, ValueType* value) const {
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

template <typename KeyType, typename ValueType, typename KeyCompareFunc>
void skiplist<KeyType, ValueType, KeyCompareFunc>::insert(
    const KeyType& key, const ValueType& value) {
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
  uint32_t new_node_level = skiplist_space::random_level();
  node_type* new_node =
      new node_type{.key = key,
                    .value = value,
                    .pre = cur,
                    .forward = new node_type*[new_node_level]};
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

template <typename KeyType, typename ValueType, typename KeyCompareFunc>
bool skiplist<KeyType, ValueType, KeyCompareFunc>::erase(const KeyType& key,
                                                         ValueType* value) {
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

template <typename KeyType, typename ValueType, typename KeyCompareFunc>
void skiplist<KeyType, ValueType, KeyCompareFunc>::for_each(
    std::function<void(KeyType*, ValueType*)> func, bool reverse) const {
  if (reverse) {
    for (node_type* it = _tail; it != _head; it = it->pre) {
      func(&(it->key), &(it->value));
    }
  } else {
    for (node_type* it = _head->forward[0]; it != nullptr;
         it = it->forward[0]) {
      func(&(it->key), &(it->value));
    }
  }
}

}  // namespace data_structure

}  // namespace sun

#endif  // SUN_DATA_STRUCTURE_SKIPLIST_H_