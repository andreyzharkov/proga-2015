#include<boost\heap\binomial_heap.hpp>
#include"BinHeap.h"

#define BOOST_TEST_MAIN
#include<boost\test\unit_test.hpp>

BOOST_AUTO_TEST_CASE(Insert)
{
	BinHeap<int> myHeap;
	boost::heap::binomial_heap<int> boostHeap;

	for (int i = 0; i < 50; i++){
		myHeap.Insert(i, i);
		boostHeap.emplace(i, i);
		BOOST_CHECK_MESSAGE(myHeap.Min() == boostHeap.top(), "insertion failed");
	}
	for (int i = 0; i < 100000; i++){
		myHeap.Insert(50 + rand() % 67890, -i);
		boostHeap.emplace(i, i);
		BOOST_CHECK_MESSAGE(myHeap.Min() == boostHeap.top(), "insertion failed");
	}
}

BOOST_AUTO_TEST_CASE(ExtractMin){
	BinHeap<int> myHeap;
	boost::heap::binomial_heap<int> boostHeap;

	for (int i = 0; i < 50; i++){
		myHeap.Insert(i, i);
		boostHeap.emplace(i, i);
		BOOST_CHECK_MESSAGE(myHeap.Min() == boostHeap.top(), "insertion failed");
	}
	for (int i = 0; i < 100000; i++){
		myHeap.Insert(50 + rand() % 67890, -i);
		boostHeap.emplace(i, i);
		BOOST_CHECK_MESSAGE(myHeap.Min() == boostHeap.top(), "insertion failed");
	}
	for (int i = 0; i < 100050; i++){
		myHeap.ExtractMin();
		boostHeap.pop();
		BOOST_CHECK_MESSAGE(myHeap.Min() == boostHeap.top(), "extraction min failed");
	}
}

BOOST_AUTO_TEST_CASE(Merge){
	BinHeap<int> myHeap;
	boost::heap::binomial_heap<int> boostHeap;

	for (int i = 0; i < 50; i++){
		myHeap.Insert(i, i);
		boostHeap.emplace(i, i);
		BOOST_CHECK_MESSAGE(myHeap.Min() == boostHeap.top(), "insertion failed");
	}
	for (int i = 0; i < 50; i++){
		int mergedHeapSize = 0;
		BinHeap<int> mergedHeap;
		for (int j = 0; j < mergedHeapSize; j++){
			int f = rand(), s = rand();
			mergedHeap.Insert(f, s);
			boostHeap.emplace(f, s);
			BOOST_CHECK_MESSAGE(mergedHeap.Min() == boostHeap.top(), "insertion failed");
		}
		if (i % 2){
			mergedHeap.ExtractMin();
			mergedHeapSize++;
		}
		myHeap.Merge(mergedHeap);
		BOOST_CHECK_MESSAGE(myHeap.Min() == boostHeap.top(), "merge failed");
	}
}

BOOST_AUTO_TEST_CASE(Decrease){
	boost::heap::binomial_heap<int> boostHeap;

	boost::heap::binomial_heap<int>::handle_type boostPtr[10000];
	BinHeap<int>::NodePointer ptr[10000];
	
	BinHeap<int> myHeap;
	for (int i = 0; i < 1000; i++){
		ptr[i] = myHeap.Insert(i, i);
		boostPtr[i] = boostHeap.emplace(i, i);
		BOOST_CHECK_MESSAGE(myHeap.Min() == boostHeap.top(), "insertion failed");
	}
	for (int i = 1000; i < 10000; i++){
		int rnd = rand() % 67890;
		ptr[i] = myHeap.Insert(1000 + rnd, -i);
		boostPtr[i] = boostHeap.emplace(1000 + rnd, -i);
		BOOST_CHECK_MESSAGE(myHeap.Min() == boostHeap.top(), "insertion failed");
	}
	for (int i = 0; i < 9999; i++){
		int j = rand() % 10000;
		myHeap.Decrease(ptr[j], 1000);
		boostHeap.decrease(boostPtr[j], 1000);
		BOOST_CHECK_MESSAGE(myHeap.Min() == boostHeap.top(), "decreasing node failed");
	}
}

BOOST_AUTO_TEST_CASE(Delete){
	boost::heap::binomial_heap<int> boostHeap;

	boost::heap::binomial_heap<int>::handle_type boostPtr[10000];
	BinHeap<int>::NodePointer ptr[10000];

	BinHeap<int> myHeap;
	for (int i = 0; i < 1000; i++){
		ptr[i] = myHeap.Insert(i, i);
		boostPtr[i] = boostHeap.emplace(i, i);
		BOOST_CHECK_MESSAGE(myHeap.Min() == boostHeap.top(), "insertion failed");
	}
	for (int i = 1000; i < 10000; i++){
		int rnd = rand() % 67890;
		ptr[i] = myHeap.Insert(1000 + rnd, -i);
		boostPtr[i] = boostHeap.emplace(1000 + rnd, -i);
		BOOST_CHECK_MESSAGE(myHeap.Min() == boostHeap.top(), "insertion failed");
	}
	for (int i = 0; i < 9; i++){
		myHeap.Delete(ptr[i]);
		boostHeap.erase(boostPtr[i]);
		BOOST_CHECK_MESSAGE(myHeap.Min() == boostHeap.top(), "delete operation node failed");
	}
	for (int i = 100; i < 900; i++){
		myHeap.Delete(ptr[i]);
		boostHeap.erase(boostPtr[i]);
		BOOST_CHECK_MESSAGE(myHeap.Min() == boostHeap.top(), "delete operation node failed");
	}
	for (int i = 6000; i < 9999; i++){
		myHeap.Delete(ptr[i]);
		boostHeap.erase(boostPtr[i]);
		BOOST_CHECK_MESSAGE(myHeap.Min() == boostHeap.top(), "delete operation node failed");
	}
	int i = 10;
	myHeap.Delete(ptr[i]);
	boostHeap.erase(boostPtr[i]);
	BOOST_CHECK_MESSAGE(myHeap.Min() == boostHeap.top(), "delete operation node failed");
}