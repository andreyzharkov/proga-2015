#include<algorithm>

struct Node{
	Node(int k){
		parent = child = nullptr;
		left = right = this;
		mark = false;
		key = k;
		degree = 1;
	}
	~Node(){
		if (child) delete child;
		Node* ptr;
		while (right != this && right != nullptr){
			ptr = right;
			ptr->left->right = ptr->right;
			ptr->right->left = ptr->left;
			ptr->left = ptr->right = ptr;
			delete ptr;
		}
	}
	Node* parent;
	Node* left;
	Node* right;
	Node* child;
	int key;
	bool mark;
	size_t degree;
};

class FibHeap{
	friend struct Node;
private:
	Node* min;
	int size;
public:
	FibHeap(){ size = 0; min = nullptr; }
	void Insert(int key){
		FibHeap fh;
		fh.min = new Node(key);
		fh.size = 1;
		Merge(fh);
	}
	void Merge(FibHeap& fh){
		if (fh.size == 0) return;
		if (size == 0){
			min = fh.min;
			size = fh.size;
			fh.min = nullptr;
			fh.size = 0;
			return;
		}
		concat(min, fh.min);
		min = (std::min(min->key, fh.min->key) == min->key) ? min : fh.min;
		fh.min = nullptr;
		size += fh.size;
		fh.size = 0;
	}
	int Min(){
		return min->key;
	}
	void ExtractMin(){
		if (size>0) size--;
		else return;
		if (min->child){
			min->child->parent = nullptr;
			for (Node* ptr = min->child->right; ptr != min->child; ptr = ptr->right){
				ptr->parent = nullptr;
			}
			concat(min, min->child);
		}
		if (min->left == min){
			delete min;
			min = nullptr;
			size = 0;
			return;
		}
		Node* exmin = min;
		min = min->right;
		removeFromBrothers(exmin);
		exmin->left = exmin->right = exmin->child = nullptr;
		delete exmin;
		consolidate();
		min = findNewMin();
	}
	void Decrease(Node* node, int decr){
		node->key -= decr;
		if (node->parent && node->parent->key < node->key) return;
		if (node->parent){
			Node* parent = node->parent;
			cut(node);
			if (node->key < min->key) min = node;
			while (parent && parent->mark){
				Node* node = parent->parent;
				if (!parent->mark){
					parent->mark = true;
					return;
				}///////////////////////
				cut(parent);
				parent = node;
			}
		}
	}
	void Delete(Node* node){
		Decrease(node, node->key - min->key + 1);
		ExtractMin();
	}
private:
	static void concat(Node* first, Node* second){
		Node* ff = first->right;
		Node* ss = second->left;
		first->right = second;
		second->left = first;
		ff->left = ss;
		ss->right = ff;
	}
	void cut(Node* node){
		node->parent->degree--;
		if (node->parent->child == node){
			node->parent->child = node->right;
			if (node == node->right) node->parent->child = nullptr;
		}
		node->parent = nullptr;
		removeFromBrothers(node);
		concat(min, node);
	}
	static Node* merge(Node* first, Node* second){
		if (first->key < second->key){
			if (first->child){
				concat(first->child, second);
			}
			else{
				first->child = second;
			}
			first->degree++;
			return first;
		}
		else{
			if (second->child){
				concat(second->child, first);
			}
			else{
				second->child = first;
			}
			second->degree++;
			return second;
		}
	}
	static void removeFromBrothers(Node* ptr){
		ptr->left->right = ptr->right;
		ptr->right->left = ptr->left;
		ptr->left = ptr->right = ptr;
	}
	void consolidate(){
		Node* ptr = min->right;
		if (ptr == min) return;
		ptr = ptr->right;
		while (ptr != min){
			if (ptr->left == min){
				ptr = ptr->right;
				continue;
			}
			if (ptr->left->degree == ptr->degree){
				Node* nd1 = ptr->left;
				Node* nd2 = ptr;
				if (nd1->key < nd2->key){
					removeFromBrothers(nd2);
					ptr=merge(nd1, nd2);
				}
				else{
					removeFromBrothers(nd1);
					ptr = merge(nd1, nd2);
				}
			}
			else{
				ptr = ptr->right;
			}
		}
	}
	Node* findNewMin(){
		int m=min->key;
		Node* ans = min;
		for (Node* ptr = min->right; ptr != min; ptr = ptr->right){
			if (m>ptr->key){
				m = ptr->key;
				ans = ptr;
			}
		}
		return ans;
	}
};