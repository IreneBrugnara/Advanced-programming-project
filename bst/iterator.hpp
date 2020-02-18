#ifndef __iterator__
#define __iterator__


#include <iostream>


template <typename kT, typename vT, typename cmp>
class bst;

template <typename node_t, typename O>
class _iterator {
  node_t* current;

public: 
  using value_type = O;
  using reference = value_type&;
  using pointer = value_type*;
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;

  // overloading operators ++, *, ->...
  _iterator(node_t* new_node): current{new_node} {}        // custom constructor
  reference operator*() const noexcept { return current->value; }
  pointer operator->() const noexcept { return &(*(*this)); }
  friend bool operator==(const _iterator& a, const _iterator& b) {
    return a.current == b.current;
  }
  friend bool operator!=(const _iterator& a, const _iterator& b) {
    return !(a == b);
  }

  _iterator& operator++() noexcept;           // pre-increment
  _iterator operator++(int) noexcept {        // post-increment
    _iterator tmp{current};
    ++(*this);
    return tmp;
  }

template <typename kT, typename vT, typename cmp>
  friend void bst<kT,vT,cmp>::erase(const kT& x);  // so that I can do iterator.current
};

template <typename node_t, typename O>
_iterator<node_t, O>& _iterator<node_t, O>::operator++() noexcept  // pre-increment
{
  if(current->right)      // if I have a right child
  {
    current = (current->right).get();     // go down right
    while(current->left)      // while I have a left child
      current = (current->left).get();     // go down left
  }
  else
  {
    while((current->parent) && ((current->parent)->right).get() == current)
      current = current->parent;          // go up
    current = current->parent;    // one more jump
  }
  return *this;
}


#endif
