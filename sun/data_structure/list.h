/**
 * @file list.h
 * @author sun-moon-star-star
 */

#ifndef SUN_DATA_STRUCTURE_LIST_H_
#define SUN_DATA_STRUCTURE_LIST_H_

namespace sun::data_structure {

namespace __list {

template <typename T>
struct list_node {
  list_node(const T& data, list_node* nxt) : data(data), nxt(nxt) {}
  list_node(T&& data, list_node* nxt) : data(std::move(data)), nxt(nxt) {}
  list_node(const T& data) : list_node(data, nullptr) {}
  list_node(T&& data) : list_node(std::move(data), nullptr) {}

  T data;
  list_node* nxt;
};

}  // namespace __list

template <typename T>
class list {
 private:
  using list_node = __list::list_node<T>;

  using iterator = struct final {
    iterator(list_node* node) : _node(node) {}

    bool next() {
      if (!_node) {
        return false;
      }
      _node = _node->nxt;
      return true;
    }

    T* operator->() const { return reinterpret_cast<T*>(_node->data); }

    iterator& operator++() {
      next();
      return *this;
    }

    bool valid() const { return _node != nullptr; }

   private:
    list_node* _node;
  };

 public:
  list() : _head(nullptr), _tail(nullptr), _size(0) {}

  const uint64_t size() const { return _size; }

  iterator begin() { return iterator(_head); }

  bool push(const iterator& it, const T& t) {
    if (!it.valid()) {
      return false;
    }

    const list_node* pre = *reinterpret_cast<const list_node**>(&it);

    list_node* new_node = new list_node(t, pre->next);
    pre->next = new_node;

    return true;
  }

  bool push(const T& t) {}

 private:
  list_node* _head;
  list_node* _tail;
  uint64_t _size;
};

}  // namespace sun::data_structure

#endif  // SUN_DATA_STRUCTURE_LIST_H_