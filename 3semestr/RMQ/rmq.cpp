#include<vector>
#include<math.h>
#include<algorithm>

using namespace std;

const int INF = INT_MAX;

class RMQ{
public:
	RMQ(vector<int>& vect){
		int a = vect.size()/2;
		k = 0;
		while (a){
			k++;
			a /= 2;
		}
		k = (int)pow(2.0, k+1)-1;
		tree.resize(k);
		tree.insert(tree.end(), vect.begin(), vect.end());
		for (int i = k - 1; i >= 0; i--){
			tree[i] = std::min(left(i), right(i));
		}
	}
	int Min(int left, int right){
		int n = tree.size() / 2;
		left += k, right += k;
		int ans = std::min(tree[left], tree[right]);
		while (left <= right)
		{
			// если l - правый сын своего родителя, 
			// учитываем его фундаментальный отрезок
			if (!(left % 2))
				ans = std::min(ans, tree[left]);
			// если r - левый сын своего родителя, 
			// учитываем его фундаментальный отрезок
			if (right % 2)
				ans = std::min(ans, tree[right]);
			// сдвигаем указатели на уровень выше
			left = (left + 1) / 2, right = (right - 1) / 2;
		}
		return ans;
	}
private:
	int left(int i){ return (2 * i + 1 < tree.size() - 1) ? tree[2 * i + 1] : INF; }
	int right(int i){ return (2 * i + 2 < tree.size() - 1) ? tree[2 * i + 2] : INF; }
	vector<int> tree;
	int k;//количество не листов в дереве
};

int main(){
	vector<int> v = { 0, 1, 2, 3, 4, 5, 6 };
	RMQ rmq(v);
	rmq.Min(0, 5);
	rmq.Min(0, 1);
	rmq.Min(0, 3);
	rmq.Min(0, 2);
	rmq.Min(1, 4);
	rmq.Min(0, 5);
	rmq.Min(5, 5);
	rmq.Min(2, 3);
	return 0;
}