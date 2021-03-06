#ifndef __bst_bits__
#define __bst_bits__

#include <iostream>
#include <memory>   		// smart pointer
#include <utility>  		// std::pair
#include <vector>



// this is a support function used in insert(), find(), operator[]
template <typename kT, typename vT, typename cmp>
std::pair<typename bst<kT,vT,cmp>::node_type*, where> bst<kT,vT,cmp>::locator(const kT& key) const {

  // here node_type is known
  node_type* jumper {root.get()};
  
  if(!jumper) {     // if the tree is empty, create root node
    return std::pair<node_type*, where>{root.get(), where::empty};
  }

  while(true)
  {
    if( op( (jumper->value).first , key ) )			// if current key is less than input key
    {
      if(jumper->right)       // if right child is different from nullptr
        jumper = (jumper->right).get();    // go right
      else {
       return std::pair<node_type*, where>{jumper, where::right};
      }
    }
    else if ( op( key , (jumper->value).first ) )
    {
      if(jumper->left)       // if right left is different from nullptr
        jumper = (jumper->left).get();      // go left
      else {
        return std::pair<node_type*, where>{jumper, where::left};
      }
    }
    else     // the keys are equal (the key is already present in the tree)
    {
      return std::pair<node_type*, where>{jumper, where::equal};
    }
  }

}



// INSERT FUNCTIONS 

template <typename kT, typename vT, typename cmp>
std::pair<typename bst<kT,vT,cmp>::iterator, bool> bst<kT,vT,cmp>::insert(const std::pair<const kT, vT> & new_pair) {
  auto info = locator(new_pair.first);    // get information of what to do from function locator
  // info is std::pair of (node*, where)

  switch (info.second) {
    case where::empty: {
      root = std::make_unique<node_type>(new_pair, nullptr);
      return std::pair<iterator, bool>{iterator{root.get()}, true};
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
    default:      // this case should never occur
      return std::pair<iterator, bool>{iterator{nullptr}, false};
  }
}

template <typename kT, typename vT, typename cmp>
std::pair<typename bst<kT,vT,cmp>::iterator, bool> bst<kT,vT,cmp>::insert(std::pair<const kT, vT> && new_pair) {
  auto info = locator(new_pair.first);    // get information of what to do from function locator
  // info is std::pair of (node*, where)

  switch (info.second) {
    case where::empty: {
      root = std::make_unique<node_type>(std::move(new_pair), nullptr);
      return std::pair<iterator, bool>{iterator{root.get()}, true};
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
   default:
     return std::pair<iterator, bool>{iterator{nullptr}, false};
  }
  
}

// EMPLACE FUNCTION
template <typename kT, typename vT, typename cmp>
template< class... Types>
std::pair<typename bst<kT,vT,cmp>::iterator,bool> bst<kT,vT,cmp>::emplace(Types&&... args) {
  return insert(std::pair<const kT,vT>{std::forward<Types>(args)...});
}



//FIND FUNCTIONS

template <typename kT, typename vT, typename cmp>
typename bst<kT,vT,cmp>::iterator bst<kT,vT,cmp>::find(const kT& x){
  auto info = locator(x);

  if(info.second == where::equal)
    return iterator{info.first};
  else{
    return end();
  }

}

template <typename kT, typename vT, typename cmp>
typename bst<kT,vT,cmp>::const_iterator bst<kT,vT,cmp>::find(const kT& x) const {
  auto info = locator(x);

  if(info.second == where::equal)
    return const_iterator{info.first};
  else{
    return cend();
  }

}

//PRINT FUNCTION (JUST FOR CHECK)

template <typename kT, typename vT, typename cmp>
void bst<kT,vT,cmp>::print_tree(node_type* jumper) const {

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


//SUBSCRIPT OPERATORS

template <typename kT, typename vT, typename cmp>
vT& bst<kT,vT,cmp>::operator[] (const kT& x){
  const std::pair<const kT, vT> input{x,vT{}};
  auto info = insert(input);
  return (info.first)->second;

}

template <typename kT, typename vT, typename cmp>
vT& bst<kT,vT,cmp>::operator[] (kT&& x){
  auto info = insert(std::pair<const kT, vT>{std::move(x),vT{}});
  
  return (info.first)->second;
  
}

// ERASE FUNCTION

template <typename kT, typename vT, typename cmp>
void bst<kT,vT,cmp>::erase(const kT& x) {
  // find node to be erased and get an iterator pointing to it
  auto mypair = locator(x);
  auto eraseme = mypair.first;
  if(mypair.second!=where::equal)
    return;

  // attach left subtree of eraseme to the leftmost node of the right subtree of eraseme
  if (eraseme->right && eraseme->left) {
    auto it = iterator{eraseme};
    ++it;
    (it.current)->left = std::move(eraseme->left);
    it.current->left->parent = it.current;
  }

  // if eraseme is the root
  if(!eraseme->parent) {
    eraseme->right->parent = nullptr;
    root = std::move(eraseme->right);
    return;
  }

// if eraseme is a right child
  if(eraseme->parent->right.get()==eraseme) {
    if(eraseme->right) {
    // attach right subtree of eraseme to the parent of eraseme
      eraseme->right->parent = eraseme->parent;
      (eraseme->parent)->right = std::move(eraseme->right);  // this also deletes eraseme
    }
    else if (eraseme->left) { 			// eraseme has only a left child
      eraseme->left->parent = eraseme->parent;
      (eraseme->parent)->right = std::move(eraseme->left);   // this also deletes eraseme
    }  
    else {					// eraseme has no children
      (eraseme->parent->right).reset();
    }
  }
// if eraseme is instead a left child
  else {
    if (eraseme->right){
		// attach right subtree of eraseme to the parent of eraseme
      eraseme->right->parent = eraseme->parent;
      (eraseme->parent)->left = std::move(eraseme->right);  // this also deletes eraseme
    }
    else if (eraseme->left) { 			// eraseme has only a left child
      eraseme->left->parent = eraseme->parent;
      (eraseme->parent)->left = std::move(eraseme->left);   // this also deletes eraseme
    }  
    else {					// eraseme has no children
      (eraseme->parent->left).reset();
    }

  }

}


// BALANCE FUNCTION

template <typename kT, typename vT, typename cmp>
void bst<kT, vT, cmp>::balance() {
  std::vector<std::pair<const kT, vT>> myarr;
  for(auto it=begin(); it!=end(); ++it){
    myarr.push_back(std::move(*it));				// move elements from bst to vector
  }
  clear();
  root = std::make_unique<node_type>(myarr, 0, myarr.size()-1, nullptr);		// start reconstructing bst

}


//ITERATOR'S FUNCTIONS

template <typename kT, typename vT, typename cmp>
_iterator<Node<std::pair<const kT, vT>>, typename Node<std::pair<const kT, vT>>::value_type> bst<kT,vT,cmp>::begin() noexcept
{
  node_type* jumper{root.get()};
  if(!jumper)
    return iterator{jumper};
  while(jumper->left)									// go as far left as I can
    jumper = (jumper->left).get();
  return iterator{jumper};
}

template <typename kT, typename vT, typename cmp>
_iterator<Node<std::pair<const kT, vT>>, const typename Node<std::pair<const kT, vT>>::value_type> bst<kT,vT,cmp>::begin() const noexcept
{
  return cbegin();
}

template <typename kT, typename vT, typename cmp>
_iterator<Node<std::pair<const kT, vT>>, const typename Node<std::pair<const kT, vT>>::value_type> bst<kT,vT,cmp>::cbegin() const noexcept
{
  node_type* jumper{root.get()};
  if(!jumper)
    return const_iterator{jumper}; 
  else {
    while(jumper->left)
      jumper = (jumper->left).get();
    return const_iterator{jumper};
  }
}

#endif
