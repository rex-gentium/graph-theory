﻿#include "AdjacencyMatrix.h"
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
		for (int j = (isDirected ? 0 : i); j < vertexCount; ++j) {
			if (adjacencyMatrix[i][j])
				result.push_back(make_tuple(i, j, adjacencyMatrix[i][j]));
		}
	return result;
}

tuple<int, int, int> AdjacencyMatrix::findMinEdge(bool * isMarked) const
{
	int minWeight = INT_MAX, minI = -1, minJ = -1;
	for (int i = 0; i < vertexCount; i++) {
		if (!isMarked[i]) continue;
		for (int j = i; j < vertexCount; j++) {
			if (isMarked[j]) continue;
			if (adjacencyMatrix[i][j] != 0 && adjacencyMatrix[i][j] < minWeight) {
				minWeight = adjacencyMatrix[i][j];
				minI = i;
				minJ = j;
			}
		}
	}
	return make_tuple(minI, minJ, minWeight);
}

GraphContent::ConstEdgeIterator& AdjacencyMatrix::edgeBegin() const
{
	ConstEdgeIterator * result = &edgeEnd();
	for (int i = 0; i < vertexCount; ++i)
		for (int j = (isDirected) ? 0 : i; j < vertexCount; ++j)
			if (adjacencyMatrix[i][j]) {
				//delete result;
				int weight = (isWeighted) ? adjacencyMatrix[i][j] : -1;
				result = new ConstEdgeIterator(this, i, j, weight);
				return *result;
			}
	return *result;
}

GraphContent::ConstEdgeIterator& AdjacencyMatrix::edgeEnd() const
{
	return *new ConstEdgeIterator(this, -1, -1, -1);
}

tuple<int, int, int> AdjacencyMatrix::nextEdge(ConstEdgeIterator & iterator) const
{
	int from = iterator.from(), to = iterator.to();
	// сначала просматриваем текущую строку
	for (int j = to + 1; j < vertexCount; ++j)
		if (adjacencyMatrix[from][j])
			return make_tuple(from, j, (isWeighted) ? adjacencyMatrix[from][j] : -1);
	// потом уже ищем как обычно
	for (int i = from + 1; i < vertexCount; ++i)
		for (int j = (isDirected) ? 0 : i; j < vertexCount; ++j)
			if (adjacencyMatrix[i][j])
				return make_tuple(i, j, (isWeighted) ? adjacencyMatrix[i][j] : -1);
	return make_tuple(-1, -1, -1);
}
