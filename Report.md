## Binary Search Tree

Authors: Irene Brugnara, Roberto Corti.

### Structure of the classes

The aim of this project is to create a templated binary search tree. 
There are three classes: 

* ``` bst< const kT , vT , cmp = std::less<kT> >``` , that represents the binary search tree templated on a key type ```const kT``` and a value type ```vT``` . This class has as members a functional object of type ```cmp``` to compare two keys and a unique pointer ```root``` which points at the root of the tree. 

* ``` Node<T> ``` which stores as members a value of type ```T```, two unique pointers  ```right``` and ```left``` that point to the children of the node and a raw pointer to the parent node .

* ```_iterator<node_t, O>  ```  that has a raw pointer to the current node of type ```node_t```. The other template ```O``` is used to differentiate iterators and const iterators.  

The two support classes ``` Node ``` and  ```_iterator ``` are outside  ``` bst``` because they have different templates.  Class ```bst``` uses as template T of the nodes an ```std::pair<const kT, vT>``` . The key type is a constant because once a node is set the key cannot be changed. 

### Methods

In class ```iterator  ``` we implemented the overload of the operators  dereference  ```* ``` , arrow  ```-> ``` , comparison  ```==  ``` , pre and post increment  ```++ ``` .  
The increment allows to jump from one node to the successive one in the ordered sequence of the keys. The keys are in ascending order because we implemented only a forward iterator.  
In order to find this successive element we provide the following algorithm: if the current node has a right child, the successive element is the leftmost node in the right subtree; otherwise, the successive node is the current node's first right ancestor.

Class ``bst``  has the following methods. 

#### Locator

```c++
std::pair<node_type*, where> locator(const kT& key)
```

This private method is used by the functions insert, find and by operator[] . Since these functions have similar tasks, that is finding/inserting the ```key ``` in the tree, the idea is that they call a common function Locator that performs their common instructions. We introduce an enum class `` where  ``with four possible values: ```right, left, equal, empty ```.  Given the key as argument, the function looks for the ``key`` in the tree. If ``key `` is found, the returned pair contains a pointer to the node with this key and an enum with value ```equal ``` ; if it is not found, the return pair has a pointer to the node to which the ```key``` can be appended and an enum that is ```right``` or ```left``` depending on where the ```key``` has to be appended. In case of an empty tree it returns a pointer to the root and an enum with ```empty``` value. 

#### Insert

```c++ 
std::pair<iterator, bool> insert(const std::pair<const kT, vT> & new_pair);
std::pair<iterator, bool> insert(std::pair<const kT, vT> && new_pair);
```

The idea is to separate the localization of the place where to put the new node from the actual allocation of memory for the new node: the first task is perfomed by Locator and the second by Insert. 
This function inserts a ```new_pair``` in the binary search tree. At first, it calls Locator and if the enum ```where```  is not ```equal``` it performs an insertion on the right place allocating a new node with ```std::make_unique ```, returning an iterator to this node and a bool ``true``.   Instead, if the ``key`` is already present inside the tree, the function returns an iterator to the related node and ``false``.

#### Find 

 ```c++
iterator find(const kT& key);
const_iterator find(const kT& key) const;
 ```

This method calls Locator to look for a node with ```key ``` and returns an iterator to this node or ```end() ```  if the ```key``` is not found. 

#### Emplace 

``` c++
template< class... Types >
std::pair<iterator,bool> emplace(Types&&... args);
```

Emplace behaves like Insert but uses variadic template parameters, so that the user is not forced to pass and ```std::pair``` as argument but simply the two values of the pair.

#### Subscripting operator

```c++
value_type& operator[](const kT& key);
value_type& operator[](kT&& key);
```

In order to return the value of the node with ```key``` , subscripting operator calls insert function and returns the first element of the returned pair.

#### Begin

```c++
iterator begin();
const_iterator begin() const;
const_iterator cbegin() const;
```

This function returns an iterator to the node with the smallest key, which is the leftmost node of the tree.

#### End 

```c++
iterator end();
const_iterator end() const;
const_iterator cend() const;
```

End function returns an iterator to  ``nullptr ``.

#### Clear

```c++
void clear() noexcept;
```

This function clears the content of the tree by calling ```reset() ``` of root node, which recursively destroys all the nodes of the tree. 

#### Put-to operator

```c++
friend std::ostream& operator<<(std::ostream& os, const bst<kT,vT>& x);
```

This method overloads the operator ``<<`` for the bst class iterating the nodes with operator ```++``` and printing the key and value of each node in ascending order.

#### Erase

```c++
void erase(const kT& x);
```

Erase removes the node with key ```x ``` from the tree. Once this node is found with Locator function, we attach the right subtree of this node to its parent. Then, the left subtree of the erased node is attached to the leftmost node of the right subtree. This solution may lead to an unbalanced tree, but the advantage is that it requires few operations. 

#### Balance

```c++
void balance();
```

This function balances the tree with a simple algorithm: we create a vector with the node's elements in ascending order, then we create a new tree allocating as root the central element of the vector. The right and left subtrees of the root are respectively the right and left halves of the vector. This operation is repeated recursively to reconstruct the balanced tree.   
To implement this algorithm we create an ```std::vector< std::pair <const kT, vT> >``` and use ```push_back``` to move the node's elements into the vector, and eventually clear the tree. Then, we call a custom constructor of ```Node``` that recursively allocates a new node moving the elements of the vector. 

#### Copy and Move Semantics

```c++
bst (const bst& b); //copy ctor
bst& operator=(const bst& b); //copy assignment
bst (bst&& b) noexcept = default; //move ctor
bst& operator=(bst&& b) noexcept = default; //move assignment
```

The copy constructor of ```bst ``` calls recursively a custom constructor of  ```Node ```  that does a deep copy of the nodes of ```b``` . The same custom constructor is called by the copy assignment of  ```bst ```. For what concerns move semantics, we used the compiler-generated constructor and assignment.  