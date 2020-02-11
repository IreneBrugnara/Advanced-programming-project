#ifndef __bst__
#define __bst__



#include <iostream>
#include <memory>   // smart pointer
#include <utility>  // std::pair
#include <functional>
#include <vector>

#include "node.hpp"
#include "iterator.hpp"

enum class where {
  right,
  left,
  equal,
  empty
};


template <typename kT, typename vT, typename cmp=std::less<kT>>
class bst {
public:
  bst(cmp x): op{x} {}      // custom constructor
  using node_type = Node<std::pair<const kT, vT>>;
  bst() noexcept = default;      // default constructor
  bst(const bst & b) {            // copy constructor
    root = std::make_unique<node_type>(b.root, nullptr);
  }
  bst& operator=(const bst& b) {    // copy assignment
    root = std::make_unique<node_type>(b.root, nullptr);
    return *this;
  }
  bst& operator=(bst&& b) noexcept = default;     // move assignment
  bst(bst&& b) noexcept = default;                // move constructor
  //begin, end
  using iterator = _iterator<node_type, typename node_type::value_type>;
  using const_iterator = _iterator<node_type, const typename node_type::value_type>;
  std::pair<iterator, bool> insert(const std::pair<const kT, vT> &);
  std::pair<iterator, bool> insert(std::pair<const kT, vT> &&);
  iterator find(const kT& x); 
  void print_tree(node_type*) const;   // just for us to debug and check tree structure - move to private when submitting
  void call_print_tree() {print_tree(root.get());}
  iterator begin() noexcept;
  iterator end() noexcept { return iterator{nullptr}; }

  vT& operator[] (const kT& x);
  vT& operator[] (kT&& x);

  const_iterator begin() const noexcept;
  const_iterator cbegin() const noexcept;
  const_iterator end() const noexcept { return const_iterator{nullptr}; }
  const_iterator cend() const noexcept { return const_iterator{nullptr}; }

  //std::pair<node_type*, where> locator(const kT& key);//move to private

  void clear() noexcept {root.reset();}

  template< class... Types>
  std::pair<iterator,bool> emplace(Types&&... args);

  void erase(const kT& x);

  void balance();

friend std::ostream& operator<<(std::ostream& os, const bst< kT,vT>& mybst){
  for(auto it=mybst.begin(); it!=mybst.end(); it++)
    os << "( " << it->first << ", " << it->second << ")  ";
  return os;
 }

  
private:
  cmp op;
  std::unique_ptr<node_type> root;

  std::pair<node_type*, where> locator(const kT& key);
  // if the key is already present in a node, locator returns an iterator to this node and where=equal; if the key is not present and should go on the right child of a node, locator returns this node and where=right; similarly for left

};

#include "bst_bits.hpp"

#endif
