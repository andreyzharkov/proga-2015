#include<vector>
#include<string>
#include<iostream>

using std::string;
using std::vector;
using std::cout;

class PeriodString{
public:
	PeriodString(const string& str){
		findPeriod(str);
	}
	void PrintNextSymbols(int count = 8){
		int pos = enteredLength;
		while (pos >= period.length()) pos -= period.length();
		if (count > period.length() - pos){
			cout << period.substr(pos);
			count -= period.length() - pos;
		}
		else{
			cout << period.substr(pos, period.length()-count) << "..." << std::endl;//
			return;
		}
		while (count >= period.length()){
			cout << period;
			count -= period.length();
		}
		cout << period.substr(0, count) << "..." << std::endl;
	}
private:
	string period;
	int enteredLength;
	void findPeriod(const string& str){
		vector<int> prefFunction;
		prefFunction.reserve(str.length());
		prefFunction.push_back(0);

		int per = 0;
		for (int i = 1; i < str.length(); i++){
			int k = prefFunction[i - 1];
			while (k > 0 && str[i] != str[k]) k = prefFunction[i - 1];
			if (str[i] == str[k]) k++;
			prefFunction.push_back(k);
			if (2 * k == i + 1 && prefFunction[i] > per) per = prefFunction[i];
		}
		enteredLength = str.length();
		period = str.substr(0, per);
	}
};

int main(){
	string str;
	std::cin >> str;
	PeriodString periodString(str);
	cout << std::endl;
	periodString.PrintNextSymbols();
	return 0;
}