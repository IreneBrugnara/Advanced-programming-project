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
class iterator {
  node_t* current;
public: 
  using value_type = typename node_t::value_type;
  using reference = value_type&;
  using pointer = value_type*;
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
// overloading operators ++, *, ->...
  iterator(node_t* new_node): current{new_node} {}        // custom constructor

};


template <typename kT, typename vT, typename cmp=std::less<kT>>
class bst {
public:
  bst(cmp x): op{x} {}      // custom constructor
  using node_type = Node<std::pair<const kT, vT>>;
  bst() noexcept = default;      // default constructor
  //begin, end
  // std::pair<iterator, bool>
  void insert(const std::pair<const kT, vT> &);
  void print_tree(node_type*);   // just for us to debug and check tree structure
private:
  cmp op;
  std::unique_ptr<node_type> root;

};
////////////////////////////////////////////////////////////////////////////////////
template <typename kT, typename vT, typename cmp>
//std::pair<typename bst<kT,vT,cmp>::iterator, bool>   
void bst<kT,vT,cmp>::insert(const std::pair<const kT, vT> & new_pair) {
  // here node_type is known
  node_type* jumper {root.get()};
  bool flag{0};

  if(!jumper) {
    root = std::make_unique<node_type>(new_pair, nullptr);
    std::cout << "root created\n";
    std::cout << "start of tree \n";
    print_tree(root.get());
    std::cout << "\nend of tree \n";
    return;
   }

  while(!flag) {
  if( op( (jumper->value).first , (new_pair).first ) )
  {
    if(jumper->right)       // if right child is different from nullptr
      jumper = (jumper->right).get();    // go right
    else {
      // insert new node here
      jumper->right = std::make_unique<node_type>(new_pair, jumper);
      flag=1;
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
      flag=1; //std::pair<iterator, bool>{iterator{jumper}, true};
    }
  }
  else     // the keys are equal (the key is already present in the tree)
  {
    flag=1; //std::pair<iterator, bool>{iterator{jumper}, false};
  }
  }
  std::cout << "start of tree \n";
  print_tree(root.get());
  std::cout << "\nend of tree \n";
  return;
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

int main() {

  bst<int,char> mybst{};   // calls the implicit default constructor
  auto p = std::pair<int,char>{5,'a'};    // calls the constructor of std::pair
  mybst.insert(p); 
  p = std::pair<int,char>{3,'b'};
  mybst.insert(p);
  p = std::pair<int,char>{7,'c'};
  mybst.insert(p);
  p = std::pair<int,char>{9,'c'};
  mybst.insert(p);
  p = std::pair<int,char>{6,'c'};
  mybst.insert(p);
  p = std::pair<int,char>{10,'c'};
  mybst.insert(p);
  p = std::pair<int,char>{4,'c'};
  mybst.insert(p);
}

