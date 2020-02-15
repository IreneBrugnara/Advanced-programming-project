#include "bst.hpp"
#include <iostream>
#include <map>
#include <chrono>			 // to measure time
#include <algorithm>   // for random_shuffle
#include <fstream>		 // to write data on file


int main(){

// open file where to put data
	std::ofstream outFile;
	outFile.open("data_benchmark.txt");

// create vector of unique random numbers from 0 to N_max-1
	int N_max {10000};
	std::vector<int> numbers;
  for(int i{0}; i<N_max; ++i)
    numbers.push_back(i);
  std::random_shuffle(numbers.begin(), numbers.end());

// initialize trees
	bst<int,char> mybst;				// bst
	bst<int,char> mybalbst;			// balanced bst
  std::map<int,char> mymap;		// std::map

// start benchmark
  int P{0};
  for(int N{100}; N<N_max; N+=10) {
  	//insert nodes in the trees
  	for(int i{P}; i<N; ++i) {
	  	mymap.insert({numbers[i],'a'});
      mybst.insert({numbers[i],'a'});
      mybalbst.insert({numbers[i],'a'});
	  }
    P=N;			// don't insert the already inserted nodes
    
    //measuring time to find keys with std::map
  	auto t0_map = std::chrono::high_resolution_clock::now();
   	for(int k{0}; k<N; ++k){
 		  mymap.find(numbers[k]);
  	}
	  auto t1_map = std::chrono::high_resolution_clock::now();
	  auto elapsed_map = std::chrono::duration_cast<std::chrono::microseconds>(t1_map-t0_map);


    //measuring time to find keys with bst
  	auto t0_bst = std::chrono::high_resolution_clock::now();
   	for(int k{0}; k<N; ++k){
 		  mybst.find(numbers[k]);
  	}
	  auto t1_bst = std::chrono::high_resolution_clock::now();
	  auto elapsed_bst = std::chrono::duration_cast<std::chrono::microseconds>(t1_bst-t0_bst);

    // balance the tree
    mybalbst.balance();
    //measuring time to find keys with balanced bst
  	auto t0_bst_bal = std::chrono::high_resolution_clock::now();
   	for(int k{0}; k<N; ++k){
 		  mybalbst.find(numbers[k]);
  	}
	  auto t1_bst_bal = std::chrono::high_resolution_clock::now();
	  auto elapsed_bst_bal = std::chrono::duration_cast<std::chrono::microseconds>(t1_bst_bal-t0_bst_bal);

    // write results to file
    outFile << N << "\t" << (double(elapsed_map.count()))/((double)N) << "\t" << (double(elapsed_bst.count()))/((double)N) << "\t" << (double(elapsed_bst_bal.count()))/((double)N) << "\n";
  }
	outFile.close();
	return 0;
}
