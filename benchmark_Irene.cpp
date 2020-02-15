#include "bst.hpp"
#include <iostream>
#include <map>
#include <chrono>
#include <algorithm>   // for random_shuffle
#include <fstream>


int main(){
	std::ofstream outFile;
	outFile.open("out.txt");

	std::map<long int,char> mymap;
	//long int N {1000000};
long int P{0};
for(long int N{100}; N<20000; N+=10) {
	//inserting nodes in mymap
	for(long int i{P}; i<N; ++i) {
		mymap.insert({i,'a'});
	}
  P=N;
  //measuring time to find keys
	auto t0 = std::chrono::high_resolution_clock::now();
	for(long int k{0}; k<N; ++k){
		mymap.find(k);
	}
	auto t1 = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t1-t0);
	//std::cout << "Elapsed " << (double(elapsed.count()))/((double)N) << " [microseconds]" << std::endl;

  outFile << N << "\t" << (double(elapsed.count()))/((double)N) << "\n";
}
	outFile.close();
	return 0;
}
