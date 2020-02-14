#include "bst.hpp"
#include <chrono>			// to measure time
#include <map>
#include <cstdlib>		// for random number generation
#include <unistd.h>
#include <algorithm>   // for random_shuffle
#include <fstream>

int main() {
  
  std::srand(static_cast<unsigned int>(std::time(nullptr))); // set initial seed value to system clock


  std::map<int,int> mybst;
  int N{1000000};				// number of nodes
  std::cout << N << "\n";
  auto t0 = std::chrono::high_resolution_clock::now();
  for(int i{0}; i<N; ++i) {
    mybst.insert({i, i});
  }
	auto t1 = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t1-t0);
  std::cout << "Elapsed " << elapsed.count()/double(N)  << " [microseconds]" << std::endl;


  auto t0_prova = std::chrono::high_resolution_clock::now();
	auto it = mybst.find(N-1);
  std::cout << "searched " << N-1 << "found " <<  it->first << "\n";
  //usleep(N);

	/*
  for(int i{0}; i<N; ++i) {
		mymap.find(i);
	}*/
	auto t1_prova = std::chrono::high_resolution_clock::now();
	auto elapsed_prova = std::chrono::duration_cast<std::chrono::microseconds>(t1_prova-t0_prova);
  std::cout << "Elapsed " << elapsed_prova.count()  << " [microseconds]" << std::endl;


/*
	std::ofstream outFile;
	outFile.open("out.txt");
  constexpr int N{50000};			// maximum number of nodes in tree
  constexpr int D{1000};							// dimension of block (how many nodes to insert before starting to find them)
  constexpr int B{N/D};							// total number of blocks

// create a vector of unique random numbers
  std::vector<int> numbers;
	outFile << "# N \t T \n";
  for(int i{0}; i<N; ++i)
    numbers.push_back(i);
  std::random_shuffle(numbers.begin(), numbers.end());

  for(auto k: numbers)
    std::cout << k << "   ";

// progressively add numbers in the bst to increase the number of nodes contained in the bst
  for(int i{0}; i<B; ++i) {		
						// loop on blocks
    std::cout << "i=" << i << "\n";
    // insert all elements of block i
    std::cout << "START INSERTING\n";
    for(int j{i*D}; j<(i+1)*D; ++j) {
      std::cout << "inserting " << numbers[j] << "\n";
      mymap.insert({numbers[j], 'a'});
    }
    // measure how long it takes to find a key on average in a tree with i*D nodes
    auto t0 = std::chrono::high_resolution_clock::now();
    for(int j{0}; j<(i+1)*D; ++j) {
      std::cout << "finding " << numbers[j] << "\n";
      mymap.find(numbers[j]);
    }
    auto t1 = std::chrono::high_resolution_clock::now();
		auto elapsed_prova = std::chrono::duration_cast<std::chrono::microseconds>(t1-t0);
  	outFile << (i+1)*D << "\t" << elapsed_prova.count()/double((i+1)*D) << "\n"; //[milliseconds]			// divide by the number of nodes we currently have in the bst
  }



/*
    auto t0_map = std::chrono::high_resolution_clock::now();
    for(int j{0}; j<i; ++j) {
      mybst.find(numbers[j]);
    }
    auto t1_map = std::chrono::high_resolution_clock::now();
		auto elapsed_prova_map = std::chrono::duration_cast<std::chrono::milliseconds>(t1_map-t0_map);
  	outFile << "\t" << elapsed_prova_map.count()/double(i) << "\n"; //[milliseconds]

  }
/*
  auto t0 = std::chrono::high_resolution_clock::now();
  for(long int i{}; i<N; ++i) {
    mybst.insert({numbers[i], 'a'});
  }
  auto t1 = std::chrono::high_resolution_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0);
  std::cout << "Elapsed " << elapsed.count() << " [milliseconds]" << std::endl;

  auto t0_map = std::chrono::high_resolution_clock::now();
  for(long int  i{}; i<N; ++i) {
    mymap.insert({numbers[i], 'a'});
  }
  auto t1_map = std::chrono::high_resolution_clock::now();
  auto elapsed_map = std::chrono::duration_cast<std::chrono::milliseconds>(t1_map-t0_map);
  std::cout << "Elapsed " << elapsed_map.count() << " [milliseconds]" << std::endl;

	outFile.close();
*/
  return 0;
}
