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
  Node(const T& val, Node* p): value{val}, parent{p} {
    std::cout << "Node copy constructed\n";
  }	// custom constructor - note that value{val} calls the copy constructor of T
  Node(T&& val, Node* p): value{std::move(val)}, parent{p} {
    std::cout << "Node move constructed\n";
  }  // calls move constructor of type T
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
 // std::pair<iterator, bool> insert(std::pair<const kT, vT> &&);
  iterator find(const kT& x); 
  void print_tree(node_type*);   // just for us to debug and check tree structure
  iterator begin();
  iterator end() { return iterator{nullptr}; }

  vT& operator[] (const kT& x);
  vT& operator[] (kT&& x);

  const_iterator begin() const;
  const_iterator end() const { return const_iterator{nullptr}; }

  //std::pair<node_type*, where> locator(const kT& key);//move to private
 
private:
  cmp op;
  std::unique_ptr<node_type> root;

  std::pair<node_type*, where> locator(const kT& key);
  // if the key is already present in a node, locator returns an iterator to this node and where=equal; if the key is not present and should go on the right child of a node, locator returns this node and where=right; similarly for left

};

// this is a support function used in insert(), find(), operator[]
template <typename kT, typename vT, typename cmp>
std::pair<typename bst<kT,vT,cmp>::node_type*, where> bst<kT,vT,cmp>::locator(const kT& key) {

  // here node_type is known
  node_type* jumper {root.get()};
  bool flag{0};     // just to signal when we have to exit from while loop

  if(!jumper) {     // if the tree is empty, create root node
    //root = std::make_unique<node_type>(new_pair, nullptr);
    return std::pair<node_type*, where>{root.get(), where::empty};
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
      
        return std::pair<node_type*, where>{jumper, where::right};
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
        return std::pair<node_type*, where>{jumper, where::left};
      }
    }
    else     // the keys are equal (the key is already present in the tree)
    {
      return std::pair<node_type*, where>{jumper, where::equal};
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
  std::cout << "l-value insert called\n";
  // here node_type is known
  auto info {locator(new_pair.first)};    // get information of what to do from function locator
// info is std::pair of (node*, where)

  switch (info.second) {
    case where::empty: {
      root = std::make_unique<node_type>(new_pair, nullptr);
      return std::pair<iterator, bool>{iterator{info.first}, true};
    }
    case where::equal: {
      return std::pair<iterator, bool>{iterator{info.first}, false};
    }
    case where::right: {
      (info.first)->right = std::make_unique<node_type>(new_pair, (info.first));
      return std::pair<iterator, bool>{iterator{(info.first)->right.get()}, true};
    }
   case where::left: {
      (info.first)->left = std::make_unique<node_type>(new_pair, (info.first));
      return std::pair<iterator, bool>{iterator{(info.first)->left.get()}, true};
    }

     //default:
  }
}
/*
template <typename kT, typename vT, typename cmp>
std::pair<typename bst<kT,vT,cmp>::iterator, bool> bst<kT,vT,cmp>::insert(std::pair<const kT, vT> && new_pair) {
  std::cout << "r-value insert called\n";
  // here node_type is known
  auto info {locator(new_pair.first)};    // get information of what to do from function locator
// info is std::pair of (node*, where)

  switch (info.second) {
    case where::empty: {
      root = std::make_unique<node_type>(std::move(new_pair), nullptr);
      return std::pair<iterator, bool>{iterator{info.first}, true};
    }
    case where::equal: {
      return std::pair<iterator, bool>{iterator{info.first}, false};
    }
    case where::right: {
      (info.first)->right = std::make_unique<node_type>(std::move(new_pair), (info.first));
      return std::pair<iterator, bool>{iterator{(info.first)->right.get()}, true};
    }
   case where::left: {
      (info.first)->left = std::make_unique<node_type>(std::move(new_pair), (info.first));
      return std::pair<iterator, bool>{iterator{(info.first)->left.get()}, true};
    }

     //default:
  }
  
}
*/

template <typename kT, typename vT, typename cmp>
typename bst<kT,vT,cmp>::iterator bst<kT,vT,cmp>::find(const kT& x){

  auto info{locator(x)};

  if(info.second == where::equal)
    return iterator{info.first};
  else{
    return end();
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

template <typename kT, typename vT, typename cmp>
vT& bst<kT,vT,cmp>::operator[] (const kT& x){
  std::cout << "l-value subscript\n";
  //const std::pair<const kT, vT> input{x,vT{}};
  auto info {insert(std::pair<const kT, vT> {x,vT{}})};
  return (info.first)->second;


  //TODO try to use not insert function but locator (why? here we initialize a std::pair)
  
}

template <typename kT, typename vT, typename cmp>
vT& bst<kT,vT,cmp>::operator[] (kT&& x){
  std::cout << "r-value subscript\n";
  auto info {insert(std::pair< kT, vT>{std::move(x),vT{}})};

  
  return (info.first)->second;
  
}


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

class resource {
private:
  int x;
public:

  resource(): x{5} {std::cout << "Resource default constructed\n";}

  resource(const resource &res): x{res.x} {std::cout << "Resource copy constructed\n";}

  resource& operator=(const resource &res) {
    x = res.x;
    std::cout << "Resource copy assigned\n";
    return *this;
  }

  resource(resource &&res): x{std::move(res.x)} {std::cout << "Resource move constructed\n";}

  resource& operator=(resource &&res) {
    x = std::move(res.x);
    std::cout << "Resource move assigned\n";
    return *this;
  }


  void prova(std::pair<int,resource>&& pair) {
    std::cout << "r-value called\n";
  }

  void prova(const std::pair<int,resource>& pair) {
    std::cout << "l-value called\n";
  }
  
  void printX() const {std::cout << "x= " << x << "\n";}

  friend bool operator<(const resource &res1, const resource &res2);
};

bool operator<(const resource &res1, const resource &res2) {
    return res1.x < res2.x;
  }


int main() {
/**************
  (05/02) CHECK FOR BST IMPLEMENTATION 
  bst<int,char> mybst{};   // calls the implicit default constructor
  auto w = mybst.find(5);

  if(w==nullptr)
    std::cout << "nullptr find" << std::endl;

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
    std::cout << it->first << "   ";

  std::cout << std::endl;

    Check for locator function
  auto result{mybst.locator(5)};
  std::cout << "Result " << static_cast<int>(result.second) << std::endl;
  
 
  auto r = mybst.find(1);
  std::cout << r->first << std::endl;

  r = mybst.find(47);

  if(r==mybst.end())
    std::cout << "Value not found" << std::endl;


  auto value = mybst[56];

  std::cout<< "Key: 1, Value: " << value << std::endl;


******************************************************/

// (06/02) CHECK MOVE-COPY SEMANTICS

  bst<int,char> prova{};
  resource myres{};  // should be called default ctor
  myres.printX();
  std::pair<int,char> mypair {1, 'c'}; //should be called a copy ctor
  std::cout << mypair.first << "\n";
  std::cout << "Now I insert an l value \n";
  std::cout << &mypair;
  prova.insert(mypair); //should be called a copy ctor
  std::cout << " -------------------------- \n";
/*
  std::cout << "\n Now I Insert r-value\n";
  prova.insert(std::pair<int,resource>{2, resource{}} );
  //should be called a default ctor (resource{}) and move ctor inside insert to init std::pair
  std::cout << " -------------------------- \n";
  std::cout << "\n SUBSCRIPT OPERATOR";
  int prova_int = 2;
  std::cout << "L-value version : prova[myres]=" << prova[prova_int] << "\n";
  //should be called a copy ctor of std::pair
  std::cout << "R-value version : prova[myres]=" << prova[2] << "\n";
  //should be called a move ctor of std::pair

  resource myres{};
  myres.printX();
  std::cout << "------------------";
  auto ress = resource(std::move(myres));
  ress.printX();
*/
 // resource&& ref = resource{};
 // auto resss = std::move(ref);
  //std::cout << "------------------";
  //std::pair<const resource, int> res = std::make_pair(std::move(ress), 3);
 // std::cout << "prova[myres]=" << find << "\n";
/*  
  std::cout << 
  resource myres2{};
  std::pair<int,resource> res2{2,myres2};
  std::cout << "should be l-value";
  myres.prova(res2);
  std::cout << "should be r-value";
  myres.prova(std::pair<int,resource>{});
  
  bst<const int, char> bbst{};
  std::cout << "should be r-value\n";
  bbst.insert(std::pair<const int, char>{1,'c'});
  std::cout << "should be l-value\n";
  std::pair<const int, char> myp {1,'c'};
  bbst.insert(myp);
*/  

  return 0;
}

