#include <iostream>
#include <memory>
#include <utility>

template <typename T>
struct Node {
  T value;
  std::unique_ptr<Node> left, right;
  Node* parent;
public:
  using value_type=T;

};


template <typename node_t>
class iterator {
  node_t* current;
public: 
  using value_type = typename node_t::value_type;
  using reference = value_type&;
  using pointer = value_type*;
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
// overloading operators ++, *, ->...

};


template <typename kT, typename vT, typename cmp=std::less<kT>>
class bst {
public:
  bst(cmp x): op{x} {}      // custom constructor
  using node_type = Node<std::pair<const kT, vT>>;
  List() noexcept = default;      // default constructor
  //begin, end
  std::pair<iterator, bool> insert(std::pair<const kT, vT>);
private:
  cmp op;
  std::unique_ptr<node_type> root;

};

template <typename kT, typename vT, typename cmp>
std::pair<typename bst<kT,vT,cmp>::iterator, bool>    bst<kT,vT,cmp>::insert(const std::pair<const kT, vT> & new_pair) {
  // here node_type is known
  node_type* jumper {root.get()};
  if(!jumper)
    break;
  if( cmp( std::get<0>(jumper->value) , std::get<0>(new_pair) ) ) {
    // go right
    if(jumper->right)       // if right child is different from nullptr
      jumper = (jumper->right).get();
    else
      // insert new node here
      jumper->right = make_unique<node_type>(new_pair, &jumper);
  else
    // go left
    if(jumper->left)       // if right left is different from nullptr
      jumper = (jumper->left).get();
    else
      break;

}


int main() {

  bst<int,char> mybst{};   // calls the implicit default constructor

  mybst.insert(std::pair<int,char>{8,'a'});     // calls the constructor of std::pair

}
