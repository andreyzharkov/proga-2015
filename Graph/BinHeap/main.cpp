//#include<boost\heap\binomial_heap.hpp>
#include"BinHeap.h"

int main(){
	//boost::heap::binomial_heap<int> boostHeap;
	BinHeap myHeap;
	
	for (int i = 0; i < 1000; i++){
		myHeap.Insert(i);
		//boostHeap.emplace(i);/
	}

	return 0;
}