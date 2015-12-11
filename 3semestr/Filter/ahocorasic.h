#include<vector>
#include<queue>
#include<map>
#include<string>
#include<fstream>
#include<iostream>
#include<memory>

using namespace std;

class AhoCorasick{
public:
	typedef void(*Callback) (string& str, int beg, int end);
	AhoCorasick() : root(new BorNode){}
	//добавление строки в бор
	void AddString(const string& str)
	{
		shared_ptr<BorNode> node(root);
		for (int i = 0; i < str.size(); ++i)
		{
			map<char, shared_ptr<BorNode>>::iterator iter = node->links.find(str[i]);
			if (iter != node->links.end())
				node = iter->second;
			else
			{
				node->links[str[i]] = shared_ptr<BorNode>(new BorNode);
				node = node->links[str[i]];
			}
		}
		node->out = words.size();
		words.push_back(str);
	}
	//строим из бора автомат
	void Init()
	{
		root->parent = root;
		queue<shared_ptr<BorNode>> q;
		q.push(shared_ptr<BorNode>(root));
		while (!q.empty())
		{
			shared_ptr<BorNode> current_node = q.front();
			q.pop();
			for (map<char, shared_ptr<BorNode>>::const_iterator iter = current_node->links.begin();
				iter != current_node->links.end(); ++iter)
			{
				shared_ptr<BorNode> child(iter->second);
				child->parent = current_node;
				q.push(child);
			}
			
			for (char c = 'a'; c != 'z'; c++){
				if (current_node->links.find(c) == current_node->links.end()){
					if (current_node == root)
						current_node->fail_links[c] = root;
					else{
						if (current_node->parent->links.find(c) != current_node->parent->links.end())
							current_node->fail_links[c] = current_node->parent->links[c];
						else
							current_node->fail_links[c] = current_node->parent->fail_links[c];
					}
				}
			}

			current_node->parent = nullptr;
		}
	}
	void Search(string& formatted, Callback callback)
	{
		const char* str = formatted.c_str();
		shared_ptr<BorNode> current_node = root;
		for (int pos = 1; *str; ++str, ++pos)
		{
			map<char, shared_ptr<BorNode>>::const_iterator iter = current_node->links.find(tolower(*str));
			if (iter == current_node->links.end())
			{
				map<char, weak_ptr<BorNode>>::const_iterator iter = current_node->fail_links.find(tolower(*str));
				if (iter == current_node->fail_links.end()){
					current_node = root;
					continue;
				}
				else{
					current_node = iter->second.lock();
					if (current_node->out >= 0){
						callback(formatted, pos - words[current_node->out].length(), pos - 1);
						current_node = root;
					}
					continue;
				}
			}
			current_node = iter->second;

			if (current_node->out >= 0){
				callback(formatted, pos - words[current_node->out].length(), pos - 1);
				current_node = root;
			}
		}
	}
private:
	struct BorNode
	{
		BorNode() : parent(), out(-1) {}
		
		map<char, shared_ptr<BorNode>> links;//можно enum чаров, если хотим уменьшить затраты памяти
		map<char, weak_ptr<BorNode>> fail_links;
		shared_ptr<BorNode> parent;//используется только при инициализации
		int out;//номер строки, которой соответствует или -1
	};

	shared_ptr<BorNode> root;
	vector<string> words;
};

const int countRemembered = 10;
int currCount;
int currStrNumber;
pair<int, int> firstCoordinates[countRemembered];

inline void bedaub(string& str, int beg, int end){
	for (int i = beg; i <= end; i++) str[i] = '*';
	if (currCount < countRemembered){
		firstCoordinates[currCount].first = currStrNumber;
		firstCoordinates[currCount].second = beg;
		currCount++;
	}
}

void filter(string input, string stopwords, string output){
	ifstream is(input);
	ofstream os(output);
	ifstream stop(stopwords);
	string str;
	AhoCorasick aho;
	while (!stop.eof()){
		getline(stop, str);
		if (str.length() > 0) aho.AddString(str);
	}
	aho.Init();
	currCount = 0;
	currStrNumber = 0;
	bool isFirst = true;
	while (!is.eof()){
		getline(is, str);
		aho.Search(str, bedaub);
		if (!isFirst) os << endl;
		os << str;
		currStrNumber++;
		isFirst = false;
	}
	for (int i = 0; i < currCount; i++){
		cout << i + 1 << ". (" << firstCoordinates[i].first << ", "
			<< firstCoordinates[i].second << ")" << endl;
	}
}