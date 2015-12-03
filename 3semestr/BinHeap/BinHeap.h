#include<iostream>
#include<list>
#include<algorithm>

using namespace std;

template<typename T>
struct Node{
	Node(int key, T data){	//исправлено
		child = sibling = parent = nullptr;
		this->key = key;
		this->data = data;
		degree = 1;
	}
	~Node(){
		delete child;
		delete sibling;
	}
	Node<T>* child;//left son
	Node<T>* sibling;//next brother
	Node<T>* parent;
	int key;
	T data;
	size_t degree;//начиная с 1
};

template<typename T>
bool myCompare(const Node<T>* _Left, const Node<T>* _Right){
	if (_Left == nullptr) return false;
	if (_Right == nullptr) return true;
	return _Left->degree < _Right->degree;
}

template<typename T>
class BinHeap{
private:
	list<Node<T>*> rootList;
	static const int decrease = 1000;	//это значение по умолчанию для Decrease
public:
	class NodePointer{
		friend class BinHeap<T>;
		Node<T>* ptr;
	public:
		int key() const{ return ptr->key; }
		const T& data() const{ return ptr->data; }
	};
	BinHeap(){ rootList.push_back(nullptr); }	//мне так merge проще писать было.. можно и без этого, конечно, но как-то времени особо нет переделывать
	BinHeap(BinHeap& first){
		rootList = first.rootList;
		first.rootList.clear();
		first.rootList.push_back(nullptr);
	}
	//вообще говоря лишний конструктор
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
					Node<T>* root = *jt;
					Node<T>* child = *it;
					child->parent = root;
					child->sibling = root->child;
					root->child = child;
					root->degree++;
					rootList.push_back(root);
				}
				else{
					Node<T>* root = *it;
					Node<T>* child = *jt;
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
					Node<T>* root = *jt;
					Node<T>* child = rootList.back();
					rootList.back() = nullptr;
					child->parent = root;
					child->sibling = root->child;
					root->child = child;
					root->degree++;
					rootList.back() = root;
				}
				else{
					Node<T>* root = rootList.back();
					Node<T>* child = *jt;
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
					Node<T>* root = *it;
					Node<T>* child = rootList.back();
					rootList.back() = nullptr;
					child->parent = root;
					child->sibling = root->child;
					root->child = child;
					root->degree++;
					rootList.back() = root;
				}
				else{
					Node<T>* root = rootList.back();
					Node<T>* child = *it;
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
	~BinHeap(){
		for (auto it = rootList.begin(); *it; it++){
			delete *it;
		}
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
		list<Node<T>*> merged;
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
					Node<T>* swp = merged.back();
					merged.back() = mergeNodes(*jt, swp);
					jt++;
				}
				else{
					if ((*it)->degree < (*jt)->degree){
						//merged+this-->merged
						Node<T>* swp = merged.back();
						merged.back() = mergeNodes(*it, swp);
						auto iter = it;
						it++;
						rootList.erase(iter);
					}
					else{
						//merged+rhs-->merged
						Node<T>* swp = merged.back();
						merged.back() = mergeNodes(*jt, swp);
						jt++; it++;
					}
				}
			}
		}

		if (*it != nullptr){
			while (!merged.empty() && (*it != nullptr) && (merged.back()->degree == (*it)->degree)){
				Node<T>* swp = merged.back();
				merged.back() = mergeNodes(*it, swp);
				auto iter = it;
				it++;
				rootList.erase(iter);
			}
		}
		if (*jt != nullptr){
			while (!merged.empty() && (*jt != nullptr) && (merged.back()->degree == (*jt)->degree)){
				Node<T>* swp = merged.back();
				merged.back() = mergeNodes(*jt, swp);
				jt++;
			}
			while (*jt != nullptr){
				merged.push_back(*jt);
				jt++;
			}
		}

		rhs.rootList.clear();
		rhs.rootList.push_back(nullptr);
		rootList.merge(merged, myCompare<T>);	/*Для этого merge и делались все операции выше
												**В результате предыдущих действий в merged и this все вершины как раз разных степеней
												**не копирование потому что они не упорядочены в том смысле, что все степени
												**merged >(или меньше) степеней из this
												*/
	}
	NodePointer Insert(int key, T data){
		Node<T>* inserted = new Node<T>(key, data);
		NodePointer pointer;
		pointer.ptr = inserted;
		inserted->degree = 1;
		BinHeap ins;
		ins.rootList.push_front(inserted);
		Merge(ins);
		return pointer;
	}
	void Decrease(NodePointer ptr0, int decr = decrease){
		Node<T>* node = ptr0.ptr;
		node->key -= decr;
		Node<T>* ptr = node;
		if (!ptr->parent){
			return;
		}
		Node<T>* lastParent = nullptr;
		while (ptr->parent && ptr->parent->key > ptr->key){
			Node<T>* parent = ptr->parent;
			Node<T>* swp = parent;

			if (swp->child != ptr){
				swp = swp->child;
				while (swp->sibling != ptr) swp = swp->sibling;
				swp->sibling = parent;
			}

			swp = parent->parent;
			if (swp){
				if (swp->child == parent) swp->child = ptr;
				else{
					swp = swp->child;
					while (swp->sibling != parent) swp = swp->sibling;
					swp->sibling = ptr;
				}
			}

			if (parent->child != ptr){
				swp = parent->child;
				parent->child = ptr->child;
				ptr->child = swp;
			}
			else{
				parent->child = ptr->child;
				ptr->child = parent;
			}
			swap(parent->sibling, ptr->sibling);
			ptr->parent = parent->parent;
			parent->parent = ptr;
			swap(ptr->degree, parent->degree);

			swp = ptr->child;
			while (swp->sibling){
				swp->parent = ptr;
				swp = swp->sibling;
			}
			swp->parent = ptr;
			
			swp = parent->child;
			if (swp){
				while (swp->sibling){
					swp->parent = parent;
					swp = swp->sibling;
				}
				swp->parent = parent;
			}
			lastParent = parent;
		}
		if (ptr->parent == nullptr){
			for (auto it = rootList.begin(); *it != nullptr; it++){
				if (*it == lastParent){
					*it = ptr;
					return;
				}
			}
		}
	}
	void Delete(NodePointer ptr){
		Node<T>* node = ptr.ptr;
		int decr = node->key - Min() + 1;
		Decrease(ptr, decr);
		ExtractMin();
	}
	int Min(){
		//будет ошибка если вызвать для пустой кучи!
		//тут exeption кидать или как?
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
				delete *it;
				rootList.erase(it);
				Merge(bh);
				return;
			}
		}
	}
private:
	BinHeap(Node<T>* removedRoot){
		Node<T>* ptr = removedRoot->child;
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
	static Node<T>* mergeNodes(Node<T>* fst, Node<T>* snd){
		Node<T>* root;
		Node<T>* child;
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
