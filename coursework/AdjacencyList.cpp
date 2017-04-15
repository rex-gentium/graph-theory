#include "AdjacencyList.h"
#include <fstream>
#include <sstream>


AdjacencyList::AdjacencyList() {}

AdjacencyList::AdjacencyList(int vertexCount, bool isWeigted, bool isDirected) {
	this->isWeighted = isWeighted;
	this->isDirected = isDirected;
	this->vertexCount = vertexCount;
	if (isWeighted)
		this->weightedAdjacencyList.resize(vertexCount);
	else
		this->adjacencyList.resize(vertexCount);
}


AdjacencyList::~AdjacencyList() {}

void AdjacencyList::read(istream & inpFile) {
	int orientationFlag, weightFlag;
	inpFile >> this->vertexCount;
	inpFile >> orientationFlag;
	this->isDirected = (orientationFlag > 0);
	inpFile >> weightFlag;
	this->isWeighted = (weightFlag > 0);
	string line;
	getline(inpFile, line); // force jump to next line
	if (isWeighted) {
		for (int vertex = 0; vertex < vertexCount; vertex++) {
			set<pair<int, int>> listContent;
			getline(inpFile, line);
			int to, weight;
			istringstream iss(line);
			while (iss >> to) {
				--to; // поправка на нумерацию с нуля
				iss >> weight;
				listContent.insert(make_pair(to, weight));
			}
			weightedAdjacencyList.push_back(listContent);
		}
	}
	else {
		for (int vertex = 0; vertex < vertexCount; vertex++) {
			set<int> listContent;
			getline(inpFile, line);
			//cout << line;
			istringstream iss(line);
			int to;
			while (iss >> to) {
				--to; // поправка на нумерацию с нуля
				listContent.insert(to);
			}
			adjacencyList.push_back(listContent);
		}
	}
}

void AdjacencyList::write(ostream & outFile) {
	outFile << 'L' << ' ' << vertexCount << '\n'
		<< ((isDirected) ? 1 : 0) << ' ' << ((isWeighted) ? 1 : 0) << '\n';
	if (isWeighted) {
		for (int vertex = 0; vertex < vertexCount; vertex++) {
			for (const auto & adjacency : weightedAdjacencyList[vertex])
				outFile << (adjacency.first + 1) << ' ' << adjacency.second << ' ';
			outFile << '\n';
		}
	}
	else {
		for (int vertex = 0; vertex < vertexCount; vertex++) {
			for (const int & adjacent : adjacencyList[vertex])
				outFile << (adjacent + 1) << ' ';
			outFile << '\n';
		}
	}
}

GraphContent * AdjacencyList::getCopy() const
{
	return new AdjacencyList(*this);
}

bool AdjacencyList::hasEdges() const
{
	if (isWeighted) {
		for (int i = 0; i < vertexCount; ++i)
			if (!weightedAdjacencyList[i].empty())
				return true;
	}
	else {
		for (int i = 0; i < vertexCount; ++i)
			if (!adjacencyList[i].empty())
				return true;
	}
	return false;
}

void AdjacencyList::addEdge(int from, int to, int weight) {
	if (isWeighted) {
		weightedAdjacencyList[from].insert(make_pair(to, weight));
		if (!isDirected) // неориентированный граф дублирует смежности
			weightedAdjacencyList[to].insert(make_pair(from, weight));
	}
	else {
		adjacencyList[from].insert(to);
		if (!isDirected) // неориентированный граф дублирует смежности
			adjacencyList[to].insert(from);
	}
}

int AdjacencyList::changeEdge(int from, int to, int newWeight) {
	int oldWeight = 0;
	set<pair<int, int>> * vertexAdjList = &(weightedAdjacencyList[from]);
	auto it = vertexAdjList->lower_bound(make_pair(to, 0));
	if (it != vertexAdjList->end()) {
		oldWeight = it->second;
		auto insertionHint = it; // should I really be doing this?
		++insertionHint;
		vertexAdjList->erase(it);
		vertexAdjList->insert(insertionHint, make_pair(to, newWeight));
	}
	if (!isDirected) {
		// неориентированный граф дублирует списки
		vertexAdjList = &(weightedAdjacencyList[to]);
		it = vertexAdjList->lower_bound(make_pair(from, 0));
		if (it != vertexAdjList->end()) {
			oldWeight = it->second;
			auto insertionHint = it;
			++insertionHint;
			vertexAdjList->erase(it);
			vertexAdjList->insert(insertionHint, make_pair(from, newWeight));
		}
	}
	return oldWeight;
}

void AdjacencyList::removeEdge(int from, int to) {
	if (isWeighted) {
		set<pair<int, int>> * vertexAdjList = &(weightedAdjacencyList[from]);
		// вес не имеет значения, сравнение происходит по первому элементу пары
		auto it = vertexAdjList->lower_bound(make_pair(to, 0));
		if (it != vertexAdjList->end())
			vertexAdjList->erase(it);
		if (!isDirected) { // в неориентированном графе списки дублируются
			vertexAdjList = &(weightedAdjacencyList[to]);
			it = vertexAdjList->lower_bound(make_pair(from, 0));
			if (it != vertexAdjList->end())
				vertexAdjList->erase(it);
		}
	}
	else {
		set<int> * vertexAdjList = &(adjacencyList[from]);
		auto it = vertexAdjList->find(to);
		if (it != vertexAdjList->end())
			vertexAdjList->erase(it);
		if (!isDirected) { // в неориентированном графе списки дублируются
			vertexAdjList = &(adjacencyList[to]);
			it = vertexAdjList->find(from);
			if (it != vertexAdjList->end())
				vertexAdjList->erase(it);
		}
	}
}

int AdjacencyList::getWeight(int from, int to) const
{
	if (!isWeighted)
		return 0;
	auto it = weightedAdjacencyList[from].lower_bound(make_pair(to, 0));
	return (it != weightedAdjacencyList[from].end()) ? it->second : 0;
}

int AdjacencyList::getAdjacent(int from) const
{
	if (isWeighted)
		return (weightedAdjacencyList[from].empty()) ? -1 : weightedAdjacencyList[from].begin()->first;
	else
		return (adjacencyList[from].empty()) ? -1 : *adjacencyList[from].begin();
}

list<tuple<int, int, int>> AdjacencyList::getWeightedEdgesList() const
{
	list<tuple<int, int, int>> result;
	for (int from = 0; from < vertexCount; ++from)
		for (const auto & adjacency : weightedAdjacencyList[from]) {
			int to = adjacency.first;
			int weight = adjacency.second;
			if (isDirected || from <= to) // избегаем дублей в неориентированном графе
				result.push_back(make_tuple(from, to, weight));
		}
	return result;
}

tuple<int, int, int> AdjacencyList::findMinEdge(bool * isMarked) const
{
	int minWeight = INT_MAX, minI = -1, minJ = -1;
	for (int i = 0; i < vertexCount; i++) {
		if (!isMarked[i]) continue;
		for (const auto & adjacency : weightedAdjacencyList[i]) {
			int j = adjacency.first;
			if (isMarked[j]) continue;
			int weight = adjacency.second;
			if (weight < minWeight) {
				minWeight = weight;
				minI = i;
				minJ = j;
			}
		}
	}
	return make_tuple(minI, minJ, minWeight);
}

int AdjacencyList::getVertexDegree(int vertex) const
{
	if (isDirected)
		return getVertexInDegree(vertex) + getVertexOutDegree(vertex);
	int degree = 0;
	if (isWeighted) {
		degree = weightedAdjacencyList[vertex].size();
		if (weightedAdjacencyList[vertex].lower_bound(make_pair(vertex, 0)) != weightedAdjacencyList[vertex].end())
			++degree; // петля учитывается дважды
	}
	else {
		degree = adjacencyList[vertex].size();
		if (adjacencyList[vertex].find(vertex) != adjacencyList[vertex].end())
			++degree; // петля учитывается дважды
	}
	return 0;
}

vector<int> AdjacencyList::getVerticesDegrees() const
{
	vector<int> degrees(vertexCount);
	for (int v = 0; v < vertexCount; ++v)
		degrees[v] = getVertexDegree(v);
	return degrees;
}

int AdjacencyList::getVertexInDegree(int vertex) const
{
	if (!isDirected) return getVertexDegree(vertex);
	int inDegree = 0;
	if (!isWeighted) {
		for (int from = 0; from < vertexCount; ++from)
			if (adjacencyList[from].find(vertex) != adjacencyList[from].end())
				++inDegree;
	}
	else {
		for (int from = 0; from < vertexCount; ++from)
			if (weightedAdjacencyList[from].lower_bound(make_pair(vertex, 0)) != weightedAdjacencyList[from].end())
				++inDegree;
	}
	return inDegree;
}

int AdjacencyList::getVertexOutDegree(int vertex) const
{
	if (!isDirected) return getVertexDegree(vertex);
	return (isWeighted) ? weightedAdjacencyList[vertex].size() : adjacencyList[vertex].size();
}

vector<int> AdjacencyList::getVerticesInDegrees() const
{
	vector<int> inDegrees(vertexCount, 0);
	for (int v = 0; v < vertexCount; ++v)
		if (isWeighted)
			for (const auto & adjacency : weightedAdjacencyList[v])
				++inDegrees[adjacency.first];
		else for (const auto & adjacency : adjacencyList[v])
				++inDegrees[adjacency];
	return inDegrees;
}

vector<int> AdjacencyList::getVerticesOutDegrees() const
{
	vector<int> outDegrees(vertexCount);
	for (int v = 0; v < vertexCount; ++v)
		outDegrees[v] = getVertexOutDegree(v);
	return outDegrees;
}

DSU AdjacencyList::getUnityComponents() const
{
	DSU result(vertexCount);
	if (isWeighted)
		for (int from = 0; from < vertexCount; ++from)
			for (const auto & adjacency : weightedAdjacencyList[from])
				result.unite(from, adjacency.first);
	else
		for (int from = 0; from < vertexCount; ++from)
			for (const auto & to : adjacencyList[from])
				result.unite(from, to);
	return result;
}

DSU AdjacencyList::getUnityComponents(int exceptFrom, int exceptTo) const
{
	DSU result(vertexCount);
	if (isWeighted)
		for (int from = 0; from < vertexCount; ++from)
			for (const auto & adjacency : weightedAdjacencyList[from]) {
				int to = adjacency.first;
				if (from == exceptFrom && to == exceptTo
					|| !isDirected && from == exceptTo && to == exceptFrom)
					continue;
				result.unite(from, adjacency.first);
			}
	else
		for (int from = 0; from < vertexCount; ++from)
			for (const auto & to : adjacencyList[from]) {
				if (from == exceptFrom && to == exceptTo
					|| !isDirected && from == exceptTo && to == exceptFrom)
					continue;
				result.unite(from, to);
			}
	return result;
}
