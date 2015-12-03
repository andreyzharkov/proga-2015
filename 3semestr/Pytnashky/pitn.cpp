#include<iostream>
#include<vector>
#include<queue>
#include<set>

using namespace std;

template<int FieldSize>
class PitnSolver{
public:
	typedef vector<vector<int>> Field;
	typedef int nullPlace;

	PitnSolver(istream& inputStream){
		start = Field(FieldSize, vector<int>(FieldSize));
		for (int i = 0; i < FieldSize; i++){
			for (int j = 0; j < FieldSize; j++){
				inputStream >> start[i][j];
			}
		}
	}
	bool doesSolutionExist(){
		int inv = 0;
		for (int i = 0; i<FieldSize*FieldSize; ++i)
			if (start[i/FieldSize][i%FieldSize])
				for (int j = 0; j<i; ++j)
					if ((start[j / FieldSize][j%FieldSize] < start[i / FieldSize][i%FieldSize])&&
						(start[j / FieldSize][j%FieldSize]!=0)) ++inv;
		for (int i = 0; i<FieldSize*FieldSize; ++i)
			if (start[i / FieldSize][i%FieldSize] == 0)
				inv += 1 + i / FieldSize;
		return inv % 2 ? true : false;
	}
	vector<char> findSolution(){
		priority_queue<status, vector<status>, compare> queue;
		set<Field> checkedPositions;
		status startStatus;
		startStatus.field = start;
		for (int i = 0; i < FieldSize*FieldSize; i++) if (start[i / FieldSize][i%FieldSize] == 0){
			startStatus.nullPlace = i;
			break;
		}
		startStatus.evristic = manhetton(start)+linealConflicts(start);
		
		queue.push(startStatus);
		checkedPositions.insert(startStatus.field);
		//это я что-то проверял, забыл закомментить
		/*if (checkedPositions.end() == checkedPositions.find(status(status(startStatus, 'd'), 'u').field)){
			cout << "ok" << endl;
		}*/
		status currentPosition;
		
		while (true){
			currentPosition = queue.top();
			if (!currentPosition.evristic) return currentPosition.pastMoves;
			queue.pop();
			
			if ((currentPosition.nullPlace >= FieldSize)){
				status st(currentPosition, 'u');
				if (checkedPositions.find(st.field) == checkedPositions.end()){
					queue.push(st);
					checkedPositions.insert(st.field);
				}
			}
			if ((currentPosition.nullPlace / FieldSize < FieldSize - 1)){
				status st(currentPosition, 'd');
				if (checkedPositions.find(st.field) == checkedPositions.end()){
					queue.push(st);
					checkedPositions.insert(st.field);
				}
			}
			if ((currentPosition.nullPlace%FieldSize > 0)){
				status st(currentPosition, 'l');
				if (checkedPositions.find(st.field) == checkedPositions.end()){
					queue.push(st);
					checkedPositions.insert(st.field);
				}
			}
			if ((currentPosition.nullPlace%FieldSize < FieldSize - 1)){
				status st(currentPosition, 'r');
				if (checkedPositions.find(st.field) == checkedPositions.end()){
					queue.push(st);
					checkedPositions.insert(st.field);
				}
			}
		}
	}
	void checkEvristic(vector<char>& minWay){
		status* prStatus = new status();
		prStatus->field = start;
		for (int i = 0; i < FieldSize*FieldSize; i++) if (prStatus->field[i / FieldSize][i%FieldSize] == 0){
			prStatus->nullPlace = i;
			break;
		}
		prStatus->evristic = manhetton(prStatus->field);

		for (size_t i = 0; i < minWay.size(); i++){
			status* currentStatus = new status(*prStatus, minWay[i]);
			if (currentStatus->evristic + linealConflicts(currentStatus->field)+i > minWay.size()){
				for (int k = 0; k < FieldSize; k++){
					for (int j = 0; j < FieldSize; j++){
						cout << currentStatus->field[k][j];
					}
				}
			}
			swap(currentStatus, prStatus);
			delete currentStatus;
		}
	}
private:
	class status{
	public:
		Field field;
		int nullPlace;
		vector<char> pastMoves;
		int evristic;
		status(){}
		status(const status& from, char move){
			field = from.field;
			pastMoves = from.pastMoves;
			pastMoves.push_back(move);
			switch (move){
			case 'u':
				nullPlace = from.nullPlace - FieldSize;
				break;
			case 'd':
				nullPlace = from.nullPlace + FieldSize;
				break;
			case 'r':
				nullPlace = from.nullPlace + 1;
				break;
			case 'l':
				nullPlace = from.nullPlace - 1;
			}
			field[from.nullPlace / FieldSize][from.nullPlace%FieldSize] = field[nullPlace / FieldSize][nullPlace%FieldSize];
			field[nullPlace / FieldSize][nullPlace%FieldSize] = 0;

			int i = from.nullPlace/FieldSize, j = from.nullPlace%FieldSize;
			int iOld = nullPlace / FieldSize, jOld = nullPlace%FieldSize;

			evristic = from.evristic + abs((field[i][j] - 1) % FieldSize - j) + abs((field[i][j] - 1) / FieldSize - i)
				- abs((field[i][j] - 1) % FieldSize - jOld) - abs((field[i][j] - 1) / FieldSize - iOld)
				+ linealConflicts(field) - linealConflicts(from.field);
		}
		static int changeLinEvr(status& newStatus, int prevNull){
			int ch = 0;
			int nowNull = newStatus.nullPlace;
			int iNow = prevNull / 3, jNow = prevNull % 3;
			int iOld = nowNull / 3, jOld = nowNull % 3;
			if (iOld == iNew){
				int i = iOld;
				for (int k = 0; k < FieldSize; k++){
					if (k != i){
						if (((newStatus.field[i][jNew] - 1) / FieldSize == i) && (((newStatus.field[i][k] - 1) / FieldSize == i))
							&& (newStatus.field[i][jNew] * (k - i) > newStatus.field[i][k] * (k - i))) ch -= 2;
						if (((newStatus.field[i][jNew] - 1) / FieldSize == i) && (((newStatus.field[i][k] - 1) / FieldSize == i))
							&& (newStatus.field[i][jNew] * (k - i) > newStatus.field[i][j] * (k - i))) ch -= 2;
					}
				}
			}
		}
	};
	struct compare
		: public binary_function<status, status, bool>
	{	// functor for operator<
		bool operator()(const status& _Left, const status& _Right) const
		{	// apply operator< to operands
			return (_Left.evristic+_Left.pastMoves.size() > _Right.evristic+_Right.pastMoves.size());
		}
	};
	static int manhetton(const Field& field){
		int evr=0;
		for (int i = 0; i < FieldSize; i++){
			for (int j = 0; j < FieldSize; j++){
				if (field[i][j] != 0) evr += abs((field[i][j] - 1) % FieldSize - j) + abs((field[i][j] - 1) / FieldSize - i);
			}
		}
		return evr;
	}
	static int linealConflicts(const Field& field){
		int evr = 0;
		for (int i = 0; i < FieldSize; i++){
			for (int j = i+1; j < FieldSize; j++){
				for (int k = 0; k < FieldSize; k++){
					if (((field[k][i]-1) / FieldSize == k) && (((field[k][j]-1) / FieldSize == k)) && field[k][i] 
						&& field[k][j] && (field[k][i] > field[k][j])) evr += 2;
					if (((field[k][i]-1) % FieldSize == k) && ((field[k][j]-1) % FieldSize == k) && field[i][k] 
						&& field[j][k] && (field[i][k] > field[j][k])) evr += 2;
				}
			}
		}
		return evr;
	}
	Field start;
};

int main(){
	PitnSolver<4> ps(cin);
	
	if (ps.doesSolutionExist()){
		cout << "yes" << endl;
		vector<char> moves = ps.findSolution();
		cout << moves.size() << endl;
		for (size_t i = 0; i < moves.size(); i++)
			cout << moves[i] << endl;
	}
	else{
		cout << "no" << endl;
	}
	//vector<char> moves = ps.findSolution();
	//ps.checkEvristic(moves);
	return 0;
}