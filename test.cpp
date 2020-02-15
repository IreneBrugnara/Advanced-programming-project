#include "bst.hpp"
#include <string>

int main() {

// test insert()
  
  bst<int,char> mybst{};   // calls the implicit default constructor

  auto p = std::pair<int,char>{5,'c'};    							// calls the constructor of std::pair
  auto output = mybst.insert(p);												// l-value insert
  std::cout << "Value inserted? " << (output.second ? "Yes\n" : "No\n") ;
  mybst.insert(std::pair<const int,char>{4,'b'});				// r-value insert
  mybst.insert(std::pair<const int,char>{7,'e'});
  mybst.insert(std::pair<const int,char>{9,'g'});
  mybst.insert(std::pair<const int,char>{1,'a'});
  output = mybst.insert(std::pair<const int,char>{7,'k'});
  std::cout << "Value inserted? " << (output.second ? "Yes\n" : "No\n") ;

// test begin()
  auto mybegin = mybst.begin();
  std::cout << "first key: " << mybegin->first << "first value: " << mybegin->second << "\n";

// test put-to operator
  std::cout << "Print tree:\n" << mybst << std::endl;
  
// test find()
  std::cout << "search 1... ";
  auto r = mybst.find(1);
  std::cout << "found " << r->first << std::endl;

  std::cout << "search 47... ";
  r = mybst.find(47);
  if(r==mybst.end())
    std::cout << "value not found" << std::endl;

// test subscript operator
  auto value = mybst[7];
  std::cout<< "mybst[7] = " << value << std::endl;

  value = mybst[56];
  std::cout<< "mybst[56] = " << value << std::endl;

// test clear()
  std::cout << "before clearing: " << mybst;
  mybst.clear();
  std::cout << "\nafter clearing: ";
  std::cout << mybst << std::endl;
  std::cout << "insert 56 and 65\n";
  mybst.insert(std::pair<const int,char>{56,'b'});
  mybst.insert(std::pair<const int,char>{65,'e'});
  std::cout << "print regenerated tree:";
  std::cout << mybst << std::endl;

// test emplace()
  mybst.emplace(8, 'a');
  mybst.emplace(6, 'a');
  mybst.emplace(3, 'a');
  mybst.emplace(2, 'a');
  mybst.emplace(10, 'a');
  mybst.emplace(10, 'a');

// test erase()
  std::cout << "before erasing: " << mybst;
  std::cout << "\nerase 3 and 9\n";
  mybst.erase(3);
  mybst.erase(9);
  std::cout << "after erasing: ";
  std::cout << mybst;

// test copy ctor
  std::cout << "\ncall copy ctor: new bst is ";
  bst<int, char> mylist {mybst}; 
  std::cout << mylist << "\n";
  mybst.erase(6);
  std::cout << "erased 6 from mybst - check if really deep copy: \n";
  std::cout << "mybst: " << mybst << "\n";       
  std::cout << "mylist: " << mylist << "\n";

  std::cout << "call move ctor from mylist to mydata\n";
  bst<int, char> mydata {std::move(mylist)};
  std::cout << "mylist: " << mylist << "\n";
  std::cout << "mydata: " << mydata << "\n"; 

  std::cout << "copy assign mydata to mylist\n";
  mylist = mydata;
  std::cout << "mylist: " << mylist << "\n";
  std::cout << "mydata: " << mydata << "\n";
  std::cout << "erased 6 from mydata - check if really deep copy: \n";
  mydata.erase(6);
  std::cout << "mylist: " << mylist << "\n";
  std::cout << "mydata: " << mydata << "\n";

  std::cout << "move assign mydata to mylist\n";
  mylist = std::move(mydata);
  std::cout << "mylist: " << mylist << "\n";
  std::cout << "mydata: " << mydata << "\n";

// test balance
  mybst.balance();
  std::cout << "mybst has been balanced \n" << mybst << std::endl;
  // note: we have checked with private member function print_tree() that the balancing occurs properly

  return 0;
}
