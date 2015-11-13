#include<iostream>
#include<list>
#include<algorithm>

using namespace std;

struct Node{
	Node(){ child = sibling = parent = nullptr; }
	Node(const Node* rhs){
		key = rhs->key;
		parent = child = sibling = nullptr;
		parent = rhs->parent;
		if (rhs->child) child = new Node(rhs->child);
		if (rhs->sibling) sibling = new Node(rhs->sibling);
		degree = rhs->degree;
	}
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

class BinHeap{
private:
	list<Node*> rootList;
	static const int decrease = 1000;
public:
	BinHeap(){ rootList.push_back(nullptr); }
	BinHeap(const BinHeap& first){
		for (auto it = first.rootList.begin(); *it != nullptr; it++){
			rootList.push_back(new Node(*it));
		}
		rootList.push_back(nullptr);
	}
	BinHeap(const BinHeap& first, const BinHeap& second){
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
				rootList.push_back(new Node(*it));
				it++;
			}
			else if ((*it)->degree >(*jt)->degree){
				rootList.push_back(new Node(*jt));
				jt++;
			}
			else{
				if ((*it)->key > (*jt)->key){
					Node* root = new Node(*jt);
					Node* child = new Node(*it);
					child->parent = root;
					child->sibling = root->child;
					root->child = child;
					root->degree++;
					rootList.push_back(root);
				}
				else{
					Node* root = new Node(*it);
					Node* child = new Node(*jt);
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
					Node* root = new Node(*jt);
					Node* child = rootList.back();
					child->parent = root;
					child->sibling = root->child;
					root->child = child;
					swap(root, child);
					root->degree++;
				}
				else{
					Node* root = rootList.back();
					Node* child = new Node(*jt);
					child->parent = root;
					child->sibling = root->child;
					root->child = child;
					root->degree++;
				}
			}
			else{
				rootList.push_back(new Node(*jt));
			}
			jt++;
		}
		while (*it != nullptr){
			if (rootList.back()->degree == (*it)->degree){
				if (rootList.back()->key > (*it)->key){
					Node* root = new Node(*it);
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
					Node* child = new Node(*it);
					child->parent = root;
					child->sibling = root->child;
					root->child = child;
					root->degree++;
				}
			}
			else{
				rootList.push_back(new Node(*it));
			}
			it++;
		}

		rootList.push_back(nullptr);
	}
	BinHeap& operator=(BinHeap& rhs){
		for (auto it = rootList.begin(); it!=rootList.end(); it++){
			delete (*it);
		}
		rootList.clear();
		for (auto it = rhs.rootList.begin(); *it != nullptr; it++){
			rootList.push_back(new Node(*it));
		}
		rootList.push_back(nullptr);
		return *this;
	}
	void Merge(BinHeap& rhs){
		*this = BinHeap(*this, rhs);
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
				Merge(BinHeap(*it));
				return;
			}
		}
	}
private:
	BinHeap(Node* removedRoot){
		Node* ptr = removedRoot->child;
		removedRoot->child = nullptr;
		delete removedRoot;
		while (ptr){
			ptr->parent = ptr->sibling = nullptr;
			rootList.push_front(ptr);
			ptr = ptr->sibling;
		}
		rootList.push_back(nullptr);
	}
};