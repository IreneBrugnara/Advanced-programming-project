## Binary Search Tree

Authors: Irene Brugnara, Roberto Corti.



#### Introduction 

The aim of this project is to create a templated binary search tree. There are three classes: 

* ``` bst< const kT , vT , cmp = std::less<kT> >```

* ``` Node<T> ```  

* ```_iterator<node_t, O>  ``` 

``` bst ``` has two private members: a fun ```Node``` with a ```std::pair<const kT, vT > ``` 

  

  

  

  The two support classes ``` Node ``` and  ```_iterator ``` are outside  ``` bst``` because they have different templates. 

A ```Node```  



``` bst ``` has the following template: 

​				``` < const kT ```, ```vT ```, ```cmp = std::less<kT> >``` 

  ```kT ``` is the type of the keys that are compared with ```cmp``` 