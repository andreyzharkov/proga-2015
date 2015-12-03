#include<vector>
#include<map>
#include<set>
#include<iostream>

#define FIRST_LETTER 'a'
#define LETTERS_COUNT 26

using namespace std;

class Solver{
public:
	Solver(){
		ChangeSourseWord("");
		wasSolutionFound = wasSolutionSearched = true;
	}
	Solver(string query){
		ChangeSourseWord(query);
		wasSolutionFound = wasSolutionSearched = false;
	}
	void AddKube(string str){
		vector<char> edges;
		set<char> repeatedLetters;
		for (int i = 0; i < str.length(); i++){
			if (repeatedLetters.find(str[i]) == repeatedLetters.end()){
				edges.push_back(str[i]);
				letToKub[str[i]].push_back(kubToLet.size());
				repeatedLetters.insert(str[i]);
			}
		}
		kubToLet.push_back(edges);
		if (!wasSolutionFound) wasSolutionSearched = false;
	}
	void ChangeSourseWord(string str){
		query = str;
		letToSink.clear();
		for (int i = 0; i < str.length(); i++){
			if (letToSink.find(str[i]) != letToSink.end())
				letToSink[str[i]]++;
			else letToSink[str[i]] = 1;
		}
		wasSolutionFound = false;
		wasSolutionSearched = false;
	}
	void FindSolution(){
		if (wasSolutionSearched) return;
		//конечно лучше было б bfs, но это потребует много лишней памяти
		usedKubes.resize(kubToLet.size(), false);
		for (auto it = letToKub.begin(); it != letToKub.end(); it++){
			usedLetters[(*it).first] = false;
		}
		kubToLetAccepted.resize(kubToLet.size(), 0);

		bool flag;
		for (auto it = letToSink.begin(); it != letToSink.end(); it++){
			while ((*it).second){
				flag = false;
				//если есть свободный кубик с этой буквой не заморачиваемся с dfs
				for (auto jt = letToKub[(*it).first].begin(); jt != letToKub[(*it).first].end(); jt++){
					if (!kubToLetAccepted[*jt]){
						(*it).second--;
						kubToLetAccepted[*jt] = (*it).first;
						flag = true;
						break;
					}
				}
				if (!flag){
					for (auto jt = letToKub[(*it).first].begin(); jt != letToKub[(*it).first].end(); jt++){
						usedLetters[(*it).first] = true;
						if (dfs(kubToLetAccepted[*jt], *jt)){
							(*it).second--;
							kubToLetAccepted[*jt] = (*it).first;
							flag = true;
							usedLetters[(*it).first] = false;
							break;
						}
						usedLetters[(*it).first] = false;
					}
				}
				if (!flag){
					wasSolutionSearched = true;
					wasSolutionFound = false;
					return;
				}
			}
		}
		wasSolutionSearched = true;
		wasSolutionFound = true;
	}
	void PrintSolution(){
		if (!wasSolutionSearched) FindSolution();
		if (wasSolutionFound){
			cout << "YES" << endl;
			set<int> usedCubes;
			for (int i = 0; i < query.length(); i++){
				for (int j = 0; j < kubToLetAccepted.size(); j++){
					if (kubToLetAccepted[j] == query[i] && usedCubes.find(j) == usedCubes.end()){
						usedCubes.insert(j);
						cout << j;
						break;
					}
				}
			}
			cout << endl;
		}
		else{
			cout << "NO" << endl;
		}
	}
private:
	string query;
	//это всё рёбра
	vector<vector<char>> kubToLet;//рёбра из кубиков в буквы
	map<char, vector<int>> letToKub;//рёбра из букв в кубики
	map<char, int> letToSink;//сколько осталось

	vector<char> kubToLetAccepted;//то что мы засчитали в макс поток

	bool wasSolutionFound;//чтобы не искать по несколько раз
	bool wasSolutionSearched;

	map<char, bool> usedLetters;//просмотренные вершины
	vector<bool> usedKubes;
	//если dfs возвращает true он сразу исправляет рёбра
	bool dfs(char letter, int prevKub){
		usedLetters[letter] = true;
		usedKubes[prevKub] = true;
		for (auto it = letToKub[letter].begin(); it != letToKub[letter].end(); it++){
			int nextKub = *it;
			if (usedKubes[nextKub]) continue;
			if (kubToLetAccepted[nextKub]){
				if (!usedLetters[kubToLetAccepted[nextKub]])
					if (dfs(kubToLetAccepted[nextKub], nextKub)){
						usedLetters[letter] = false;
						usedKubes[prevKub] = false;
						kubToLetAccepted[nextKub] = letter;
						return true;
					}
			}
			else{
				usedLetters[letter] = false;
				usedKubes[prevKub] = false;
				kubToLetAccepted[nextKub] = letter;
				return true;
			}
		}
		usedLetters[letter] = false;
		usedKubes[prevKub] = false;
		return false;
	}
};

int main(){
	Solver solver;
	solver.AddKube("ABCDEFGHIJ");
	solver.AddKube("ABCDEFGHI");
	solver.AddKube("ABCDEFGH");
	solver.AddKube("ABCDEFG");
	solver.AddKube("ABCDEF");
	solver.AddKube("ABCDE");
	solver.AddKube("ABCD");
	solver.AddKube("ABC");
	solver.AddKube("AB");
	solver.AddKube("A");
	solver.ChangeSourseWord("ABCDEFGHIJKK");
	solver.FindSolution();
	solver.PrintSolution();
	return 0;
}