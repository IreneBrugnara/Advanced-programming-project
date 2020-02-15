#ifndef __node__
#define __node__



#include <iostream>
#include <memory>   // smart pointer
#include <utility>  // std::pair
//#include <functional>
#include <vector>


template <typename T>
struct Node {
  T value;
  std::unique_ptr<Node> left, right;
  Node* parent;
public:
  using value_type=T;
  Node(const T& val, Node* p): value{val}, parent{p} {}	// custom constructor - note that value{val} calls the copy constructor of T
  Node(T&& val, Node* p): value{std::move(val)}, parent{p} {}  // calls move constructor of type T
  Node(const std::unique_ptr<Node>& p, Node* myparent): value{p->value}, parent{myparent} {      // ad-hoc constructor for bst copy
    if(p->right)
      right = std::make_unique<Node>(p->right, this);
    if(p->left)
      left = std::make_unique<Node>(p->left, this);
    // if *p is a terminal node, the two if conditions are not met and the members right and left are initialized to nullptr by default
  }
  Node(std::vector<T> &v, int start, int end, Node* myparent): value {std::move(v[(start+end)/2])}, parent{myparent} {
    int M{(start+end)/2};
    if(start<=M-1)
      left = std::make_unique<Node>(v, start, M-1, this);
    if(M+1<=end)
      right = std::make_unique<Node>(v, M+1, end, this);
  }

};


#endif
