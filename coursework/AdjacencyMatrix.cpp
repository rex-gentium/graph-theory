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
		for (size_t i = 0; i < adjacencyMatrix.size(); i++)
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
	for (int from = 0; from < vertexCount; from++) {
		if (adjacencyMatrix[from][from] != 0) { // наличие петли — достаточное условие
			this->isDirected = true;
			break;
		}
		for (int to = from; to < vertexCount; to++)
			if (adjacencyMatrix[from][to] != adjacencyMatrix[to][from]) { // асимметричность — достаточное условие
				this->isDirected = true;
				return; // этот цикл всегда должен стоять последним в методе
			}
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
	result->isDirected = isDirected;
	result->isWeighted = isWeighted;
	bool * isMarked = new bool[vertexCount];
	for (int i = 0; i < vertexCount; i++)
		isMarked[i] = false;
	
	isMarked[0] = true;
	bool hasUnmarked = (vertexCount == 1) ? false : true;
	
	while (hasUnmarked) {
		// ищем минимальное из ребёр, соединяющих помеченную вершину i с непомеченной j
		int minWeight = INT_MAX, minI = -1, minJ = -1;
		for (int i = 0; i < vertexCount; i++) {
			if (!isMarked[i]) continue;
			for (int j = i + 1; j < vertexCount; j++) {
				if (isMarked[j]) continue;
				if (adjacencyMatrix[i][j] != 0 && adjacencyMatrix[i][j] < minWeight) {
					minWeight = adjacencyMatrix[i][j];
					minI = i;
					minJ = j;
				}
			}
		}
		
		if (minI >= 0) {
			// нашли ребро
			result->addEdge(minI, minJ, minWeight);
			isMarked[minI] = isMarked[minJ] = true;
		}
		else { // если не нашли никакого ребра, помечаем любую недостижимую вершину
			for (int i = 0; i < vertexCount; i++)
				if (!isMarked[i]) {
					isMarked[i] = true;
					break;
				};
		}
		// смотрим, есть ли ещё непомеченные
		hasUnmarked = false;
		for (int i = 0; i < vertexCount; i++)
			if (!isMarked[i]) {
				hasUnmarked = true;
				break;
			}
	}

	delete[] isMarked;
	return result;
}

GraphContent * AdjacencyMatrix::getSpaingTreeKruscal() {
	return nullptr;
}

GraphContent * AdjacencyMatrix::getSpaingTreeBoruvka() {
	return nullptr;
}