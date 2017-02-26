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

list<tuple<int, int, int>> AdjacencyMatrix::getWeightedEdgesList() const
{
	list<tuple<int, int, int>> result;
	for (int i = 0; i < vertexCount; ++i)
		for (int j = (isDirected ? 0 : i + 1); j < vertexCount; ++j) {
			if (adjacencyMatrix[i][j])
				result.push_back(make_tuple(i, j, adjacencyMatrix[i][j]));
		}
	return result;
}

set<pair<int, int>> AdjacencyMatrix::getWeightedAdjacencies(int vertex) const
{
	set<pair<int, int>> result;
	for (int j = 0; j < adjacencyMatrix[vertex].size(); ++j)
		if (adjacencyMatrix[vertex][j])
			result.insert(make_pair(j, adjacencyMatrix[vertex][j]));
	return result;
}
