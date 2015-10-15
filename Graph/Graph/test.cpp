#include"CMyGraph.h"

int main(){
	CMyGraph<int, int, 0> gr;
	/*gr.AddVertex(0);
	gr.AddVertex(1);
	gr.AddVertex(2);
	gr.AddVertex(3);
	gr.AddVertex(4);
	gr.AddVertex(5);
	gr.AddVertex(6);
	gr.AddVertex(7);
	gr.AddVertex(8);
	gr.AddVertex(9);
	gr.AddVertex(10);
	gr.AddVertex(11);

	gr.AddEdge(0, 1, 1000);
	gr.AddEdge(1, 2, 100);
	gr.AddEdge(2, 0, 1);
	gr.AddEdge(3, 7, 1);
	gr.AddEdge(2, 6, 9);

	gr.AddEdge(0, 9, 1000);
	gr.AddEdge(1, 5, 100);
	gr.AddEdge(2, 6, 1);
	gr.AddEdge(3, 0, 1);
	gr.AddEdge(6, 1, 9);
	gr.AddEdge(6, 7, 1000);
	gr.AddEdge(6, 8, 100);
	gr.AddEdge(6, 9, 1);
	gr.AddEdge(6, 10, 1);
	gr.AddEdge(6, 11, 9);
	gr.AddEdge(9, 0, 1000);
	gr.AddEdge(9, 4, 100);
	gr.AddEdge(9, 6, 1);
	gr.AddEdge(7, 3, 1);
	gr.AddEdge(7, 11, 9);
	*/
	
	///*gr.DeleteEdge(gr.OutgoingEdgesBegin(gr.FindVertex(0)));
	//gr.DeleteEdge(gr.OutgoingEdgesBegin(gr.FindVertex(2)));
	//gr.DeleteEdge(gr.OutgoingEdgesBegin(gr.FindVertex(2)));*/

	//gr.OutgoingEdgesBegin(gr.Begin());

	/*gr.Transpose();
	gr.DeleteVertex(gr.Begin());
	*/
	//auto it = gr.Begin();
	//it.IsValid(); не прокатывает
	
	
	/*
	map<int, int> ttt = gr.Kosarayu();
	map<int, int> kkk = gr.Taryano();
*/
	

	//bool res = compareCompanents<int>(ttt, kkk);

	int N;
	cin >> N;
	int from, to, weight;
	for (int i = 0; i < N; i++){
		cin >> from >> to >> weight;
		gr.AddEdge(from, to, weight);
	}
	//cin >> from;
	map<int, int> res=gr.BellmanFord(0);

	for (int i = 0; i < N; i++) cout << res[i] << endl;


	return 0;
}