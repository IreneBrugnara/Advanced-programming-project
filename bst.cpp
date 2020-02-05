#include <iostream>
#include <memory>
#include <utility>
#include <functional>

enum class where {
  right,
  left,
  equal,
  empty
};

template <typename T>
struct Node {
  T value;
  std::unique_ptr<Node> left, right;
  Node* parent;
public:
  using value_type=T;
  Node(const T& val, Node* p): value{val}, parent{p} {}	// custom constructor - note that value{val} calls the copy constructor of T
};


template <typename node_t, typename O>
class _iterator {
  node_t* current;
public: 
  using value_type = O;// typename node_t::value_type;
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
  // check why friend????
  _iterator& operator++() noexcept;           // pre-increment
  _iterator operator++(int) noexcept {        // post-increment
    _iterator tmp{current};
    ++(*this);
    return tmp;
  }
};


template <typename kT, typename vT, typename cmp=std::less<kT>>
class bst {
public:
  bst(cmp x): op{x} {}      // custom constructor
  using node_type = Node<std::pair<const kT, vT>>;
  bst() noexcept = default;      // default constructor
  //begin, end
  using iterator = _iterator<node_type, typename node_type::value_type>;
  using const_iterator = _iterator<node_type, const typename node_type::value_type>;
  std::pair<iterator, bool> insert(const std::pair<const kT, vT> &);
  void print_tree(node_type*);   // just for us to debug and check tree structure
  iterator begin();
  iterator end() { return iterator{nullptr}; }

  const_iterator begin() const;
  const_iterator end() const { return const_iterator{nullptr}; }

 
private:
  cmp op;
  std::unique_ptr<node_type> root;

  std::pair<iterator, where> locator(const kT& key);
  // if the key is already present in a node, locator returns an iterator to this node and where=equal; if the key is not present and should go on the right child of a node, locator returns this node and where=right; similarly for left

};

// this is a support function used in insert(), find(), operator[]
template <typename kT, typename vT, typename cmp>
std::pair<typename bst<kT,vT,cmp>::iterator, where> bst<kT,vT,cmp>::locator(const kT& key) {

  // here node_type is known
  node_type* jumper {root.get()};
  bool flag{0};     // just to signal when we have to exit from while loop

  if(!jumper) {     // if the tree is empty, create root node
    //root = std::make_unique<node_type>(new_pair, nullptr);
    return std::pair<iterator, where>{iterator{root.get()}, where::empty};
   }

  while(true)
  {
    if( op( (jumper->value).first , key ) )
    {
      if(jumper->right)       // if right child is different from nullptr
        jumper = (jumper->right).get();    // go right
      else {
        // insert new node here
        //jumper->right = std::make_unique<node_type>(new_pair, jumper);
      
        return std::pair<iterator, where>{iterator{jumper}, where::right};
      }
    }
    else if ( op( key , (jumper->value).first ) )
    {
      // go left
      if(jumper->left)       // if right left is different from nullptr
        jumper = (jumper->left).get();
      else {
        // insert new node here
        //jumper->left = std::make_unique<node_type>(new_pair, jumper);
        return std::pair<iterator, where>{iterator{jumper}, where::left};
      }
    }
    else     // the keys are equal (the key is already present in the tree)
    {
      return std::pair<iterator, where>{iterator{jumper}, where::equal};
    }
  }


}


/* std::cout << "start of tree \n";
        print_tree(root.get());
        std::cout << "\nend of tree \n";
*/


////////////////////////////////////////////////////////////////////////////////////
template <typename kT, typename vT, typename cmp>
std::pair<typename bst<kT,vT,cmp>::iterator, bool> bst<kT,vT,cmp>::insert(const std::pair<const kT, vT> & new_pair) {
  // here node_type is known
  auto info {locator(new_pair.first)};    // get information of what to do from function locator
// info is std::pair of (iterator, where)

  switch (info.second) {
    case where::empty: {
      root = std::make_unique<node_type>(new_pair, nullptr);
      return std::pair<iterator, bool>{info.first, true};
    }
    case where::equal: {
      return std::pair<iterator, bool>{info.first, false};
    }
    case where::right: {
      (info.first.current)->right = std::make_unique<node_type>(new_pair, info.first.current);
      return std::pair<iterator, bool>{iterator{(jumper->right).get()}, true};
    }
    

    default: 
  }

// TODO - capire se possiamo passare l'argomento by reference da locator a insert
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
_iterator<Node<std::pair<const kT, vT>>, typename Node<std::pair<const kT, vT>>::value_type> bst<kT,vT,cmp>::begin()
{
  node_type* jumper{root.get()};
  while(jumper->left)
    jumper = (jumper->left).get();
  return iterator{jumper};
}

template <typename kT, typename vT, typename cmp>
_iterator<Node<std::pair<const kT, vT>>, const typename Node<std::pair<const kT, vT>>::value_type> bst<kT,vT,cmp>::begin() const
{
  node_type* jumper{root.get()};
  while(jumper->left)
    jumper = (jumper->left).get();
  return iterator{jumper};
}


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
  

  std::cout << "for-range loop: ";
  for(auto it{mybst.begin()}; it!=mybst.end(); it++)              // for-range loop
    std::cout << it->first << "   "<<std::endl;

  /*  Check for locator function
  auto result{mybst.locator(5)};
  std::cout << "Result " << static_cast<int>(result.second) << std::endl;
  */
 



  return 0;
}

