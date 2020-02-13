#include "bst.hpp"
#include <chrono>			// to measure time
#include <map>
#include <cstdlib>		// for random number generation
#include <unistd.h>
#include <algorithm>   // for random_shuffle
#include <fstream>

int main() {
  bst<int, char> mybst;
  std::map<int, char> mymap;

  std::srand(static_cast<unsigned int>(std::time(nullptr))); // set initial seed value to system clock
/*
  int N{1000000};				// number of repetitions
  int min=1;
  int max=50000;
  auto t0 = std::chrono::high_resolution_clock::now();
  for(int i{}; i<N; ++i) {
    mybst.insert({rand()%RAND_MAX*(max-min)+min, 'a'});
  }
  auto t1 = std::chrono::high_resolution_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0);
  std::cout << "Elapsed " << elapsed.count() << " [milliseconds]" << std::endl;

  auto t0_map = std::chrono::high_resolution_clock::now();
  for(int i{}; i<N; ++i) {
    mymap.insert({rand()%RAND_MAX*(max-min)+min, 'a'});
  }
  auto t1_map = std::chrono::high_resolution_clock::now();
  auto elapsed_map = std::chrono::duration_cast<std::chrono::milliseconds>(t1_map-t0_map);
  std::cout << "Elapsed " << elapsed_map.count() << " [milliseconds]" << std::endl;

  auto t0_prova = std::chrono::high_resolution_clock::now();
  usleep(50000);
  auto t1_prova = std::chrono::high_resolution_clock::now();
  auto elapsed_prova = std::chrono::duration_cast<std::chrono::milliseconds>(t1_prova-t0_prova);
  std::cout << "Elapsed " << elapsed_prova.count() << " [milliseconds]" << std::endl;
*/
	std::ofstream outFile;
	outFile.open("out.txt");
  constexpr long int N{3};
  std::cout << N;

  std::vector<long int> numbers;
	outFile << "# N \t T \n"; 
  for(long int i{0}; i<N; ++i)
    numbers.push_back(i);
  std::random_shuffle(numbers.begin(), numbers.end());

  for(int i{0}; i<N; ++i) {
    mybst.insert({numbers[i], 'a'});
    auto t0 = std::chrono::high_resolution_clock::now();
    for(int j{0}; j<i; ++j) {
      mybst.find(numbers[j]);
    }
		usleep(i*100000);
    auto t1 = std::chrono::high_resolution_clock::now();
		auto elapsed_prova = std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0);
  	outFile << i << "\t" << elapsed_prova.count(); //[milliseconds]

    auto t0_map = std::chrono::high_resolution_clock::now();
    for(int j{0}; j<i; ++j) {
      mybst.find(numbers[j]);
    }
		usleep(i*200000);
    auto t1_map = std::chrono::high_resolution_clock::now();
		auto elapsed_prova_map = std::chrono::duration_cast<std::chrono::milliseconds>(t1_map-t0_map);
  	outFile << "\t" << elapsed_prova_map.count() << "\n"; //[milliseconds]

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
*/
	outFile.close();

  return 0;
}
