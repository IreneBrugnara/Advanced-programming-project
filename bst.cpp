#include <iostream>
#include <memory>
#include <utility>
#include <functional>

template <typename T>
struct Node {
  T value;
  std::unique_ptr<Node> left, right;
  Node* parent;
public:
  using value_type=T;
  Node(const T& val, Node* p): value{val}, parent{p} {}	// custom constructor - note that value{val} calls the copy constructor of T
};


template <typename node_t>
class _iterator {
  node_t* current;
public: 
  using value_type = typename node_t::value_type;
  using reference = value_type&;
  using pointer = value_type*;
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
// overloading operators ++, *, ->...
  _iterator(node_t* new_node): current{new_node} {}        // custom constructor
  reference operator*() const noexcept { return current->value; }
  pointer operator->() const noexcept { return &(*(*this)); }
  reference operator++() noexcept;
  friend bool operator==(const _iterator& a, const _iterator& b) {
    return a.current == b.current;
  }
  // check why friend????
};


template <typename kT, typename vT, typename cmp=std::less<kT>>
class bst {
public:
  bst(cmp x): op{x} {}      // custom constructor
  using node_type = Node<std::pair<const kT, vT>>;
  bst() noexcept = default;      // default constructor
  //begin, end
  using iterator = _iterator<node_type>;
  std::pair<iterator, bool> insert(const std::pair<const kT, vT> &);
  void print_tree(node_type*);   // just for us to debug and check tree structure
  iterator begin();
  iterator end() { return iterator{nullptr}; }

private:
  cmp op;
  std::unique_ptr<node_type> root;

};
////////////////////////////////////////////////////////////////////////////////////
template <typename kT, typename vT, typename cmp>
std::pair<typename bst<kT,vT,cmp>::iterator, bool> bst<kT,vT,cmp>::insert(const std::pair<const kT, vT> & new_pair) {
  // here node_type is known
  node_type* jumper {root.get()};
  bool flag{0};     // just to signal when we have to exit from while loop

  if(!jumper) {     // if the tree is empty, create root node
    root = std::make_unique<node_type>(new_pair, nullptr);
    std::cout << "root created\n";
    std::cout << "start of tree \n";
    print_tree(root.get());
    std::cout << "\nend of tree \n";
    return std::pair<iterator, bool>{iterator{root.get()}, true};
   }

  while(true) {
    if( op( (jumper->value).first , (new_pair).first ) )
    {
      if(jumper->right)       // if right child is different from nullptr
        jumper = (jumper->right).get();    // go right
      else {
        // insert new node here
        jumper->right = std::make_unique<node_type>(new_pair, jumper);
        std::cout << "start of tree \n";
        print_tree(root.get());
        std::cout << "\nend of tree \n";
        return std::pair<iterator, bool>{iterator{(jumper->right).get()}, true};
      }
    }
    else if ( op( std::get<0>(new_pair) , std::get<0>(jumper->value) ) )
    {
      // go left
      if(jumper->left)       // if right left is different from nullptr
        jumper = (jumper->left).get();
      else {
        // insert new node here
        jumper->left = std::make_unique<node_type>(new_pair, jumper);
        std::cout << "start of tree \n";
        print_tree(root.get());
        std::cout << "\nend of tree \n";
        return std::pair<iterator, bool>{iterator{(jumper->left).get()}, true};
      }
    }
    else     // the keys are equal (the key is already present in the tree)
    {
      std::cout << "start of tree \n";
      print_tree(root.get());
      std::cout << "\nend of tree \n";
      return std::pair<iterator, bool>{iterator{jumper}, false};
    }
  }


}

template <typename kT, typename vT, typename cmp>
void bst<kT,vT,cmp>::print_tree(node_type* jumper) {
                      //node_type* jumper{root.get()};

  std::cout << jumper->value.first;
  // call function recursively on left child
  if(jumper->left) {
  std::cout << " ( ";
  print_tree((jumper->left).get());
  std::cout << ", ";
  }
  else
  {
    std::cout << " ( -,";
  }
  // call function recursively on right child
  if(jumper->right) {
  print_tree((jumper->right).get());
  std::cout << " ) ";
  }
  else
  {
    std::cout << " -)";
  }
  return;
};

////////////////////////////////////////////////////
template <typename kT, typename vT, typename cmp>
_iterator<Node<std::pair<const kT, vT>>> bst<kT,vT,cmp>::begin()
{
  node_type* jumper{root.get()};
  while(jumper->left)
    jumper = (jumper->left).get();
  return iterator{jumper};
}

template <typename node_t>
_iterator<node_t<std::pair<const kT, vT>>>& operator++() noexcept  // pre-increment
{
  if(current->right)      // if I have a right child
  {
    current = (current->right).get();     // go down right
    while(current->left)      // while I have a left child
      current = (current->left).get();     // go down left
    return *this;
  }
  else
  {
    if(current->parent)
      return nullptr;
    while((current->parent)->right) == current)
      current = current->parent;          // go up
    current = current->parent;
    return *this;
  }

}


int main() {

  bst<int,char> mybst{};   // calls the implicit default constructor
  auto p = std::pair<int,char>{5,'a'};    // calls the constructor of std::pair
  auto output = mybst.insert(p); 
  p = std::pair<int,char>{4,'b'};
  std::cout << "Value inserted? (0 false, 1 true)" << output.second;
  mybst.insert(p);
  p = std::pair<int,char>{7,'c'};
  mybst.insert(p);
  p = std::pair<int,char>{9,'c'};
  mybst.insert(p);
  p = std::pair<int,char>{6,'c'};
  mybst.insert(p);
  p = std::pair<int,char>{1,'c'};
  mybst.insert(p);
  p = std::pair<int,char>{7,'c'};
  output = mybst.insert(p);
  std::cout << "Value inserted? (0 false, 1 true)" << output.second << "\n";

  auto mybegin = mybst.begin();
  std::cout << "first key: " << mybegin->first << "first value: " << mybegin->second << "\n";
 // for(auto it=mybst.begin(); it!=mybst.end(); ++it)              // for-range loop
 //   std::cout << it->first << "   ";
}

