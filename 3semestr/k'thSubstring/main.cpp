#include<iostream>

#include"sufautomat.h"

int main(){
	string str;
	std::getline(std::cin, str);
	SufAuto solver(str);
	
	int N, k;
	std::cin >> N;
	for (int i = 1; i <= N; i++){
		std::cin >> k;
		std::cout << solver.GetkthSubstring(k) << std::endl;
		//std::cout << "substr[" << i << "]=" << solver.GetkthSubstring(i) << std::endl;
	}
	return 0;
}