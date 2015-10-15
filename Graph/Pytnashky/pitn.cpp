#include<iostream>
#include<vector>
#include<queue>

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
		status startStatus;
		startStatus.field = start;
		for (int i = 0; i < FieldSize*FieldSize; i++) if (start[i / FieldSize][i%FieldSize] == 0){
			startStatus.nullPlace = i;
			break;
		}
		startStatus.evristic = manhetton(start)/*+linealConflicts(start)*/;
		
		queue.push(startStatus);
		status currentPosition;
		currentPosition = queue.top();
		if (!currentPosition.evristic) return currentPosition.pastMoves;
		queue.pop();
		if ((currentPosition.nullPlace >= FieldSize)){
			queue.push(status(currentPosition, 'u'));
		}
		if (currentPosition.nullPlace / FieldSize < FieldSize - 1){
			queue.push(status(currentPosition, 'd'));
		}
		if ((currentPosition.nullPlace%FieldSize > 0)){
			queue.push(status(currentPosition, 'l'));
		}
		if ((currentPosition.nullPlace%FieldSize < FieldSize - 1)){
			queue.push(status(currentPosition, 'r'));
		}
		while (true){
			currentPosition = queue.top();
			if (!currentPosition.evristic) return currentPosition.pastMoves;
			queue.pop();
			if ((currentPosition.pastMoves[currentPosition.pastMoves.size()-1]!='d')
				&&(currentPosition.nullPlace >= FieldSize)){
				queue.push(status(currentPosition, 'u'));
			}
			if ((currentPosition.pastMoves[currentPosition.pastMoves.size() - 1] != 'u')
				&& (currentPosition.nullPlace / FieldSize < FieldSize - 1)){
				queue.push(status(currentPosition, 'd'));
			}
			if ((currentPosition.pastMoves[currentPosition.pastMoves.size() - 1] != 'r')
				&& (currentPosition.nullPlace%FieldSize > 0)){
				queue.push(status(currentPosition, 'l'));
			}
			if ((currentPosition.pastMoves[currentPosition.pastMoves.size() - 1] != 'l')
				&& (currentPosition.nullPlace%FieldSize < FieldSize - 1)){
				queue.push(status(currentPosition, 'r'));
			}
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
				- abs((field[i][j] - 1) % FieldSize - jOld) - abs((field[i][j] - 1) / FieldSize - iOld);
				//+ linealConflicts(field) - linealConflicts(from.field);
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
					if ((field[k][i] / FieldSize == k) && ((field[k][j] / FieldSize == k)) && field[k][i] 
						&& field[k][j] && (field[k][i] > field[k][j])) evr += 2;
					if ((field[k][i] % FieldSize == k) && ((field[k][j] % FieldSize == k)) && field[i][k] 
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
	return 0;
}