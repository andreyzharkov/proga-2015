#include<vector>
#include<queue>
#include<map>
#include<string>
#include<fstream>

using namespace std;

class AhoCorasick{
public:
	typedef void(*Callback) (string& str, int beg, int end);

	~AhoCorasick()
	{
		queue<BorNode*> q;
		for (map<char, BorNode*>::const_iterator iter = root.links.begin();
			iter != root.links.end(); ++iter)
			q.push(iter->second);
		while (!q.empty())
		{
			BorNode* current_node = q.front();
			q.pop();
			for (map<char, BorNode*>::const_iterator iter = current_node->links.begin();
				iter != current_node->links.end(); ++iter)
				q.push(iter->second);
			delete current_node;
		}
	}
	//добавление строки в бор
	void AddString(const string& str)
	{
		BorNode* node = &root;
		for (int i = 0; i < str.size(); ++i)
		{
			map<char, BorNode*>::iterator iter = node->links.find(str[i]);
			if (iter != node->links.end())
				node = iter->second;
			else
			{
				BorNode* new_node = new BorNode;
				node->links[str[i]] = new_node;
				node = new_node;
			}
		}
		node->out = words.size();
		words.push_back(str);
	}
	//строим из бора автомат
	void Init()
	{
		root.fail = &root;
		queue<BorNode*> q;
		q.push(&root);
		while (!q.empty())
		{
			BorNode* current_node = q.front();
			q.pop();
			for (map<char, BorNode*>::const_iterator iter = current_node->links.begin();
				iter != current_node->links.end(); ++iter)
			{
				BorNode* child = iter->second;
				char symb = iter->first;
				q.push(child);

				BorNode* parent_fail = current_node->fail;
				while (true)
				{
					map<char, BorNode*>::const_iterator it = parent_fail->links.find(symb);
					if (it != parent_fail->links.end())
					{
						child->fail = it->second != child ? it->second : &root;
						if (child->out < 0)
							child->out = child->fail->out;
						break;
					}
					if (parent_fail == &root)
					{
						child->fail = &root;
						break;
					}
					else
						parent_fail = parent_fail->fail;
				}
			}
		}
	}
	void Search(string& formatted, Callback callback)
	{
		const char* str = formatted.c_str();
		BorNode* current_node = &root;
		for (int pos = 1; *str; ++str, ++pos)
		{
			map<char, BorNode*>::const_iterator iter = current_node->links.find(*str);
			while (iter == current_node->links.end())
			{
				current_node = current_node->fail;
				iter = current_node->links.find(*str);
				if (current_node == current_node->fail)
					break;
			}
			if (iter != current_node->links.end())
			{
				current_node = iter->second;

				if (current_node->out >= 0)
					callback(formatted, pos - words[current_node->out].length(), pos - 1);
			}
		}
	}
private:
	struct BorNode
	{
		BorNode() : fail(NULL), out(-1) {}

		map<char, BorNode*> links;//можно enum чаров, если хотим уменьшить затраты памяти
		BorNode* fail;
		int out;//номер строки, которой соответствует или -1
	};

	BorNode root;
	vector<string> words;
};

class Filter{
public:
	Filter(string input, string stopwords, string output){
		is = ifstream(input);
		os = ofstream(output);
		ifstream stop(stopwords);
		string str;
		while (!stop.eof()){
			getline(stop, str);
			if (str.length() > 0) aho.AddString(str);
		}
		aho.Init();
		count = 0;
		currStrNumber = 0;
		//bool isFirst = true;
		while (!is.eof()){
			getline(is, str);
			aho.Search(str, filter);
			//if (!isFirst) os << endl;
			os << str;
			currStrNumber++;
		}
		for (int i = 0; i < count; i++){
			cout << i + 1 << ". (" << firstCoordinates[i].first << ", "
				<< firstCoordinates[i].second << ")" << endl;
		}
	}
private:
	AhoCorasick aho;
	ifstream is;
	ofstream os;
public:
	static const int countRemembered = 10;
	static int count;
	static int currStrNumber;
	static pair<int, int> firstCoordinates[countRemembered];
	static inline void filter(string& str, int beg, int end){
		for (int i = beg; i < end; i++) str[i] = '*';
		if (count < countRemembered){
			firstCoordinates[count].first = currStrNumber;
			firstCoordinates[count].second = beg;
			count++;
		}
	}
};