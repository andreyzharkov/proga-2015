#include<algorithm>

template<typename T>
struct Node{
	Node(int k, T data){
		parent = child = nullptr;
		left = right = this;
		mark = false;
		key = k;
		this->data = data;
		degree = 1;
	}
	~Node(){
		if (child) delete child;
		Node<T>* ptr;
		while (right != this && right != nullptr){
			ptr = right;
			ptr->left->right = ptr->right;
			ptr->right->left = ptr->left;
			ptr->left = ptr->right = ptr;
			delete ptr;
		}
	}
	Node<T>* parent;
	Node<T>* left;
	Node<T>* right;
	Node<T>* child;
	int key;
	T data;
	bool mark;
	size_t degree;
};

template<typename T>
class FibHeap{
	friend struct Node<T>;
private:
	Node<T>* min;
	int size;
	static const int decrease = 1000;
public:
	class NodePointer{
		friend class FibHeap<T>;
		Node<T>* ptr;
	public:
		int key() const{ return ptr->key; }
		const T& data() const{ return ptr->data; }
	};
	FibHeap(){ size = 0; min = nullptr; }
	~FibHeap(){
		delete min;
	}
	NodePointer Insert(int key, T data){
		FibHeap fh;
		NodePointer ptr;
		ptr.ptr = fh.min = new Node<T>(key, data);
		fh.size = 1;
		Merge(fh);
		return ptr;
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
			for (Node<T>* ptr = min->child->right; ptr != min->child; ptr = ptr->right){
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
		Node<T>* exmin = min;
		min = min->right;
		removeFromBrothers(exmin);
		exmin->left = exmin->right = exmin->child = nullptr;
		delete exmin;
		consolidate();
		min = findNewMin();
	}
	void Decrease(NodePointer ptr, int decr=decrease){
		Node<T>* node = ptr.ptr;
		node->key -= decr;
		if (node->parent && node->parent->key < node->key) return;
		if (node->parent){
			Node<T>* parent = node->parent;
			cut(node);
			if (node->key < min->key) min = node;
			while (parent && parent->mark){
				Node<T>* node = parent->parent;
				if (!parent->mark){
					parent->mark = true;
					return;
				}///////////////////////
				cut(parent);
				parent = node;
			}
		}
		if (node->key < min->key) min = node;
	}
	void Delete(NodePointer ptr){
		Node<T>* node = ptr.ptr;
		Decrease(ptr, node->key - min->key + 1);
		ExtractMin();
	}
private:
	static void concat(Node<T>* first, Node<T>* second){
		Node<T>* ff = first->right;
		Node<T>* ss = second->left;
		first->right = second;
		second->left = first;
		ff->left = ss;
		ss->right = ff;
	}
	void cut(Node<T>* node){
		node->parent->degree--;
		if (node->parent->child == node){
			node->parent->child = node->right;
			if (node == node->right) node->parent->child = nullptr;
		}
		node->parent = nullptr;
		removeFromBrothers(node);
		concat(min, node);
	}
	static Node<T>* merge(Node<T>* first, Node<T>* second){
		if (first->key < second->key){
			if (first->child){
				concat(first->child, second);
			}
			else{
				first->child = second;
			}
			second->parent = first;
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
			first->parent = second;
			second->degree++;
			return second;
		}
	}
	static void removeFromBrothers(Node<T>* ptr){
		ptr->left->right = ptr->right;
		ptr->right->left = ptr->left;
		ptr->left = ptr->right = ptr;
	}
	void consolidate(){
		Node<T>* ptr = min->right;
		if (ptr == min) return;
		ptr = ptr->right;
		while (ptr != min){
			if (ptr->left == min){
				ptr = ptr->right;
				continue;
			}
			if (ptr->left->degree == ptr->degree){
				Node<T>* nd1 = ptr->left;
				Node<T>* nd2 = ptr;
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
	Node<T>* findNewMin(){
		int m=min->key;
		Node<T>* ans = min;
		for (Node<T>* ptr = min->right; ptr != min; ptr = ptr->right){
			if (m>ptr->key){
				m = ptr->key;
				ans = ptr;
			}
		}
		return ans;
	}
};