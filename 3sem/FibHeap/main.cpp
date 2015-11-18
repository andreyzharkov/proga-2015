#include"FibHeap.h"

int main(){
	FibHeap myHeap;
	for (int i = 0; i < 500; i++){
		myHeap.Insert(i);
		//boostHeap.emplace(i);/
	}
	for (int i = 0; i < 499; i++){
		myHeap.ExtractMin();
		//boostHeap.emplace(i);/
	}
	return 0;
}