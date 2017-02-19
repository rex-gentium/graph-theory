#include "AdjacencyMatrix.h"
#include "AdjacencyList.h"
#include "EdgeList.h"


AdjacencyMatrix::AdjacencyMatrix() {}

AdjacencyMatrix::AdjacencyMatrix(int vertexCount) {
	this->vertexCount = vertexCount;
	adjacencyMatrix.resize(vertexCount);
	for (int i = 0; i < vertexCount; i++)
		adjacencyMatrix[i].resize(vertexCount);
}


AdjacencyMatrix::~AdjacencyMatrix() {
	if (!adjacencyMatrix.empty()) {
		for (int i = 0; i < adjacencyMatrix.size(); i++)
			adjacencyMatrix[i].clear();
		adjacencyMatrix.clear();
	}
}

void AdjacencyMatrix::read(istream & inpFile) {
	int weightFlag;
	inpFile >> this->vertexCount;
	inpFile >> weightFlag;
	this->isWeighted = (weightFlag > 0);
	adjacencyMatrix.resize(vertexCount);
	for (int from = 0; from < vertexCount; from++) {
		adjacencyMatrix[from].resize(vertexCount);
		for (int to = 0; to < vertexCount; to++)
			inpFile >> adjacencyMatrix[from][to];
	}
	// проверка на ориентированность
	this->isDirected = false;
	for (int from = 0; from < vertexCount; from++)
		for (int to = from; to < vertexCount; to++)
			if (adjacencyMatrix[from][to] != 0 || // наличие петли — достаточное условие
				adjacencyMatrix[from][to] != adjacencyMatrix[to][from]) { // несимметричность — достаточное условие
				this->isDirected = true;
				return; // этот цикл всегда должен стоять последним в методе
			}
}

void AdjacencyMatrix::write(ostream & outFile) {
	outFile << 'C' << ' ' << vertexCount << ' ' << ((isWeighted) ? 1 : 0) << '\n';
	for (int from = 0; from < vertexCount; from++)
		for (int to = 0; to < vertexCount; to++)
			outFile << adjacencyMatrix[from][to] << ((to == vertexCount - 1) ? '\n' : ' ');
}

void AdjacencyMatrix::addEdge(int from, int to, int weight) {
	adjacencyMatrix[from][to] = (isWeighted) ? weight : 1;
	if (!isDirected) // поддержка симметрии матрицы в неориентированном графе
		adjacencyMatrix[to][from] = adjacencyMatrix[from][to];
}

int AdjacencyMatrix::changeEdge(int from, int to, int newWeight) {
	int oldWeight = adjacencyMatrix[from][to];
	adjacencyMatrix[from][to] = newWeight;
	if (!isDirected) // поддержка симметрии матрицы неориентированного графа
		adjacencyMatrix[to][from] = newWeight;
	return oldWeight;
}

void AdjacencyMatrix::removeEdge(int from, int to) {
	adjacencyMatrix[from][to] = 0;
	if (!isDirected) // поддержка симметрии матрицы неориентированного графа
		adjacencyMatrix[to][from] = 0;
}

GraphContent * AdjacencyMatrix::getSpaingTreePrima() {
	AdjacencyMatrix* result = new AdjacencyMatrix(this->vertexCount);
	vector<bool> isMarked;
	isMarked.resize(vertexCount, false);
	isMarked[0] = true;
	//while (/*have unconnected vertice*/) {
		// попытка добавить ребро в остов
		// из всех потенциальных рёбер выбирается минимальное

	//}
	return nullptr;
}
