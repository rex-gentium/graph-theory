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
				return; //этот цикл всегда должен быть последним в методе
			}
	}
}

void AdjacencyMatrix::write(ostream & outFile) {
	outFile << 'C' << ' ' << vertexCount << ' ' << ((isWeighted) ? 1 : 0) << '\n';
	for (int from = 0; from < vertexCount; from++)
		for (int to = 0; to < vertexCount; to++)
			outFile << adjacencyMatrix[from][to] << ((to == vertexCount - 1) ? '\n' : ' ');
}

GraphContent * AdjacencyMatrix::getCopy() const
{
	return new AdjacencyMatrix(*this);
}

bool AdjacencyMatrix::hasEdges() const
{
	for (int i = 0; i < adjacencyMatrix.size(); ++i)
		for (int j = 0; j < adjacencyMatrix.size(); ++j)
			if (adjacencyMatrix[i][j])
				return true;
	return false;
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

int AdjacencyMatrix::getWeight(int from, int to) const
{
	return (isWeighted) ? adjacencyMatrix[from][to] : 0;
}

int AdjacencyMatrix::getAdjacent(int from) const
{
	for (int to = 0; to < vertexCount; ++to)
		if (adjacencyMatrix[from][to])
			return adjacencyMatrix[from][to];
	return -1;
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
  
  int AdjacencyMatrix::getVertexDegree(int vertex) const
{
	if (isDirected) 
		return getVertexInDegree(vertex) + getVertexOutDegree(vertex);

	int degree = 0;
	for (int v = 0; v < adjacencyMatrix[vertex].size(); ++v)
		if (adjacencyMatrix[vertex][v]) {
			++degree;
			if (vertex == v && !isDirected)
				++degree; // петля учитывается дважды
		}
	return degree;
}

vector<int> AdjacencyMatrix::getVerticesDegrees() const
{
	vector<int> degrees(vertexCount);
	for (int v = 0; v < vertexCount; ++v)
		degrees[v] = getVertexDegree(v);
	return degrees;
}

int AdjacencyMatrix::getVertexInDegree(int vertex) const
{
	if (!isDirected) return getVertexDegree(vertex);
	int inDegree = 0;
	for (int from = 0; from < vertexCount; ++from)
		if (adjacencyMatrix[from][vertex])
			++inDegree;
	return inDegree;
}

int AdjacencyMatrix::getVertexOutDegree(int vertex) const
{
	if (!isDirected) return getVertexDegree(vertex);
	int outDegree = 0;
	for (int to = 0; to < vertexCount; ++to)
		if (adjacencyMatrix[vertex][to])
			++outDegree;
	return outDegree;
}

vector<int> AdjacencyMatrix::getVerticesInDegrees() const
{
	vector<int> inDegrees(vertexCount);
	for (int v = 0; v < vertexCount; ++v)
		inDegrees[v] = getVertexInDegree(v);
	return inDegrees;
}

vector<int> AdjacencyMatrix::getVerticesOutDegrees() const
{
	vector<int> outDegrees(vertexCount);
	for (int v = 0; v < vertexCount; ++v)
		outDegrees[v] = getVertexOutDegree(v);
	return outDegrees;
}

DSU AdjacencyMatrix::getUnityComponents() const
{
	DSU * result = new DSU(vertexCount);
	for (int from = 0; from < vertexCount; ++from)
		for (int to = (isDirected) ? from : 0; to < vertexCount; ++to)
			if (adjacencyMatrix[from][to])
				result->unite(from, to);
	return *result;
}

DSU AdjacencyMatrix::getUnityComponents(int exceptFrom, int exceptTo) const
{
	DSU * result = new DSU(vertexCount);
	for (int from = 0; from < vertexCount; ++from)
		for (int to = (isDirected) ? from : 0; to < vertexCount; ++to)
			if (adjacencyMatrix[from][to] 
				&& !(from == exceptFrom && to == exceptTo || from == exceptTo && to == exceptFrom))
				result->unite(from, to);
	return *result;
}
