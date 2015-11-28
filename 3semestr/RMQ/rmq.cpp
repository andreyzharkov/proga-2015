#include<vector>
#include<iostream>
#include<math.h>
#include<algorithm>

using namespace std;

const int INF = INT_MAX;

class RMQ{
public:
	RMQ(vector<double>& vect){
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
	double Min(int left, int right){
		int n = tree.size() / 2;
		left += k, right += k;
		double ans = std::min(tree[left], tree[right]);
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
	double left(int i){ return (2 * i + 1 < tree.size() - 1) ? tree[2 * i + 1] : INF; }
	double right(int i){ return (2 * i + 2 < tree.size() - 1) ? tree[2 * i + 2] : INF; }
	vector<double> tree;
	int k;//количество не листов в дереве
};

int main(){
	int N, M;
	cin >> N;
	vector<double> v;
	double d;
	for (int i = 0; i < N; i++){
		cin >> d;
		v.push_back(d);
	}
	
	RMQ rmq(v);
	
	int a, b;
	cin >> M;
	for (int i = 0; i < M; i++){
		cin >> a >> b;
		cout << rmq.Min(a, b-1) << endl;
	}
	
	return 0;
}