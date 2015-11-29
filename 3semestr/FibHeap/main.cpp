#include"FibHeap.h"
#include<map>

using std::map;

#define BOOST_TEST_MAIN
#include<boost\test\included\unit_test.hpp>

void initIncrease(FibHeap<int>& bh, map<int, int>& m, int start, size_t count){
	for (int i = 0; i < count; i++){
		m[i] = start + i;
		bh.Insert(start + i, start + i);
	}
}

void initDecrease(FibHeap<int>& bh, map<int, int>& m, int start, size_t count){
	for (int i = count + start; i > start; i--){
		m[i] = i;
		bh.Insert(i, i);
	}
}

void initRandom(FibHeap<int>& bh, map<int, int>& m, size_t count){
	int rnd;
	for (int i = 0; i < count; i++){
		rnd = rand();
		if (m.find(rnd) == m.end()){
			m[rnd] = rnd;
			bh.Insert(rnd, rnd);
		}
	}
}

BOOST_AUTO_TEST_CASE(Insert)
{
	FibHeap<int> myHeap;
	map<int, int> m;

	int count = 100;
	int start = 0;
	for (int i = 0; i < count; i++){
		m[i] = start + i;
		myHeap.Insert(start + i, start + i);
		BOOST_CHECK_MESSAGE(myHeap.Min() == (*m.begin()).first, "insertion failed");
	}

	start = 500;
	for (int i = count + start; i > start; i--){
		m[i] = i;
		myHeap.Insert(i, i);
		BOOST_CHECK_MESSAGE(myHeap.Min() == (*m.begin()).first, "insertion failed");
	}

	int rnd;
	for (int i = 0; i < count; i++){
		rnd = rand();
		if (m.find(rnd) == m.end()){
			m[rnd] = rnd;
			myHeap.Insert(rnd, rnd);
			BOOST_CHECK_MESSAGE(myHeap.Min() == (*m.begin()).first, "insertion failed");
		}
	}
}

BOOST_AUTO_TEST_CASE(Merge){
	FibHeap<int> myHeap, merged;

	map<int, int> m, m_merged;
	int count = 5 + rand() % 500;
	for (int i = 0; i < count; i++){
		switch (rand() % 5){
		case 0: initDecrease(merged, m_merged, i*count, rand() % 50);
			break;
		case 1: initIncrease(merged, m_merged, i*count, rand() % 50);
			break;
		default: initRandom(merged, m_merged, rand() % 50);
		}
		myHeap.Merge(merged);
		m.insert(m_merged.begin(), m_merged.end());
		m_merged.clear();
		BOOST_CHECK_MESSAGE(myHeap.Min() == (*m.begin()).first, "merge failed");
	}
}

BOOST_AUTO_TEST_CASE(ExtractMin){
	FibHeap<int> myHeap;
	map<int, int> m;
	int count = 1 + rand() % 5000;
	initIncrease(myHeap, m, 1000, count);
	for (int i = 0; i < count - 1; i++){
		myHeap.ExtractMin();
		m.erase(m.begin());
		BOOST_CHECK_MESSAGE(myHeap.Min() == (*m.begin()).first, "extracted min failed");
	}
	myHeap.ExtractMin();
	m.erase(m.begin());

	initDecrease(myHeap, m, 1000, count);
	for (int i = 0; i < count - 1; i++){
		myHeap.ExtractMin();
		m.erase(m.begin());
		BOOST_CHECK_MESSAGE(myHeap.Min() == (*m.begin()).first, "extracted min failed");
	}
	myHeap.ExtractMin();
	m.erase(m.begin());

	initRandom(myHeap, m, count);
	for (int i = 0; i < count - 10; i++){
		myHeap.ExtractMin();
		m.erase(m.begin());
		BOOST_CHECK_MESSAGE(myHeap.Min() == (*m.begin()).first, "extracted min failed");
	}
}

BOOST_AUTO_TEST_CASE(Decrease){
	const int count = 1000;
	FibHeap<int>::NodePointer ptr[count];
	map<int, int>::iterator it[count];

	FibHeap<int> myHeap;
	map<int, int> m;

	for (int j = 0; j < 100; j++){
		for (int i = 0; i < count * 2; i++){
			if (i < count){
				m[i] = i;
				it[i] = m.find(i);
				ptr[i] = myHeap.Insert(i, i);
			}
			else{
				m[i] = i;
				myHeap.Insert(i, i);
			}
		}
		for (int i = count - 1; i >= 0; i++){
			myHeap.Decrease(ptr[i]);
			m[ptr[i].key() - 1000] = ptr[i].data();
			m.erase(it[i]);
			BOOST_CHECK_MESSAGE(myHeap.Min() == (*m.begin()).first, "decrease failed");
		}
	}
}

BOOST_AUTO_TEST_CASE(Delete){
	const int count = 1000;
	FibHeap<int>::NodePointer ptr[count];
	map<int, int>::iterator it[count];

	FibHeap<int> myHeap;
	map<int, int> m;

	for (int j = 0; j < 100; j++){
		for (int i = 0; i < count * 2; i++){
			if (i < count){
				m[i] = i;
				it[i] = m.find(i);
				ptr[i] = myHeap.Insert(i, i);
			}
			else{
				m[i] = i;
				myHeap.Insert(i, i);
			}
		}
		for (int i = count - 1; i >= 0; i++){
			myHeap.Decrease(ptr[i]);
			m.erase(it[i]);
			BOOST_CHECK_MESSAGE(myHeap.Min() == (*m.begin()).first, "deletion failed");
		}
	}
}