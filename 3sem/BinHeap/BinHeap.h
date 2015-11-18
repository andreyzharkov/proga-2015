#include<iostream>
#include<list>
#include<algorithm>

using namespace std;

struct Node{
	Node(){ child = sibling = parent = nullptr; }
	~Node(){
		if (child) delete child;
		if (sibling) delete sibling;
	}
	Node* child;//left son
	Node* sibling;//next brother
	Node* parent;
	int key;
	size_t degree;//начина€ с 1
};

struct pred
	: public binary_function<Node*, Node*, bool>
	{	// functor for operator<
		bool operator()(const Node* _Left, const Node* _Right) const
		{	// apply operator< to operands
			if (_Left == nullptr) return false;
			if (_Right == nullptr) return true;
			return _Left->degree < _Right->degree;
		}
};

bool myCompare(const Node* _Left, const Node* _Right){
	if (_Left == nullptr) return false;
	if (_Right == nullptr) return true;
	return _Left->degree < _Right->degree;
}

class BinHeap{
private:
	list<Node*> rootList;
	static const int decrease = 1000;
public:
	BinHeap(){ rootList.push_back(nullptr); }
	BinHeap(BinHeap& first){
		rootList = first.rootList;
		first.rootList.clear();
		first.rootList.push_back(nullptr);
	}
	BinHeap(BinHeap& first, BinHeap& second){
		auto it = first.rootList.begin();
		auto jt = second.rootList.begin();

		if (*it == nullptr){
			*this = BinHeap(second);
			return;
		}
		if (*jt == nullptr){
			*this = BinHeap(first);
			return;
		}

		while (*it != nullptr && *jt != nullptr){
			if ((*it)->degree < (*jt)->degree){
				rootList.push_back(*it);
				it++;
			}
			else if ((*it)->degree >(*jt)->degree){
				rootList.push_back(*jt);
				jt++;
			}
			else{
				if ((*it)->key > (*jt)->key){
					Node* root = *jt;
					Node* child = *it;
					child->parent = root;
					child->sibling = root->child;
					root->child = child;
					root->degree++;
					rootList.push_back(root);
				}
				else{
					Node* root = *it;
					Node* child = *jt;
					child->parent = root;
					child->sibling = root->child;
					root->child = child;
					root->degree++;
					rootList.push_back(root);
				}
				it++; jt++;
			}
		}

		while (*jt != nullptr){
			if (rootList.back()->degree == (*jt)->degree){
				if (rootList.back()->key > (*jt)->key){
					Node* root = *jt;
					Node* child = rootList.back();
					rootList.back() = nullptr;
					child->parent = root;
					child->sibling = root->child;
					root->child = child;
					root->degree++;
					rootList.back() = root;
				}
				else{
					Node* root = rootList.back();
					Node* child = *jt;
					child->parent = root;
					child->sibling = root->child;
					root->child = child;
					root->degree++;
				}
			}
			else{
				rootList.push_back(*jt);
			}
			jt++;
		}
		while (*it != nullptr){
			if (rootList.back()->degree == (*it)->degree){
				if (rootList.back()->key > (*it)->key){
					Node* root = *it;
					Node* child = rootList.back();
					rootList.back() = nullptr;
					child->parent = root;
					child->sibling = root->child;
					root->child = child;
					root->degree++;
					rootList.back() = root;
				}
				else{
					Node* root = rootList.back();
					Node* child = *it;
					child->parent = root;
					child->sibling = root->child;
					root->child = child;
					root->degree++;
				}
			}
			else{
				rootList.push_back(*it);
			}
			it++;
		}

		rootList.push_back(nullptr);
		first.rootList.clear();
		second.rootList.clear();
		first.rootList.push_back(nullptr);
		second.rootList.push_back(nullptr);
	}
	BinHeap& operator=(BinHeap& rhs){
		rootList = rhs.rootList;
		rhs.rootList.clear();
		rhs.rootList.push_back(nullptr);
		return *this;
	}
	void Merge(BinHeap& rhs){
		if (rootList.front() == nullptr){
			*this = rhs;
			return;
		}
		auto it = rootList.begin();
		auto jt = rhs.rootList.begin();
		list<Node*> merged;
		while (*it != nullptr && *jt != nullptr){
			if (merged.empty() || merged.back()->degree < min((*it)->degree, (*jt)->degree)){
				if ((*it)->degree < (*jt)->degree){
					it++;
				}
				else{
					if ((*it)->degree > (*jt)->degree){
						merged.push_back(*jt);
						jt++;
					}
					else{
						merged.push_back(mergeNodes(*it, *jt));
						jt++;
						auto iter = it;
						it++;
						rootList.erase(iter);
					}
				}
			}
			else{
				if ((*it)->degree > (*jt)->degree){
					//merged+rhs-->merged
					Node* swp = merged.back();
					merged.back() = mergeNodes(*jt, swp);
					jt++;
				}
				else{
					if ((*it)->degree < (*jt)->degree){
						//merged+this-->merged
						Node* swp = merged.back();
						merged.back() = mergeNodes(*it, swp);
						auto iter = it;
						it++;
						rootList.erase(iter);
					}
					else{
						//merged+rhs-->merged
						Node* swp = merged.back();
						merged.back() = mergeNodes(*jt, swp);
						jt++; it++;
					}
				}
			}
			
			if (*it != nullptr){
				while (!merged.empty() && (*it != nullptr) && (merged.back()->degree == (*it)->degree)){
					Node* swp = merged.back();
					merged.back() = mergeNodes(*it, swp);
					auto iter = it;
					it++;
					rootList.erase(iter);
				}
			}
			if (*jt != nullptr){
				while (!merged.empty() && (*jt != nullptr) && (merged.back()->degree == (*jt)->degree)){
					Node* swp = merged.back();
					merged.back() = mergeNodes(*jt, swp);
					jt++;
				}
				while (*jt != nullptr){
					merged.push_back(*jt);
					jt++;
				}
			}
		}
		rhs.rootList.clear();
		rootList.merge(merged, myCompare);
	}
	void Insert(int key){
		Node* inserted = new Node();
		inserted->key = key;
		inserted->degree = 1;
		BinHeap ins;
		ins.rootList.push_front(inserted);
		Merge(ins);
	}
	void Decrease(Node* node, int decr = decrease){
		node->key -= decr;
		Node* ptr = node;
		if (!ptr->parent){
			return;
		}
		while (ptr->parent && ptr->parent->key > ptr->key){
			swap(ptr->parent->key, ptr->key);
			ptr = ptr->parent;
		}
	}
	void Delete(Node* node){
		int decr = node->key - Min() + 1;
		Decrease(node, decr);
		ExtractMin();
	}
	int Min(){
		//не надо вызывать этот метод дл€ пустой кучи!
		int m = rootList.front()->key;
		for (auto it = rootList.begin(); *it != nullptr; it++){
			m = min(m, (*it)->key);
		}
		return m;
	}
	void ExtractMin(){
		int m = Min();
		for (auto it = rootList.begin(); it != rootList.end(); it++){
			if ((*it)->key == m){
				BinHeap bh(*it);
				*it = nullptr;
				rootList.erase(it);
				Merge(bh);
				return;
			}
		}
	}
private:
	BinHeap(Node* removedRoot){
		Node* ptr = removedRoot->child;
		removedRoot->child = nullptr;
		//delete removedRoot;
		while (ptr){
			ptr->parent = nullptr;
			rootList.push_front(ptr);
			ptr = ptr->sibling;
			rootList.front()->sibling = nullptr;
		}
		rootList.push_back(nullptr);
	}
	static Node* mergeNodes(Node* fst, Node* snd){
		Node* root;
		Node* child;
		if (fst->key > snd->key){
			root = snd;
			child = fst;
		}
		else{
			root = fst;
			child = snd;
		}
		child->parent = root;
		child->sibling = root->child;
		root->child = child;
		root->degree++;
		return root;
	}
};