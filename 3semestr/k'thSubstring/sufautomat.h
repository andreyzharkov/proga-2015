#include<string>
#include<map>
#include<vector>

using std::string;
using std::vector;
using std::map;

class SufAuto{
public:
	SufAuto(const string& str){
		states.reserve(str.length() * 2);
		last = 0;
		states.push_back(state());
		states[0].len = 0;
		states[0].link = -1;

		for (int i = 0; i < str.length(); i++) sa_extend(str[i]);

		ways.resize(states.size(), 0);
		count_ways(0);
		ways[0]--;
	}
	string GetkthSubstring(int k){
		if (k > ways[0]) return "";//таких нет
		return getLexSubstrRest(0, k);
	}
	size_t GetSubstringsCount(){ return ways[0]; }
private:
	struct state {
		int len, link;
		map<char, int> next;
	};
	vector<state> states;
	vector<size_t> ways;
	int last;

	void sa_extend(char c) {
		states.push_back(state());
		int cur = states.size() - 1;
		states[cur].len = states[last].len + 1;
		
		int p;
		for (p = last; p != -1 && !states[p].next.count(c); p = states[p].link)
			states[p].next[c] = cur;
		if (p == -1)
			states[cur].link = 0;
		else {
			int q = states[p].next[c];
			if (states[p].len + 1 == states[q].len)
				states[cur].link = q;
			else
			{
				int clone = states.size();
				states.push_back(state());

				states[clone].len = states[p].len + 1;
				states[clone].next = states[q].next;
				states[clone].link = states[q].link;
				
				for (; p != -1 && states[p].next[c] == q; p = states[p].link)
					states[p].next[c] = clone;
				
				states[q].link = states[cur].link = clone;
			}
		}
		last = cur;
	}
	size_t count_ways(size_t pos){
		if (ways[pos]) return ways[pos];
		if (!states[pos].next.size()) return ways[pos] = 1;
		ways[pos] = 1;
		for (map<char, int>::iterator it = states[pos].next.begin(); it != states[pos].next.end(); it++){
			ways[pos] += count_ways(it->second);
		}
		return ways[pos];
	}
	string getLexSubstrRest(size_t pos, int k){
		if (k == 0) return "";
		for (map<char, int>::iterator it = states[pos].next.begin(); it != states[pos].next.end(); it++){
			if (ways[it->second] < k) k -= ways[it->second];
			else return it->first + getLexSubstrRest(it->second, k - 1);
		}
	}
};