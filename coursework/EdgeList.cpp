#include "EdgeList.h"
#include <tuple>

EdgeList::EdgeList() {}

EdgeList::~EdgeList() {}

void EdgeList::read(istream & inpFile) {
	int orientationFlag, weightFlag;
	inpFile >> this->vertexCount;
	int edgeCount;
	inpFile >> edgeCount;
	inpFile >> orientationFlag;
	this->isDirected = (orientationFlag > 0);
	inpFile >> weightFlag;
	this->isWeighted = (weightFlag > 0);
	for (int edge = 0; edge < edgeCount; edge++) {
		int from, to;
		inpFile >> from;
		inpFile >> to;
		--from;
		--to; // поправка на нумерацию с нуля
		if (isWeighted) {
			int weight;
			inpFile >> weight;
			this->weightedEdgeList.insert(make_tuple(from, to, weight));
		}
		else
			this->edgeList.insert(make_pair(from, to));
	}
}

void EdgeList::write(ostream & outFile) {
	outFile << 'E' << ' ' << vertexCount << ' '
		<< ((isWeighted) ? weightedEdgeList.size() : edgeList.size()) << '\n'
		<< ((isDirected) ? 1 : 0) << ' ' << ((isWeighted) ? 1 : 0) << '\n';
	if (isWeighted)
		for (const auto & edge : weightedEdgeList)
			outFile << get<0>(edge) + 1 << ' ' << get<1>(edge) + 1 << ' ' << get<2>(edge) << '\n';
	else
		for (const auto & edge : edgeList)
			outFile << edge.first + 1 << ' ' << edge.second + 1 << '\n';
}

GraphContent * EdgeList::getCopy() const
{
	return new EdgeList(*this);
}

bool EdgeList::hasEdges() const
{
	return (isWeighted) ? !weightedEdgeList.empty() : !edgeList.empty();
}

void EdgeList::addEdge(int from, int to, int weight) {
	if (isWeighted)
		weightedEdgeList.insert(make_tuple(from, to, weight));
	else
		edgeList.insert(make_pair(from, to));
}

int EdgeList::changeEdge(int from, int to, int newWeight) {
	int oldWeight = 0;
	auto it = weightedEdgeList.lower_bound(make_tuple(from, to, 0));
	if (it != weightedEdgeList.end() &&
		get<0>(*it) == from && get<1>(*it) == to) {
		oldWeight = get<2>(*it);
		auto insertionHint = it;
		++insertionHint;
		weightedEdgeList.erase(it);
		weightedEdgeList.insert(insertionHint, make_tuple(from, to, newWeight));
	}
	if (!isDirected) { // в неориентированном графе ребро может связывать вершины в обратном порядке
		it = weightedEdgeList.lower_bound(make_tuple(to, from, 0));
		if (it != weightedEdgeList.end() &&
			get<0>(*it) == to && get<1>(*it) == from) {
			oldWeight = get<2>(*it);
			auto insertionHint = it;
			++insertionHint;
			weightedEdgeList.erase(it);
			weightedEdgeList.insert(insertionHint, make_tuple(to, from, newWeight));
		}
	}
	return oldWeight;
}

void EdgeList::removeEdge(int from, int to) {
	if (isWeighted) {
		auto it = weightedEdgeList.lower_bound(make_tuple(from, to, 0));
		if (it != weightedEdgeList.end() &&
			get<0>(*it) == from && get<1>(*it) == to)
			weightedEdgeList.erase(it);
		if (!isDirected) { // в неориентированном графе ребро может связывать вершины в обратном порядке
			it = weightedEdgeList.lower_bound(make_tuple(to, from, 0));
			if (it != weightedEdgeList.end() &&
				get<0>(*it) == to && get<1>(*it) == from)
				weightedEdgeList.erase(it);
		}
	}
	else {
		edgeList.erase(make_pair(from, to));
		if (!isDirected)
			edgeList.erase(make_pair(to, from));
	}
}

int EdgeList::getWeight(int from, int to) const
{
	if (!isWeighted)
		return 0;
	auto it = weightedEdgeList.lower_bound(make_tuple(from, to, 0));
	return (it != weightedEdgeList.end()) ? get<2>(*it) : 0;
}

int EdgeList::getAdjacent(int from) const
{
	if (isWeighted) {
		auto it = weightedEdgeList.lower_bound(make_tuple(from, 0, 0));
		return (it != weightedEdgeList.end()) ? get<1>(*it) : -1;
	} else {
		auto it = edgeList.lower_bound(make_pair(from, 0));
		return (it != edgeList.end()) ? it->second : -1;
	}
	// если не ориентирован, то может имеется в виду другой конец ребра
	if (isWeighted) {
		for (auto it = weightedEdgeList.begin(); it != weightedEdgeList.end(); ++it)
			if (get<1>(*it) == from) return get<0>(*it);
	}
	else {
		for (auto it = edgeList.begin(); it != edgeList.end(); ++it)
			if (it->second == from) return it->first;
	}
}

list<tuple<int, int, int>> EdgeList::getWeightedEdgesList() const {
	return list<tuple<int, int, int>>(weightedEdgeList.cbegin(), weightedEdgeList.cend());
}

tuple<int, int, int> EdgeList::findMinEdge(char * isMarked) const
{
	int minWeight = INT_MAX, minI = -1, minJ = -1;
	for (const auto & edge : weightedEdgeList) {
		int i = get<0>(edge);
		int j = get<1>(edge);
		if (isMarked[i] == isMarked[j]) continue;
		int weight = get<2>(edge);
		if (weight < minWeight) {
			minWeight = weight;
			minI = i;
			minJ = j;
		}
	}
	return make_tuple(minI, minJ, minWeight);
}

int EdgeList::getVertexDegree(int vertex) const
{
	if (isDirected)
		return getVertexInDegree(vertex) + getVertexOutDegree(vertex);
	int degree = 0;
	if (isWeighted)
		for (const auto & edge : weightedEdgeList) {
			int from = get<0>(edge);
			int to = get<1>(edge);
			if (from == vertex || to == vertex)
				++degree;
			if (from == vertex && to == vertex)
				++degree; // петля учитывается дважды
		}
	else for (const auto & edge : edgeList) {
			int from = edge.first;
			int to = edge.second;
			if (from == vertex || to == vertex)
				++degree;
			if (from == vertex && to == vertex)
				++degree; // петля учитывается дважды
		}
	return degree;
}

vector<int> EdgeList::getVerticesDegrees() const
{
	vector<int> degrees(vertexCount, 0);
	if (isWeighted)
		for (const auto & edge : weightedEdgeList) {
			int from = get<0>(edge);
			int to = get<1>(edge);
			++degrees[from];
			++degrees[to];
		}
	else for (const auto & edge : edgeList) {
		int from = edge.first;
		int to = edge.second;
		++degrees[from];
		++degrees[to];
	}
	return degrees;
}

int EdgeList::getVertexInDegree(int vertex) const
{
	if (!isDirected) return getVertexDegree(vertex);
	int inDegree = 0;
	if (isWeighted)
		for (const auto & edge : weightedEdgeList) {
			int to = get<1>(edge);
			if (to == vertex)
				++inDegree;
		}
	else for (const auto & edge : edgeList) {
		int to = edge.second;
		if (to == vertex)
			++inDegree;
	}
	return 0;
}

int EdgeList::getVertexOutDegree(int vertex) const
{
	if (!isDirected) return getVertexDegree(vertex);
	int outDegree = 0;
	if (isWeighted)
		for (const auto & edge : weightedEdgeList) {
			int from = get<0>(edge);
			if (from == vertex)
				++outDegree;
		}
	else for (const auto & edge : edgeList) {
		int from = edge.first;
		if (from == vertex)
			++outDegree;
	}
	return 0;
}

vector<int> EdgeList::getVerticesInDegrees() const
{
	vector<int> inDegrees(vertexCount, 0);
	if (isWeighted)
		for (const auto & edge : weightedEdgeList) {
			int to = get<1>(edge);
			++inDegrees[to];
		}
	else for (const auto & edge : edgeList) {
		int to = edge.second;
		++inDegrees[to];
	}
	return inDegrees;
}

vector<int> EdgeList::getVerticesOutDegrees() const
{
	vector<int> outDegrees(vertexCount, 0);
	if (isWeighted)
		for (const auto & edge : weightedEdgeList) {
			int from = get<0>(edge);
			++outDegrees[from];
		}
	else for (const auto & edge : edgeList) {
		int from = edge.first;
		++outDegrees[from];
	}
	return outDegrees;
}

DSU EdgeList::getUnityComponents() const
{
	DSU result(vertexCount);
	if (!isWeighted)
		for (const auto & edge : edgeList)
			result.unite(edge.first, edge.second);
	else 
		for (const auto & edge : weightedEdgeList)
			result.unite(get<0>(edge), get<1>(edge));
	return result;
}

DSU EdgeList::getUnityComponents(int exceptFrom, int exceptTo) const
{
	DSU result(vertexCount);
	if (!isWeighted)
		for (const auto & edge : edgeList) {
			int from = edge.first, to = edge.second;
			if (from == exceptFrom && to == exceptTo
				|| !isDirected && from == exceptTo && to == exceptFrom)
				continue;
			result.unite(edge.first, edge.second);
		}
	else
		for (const auto & edge : weightedEdgeList) {
			int from = get<0>(edge), to = get<1>(edge);
			if (from == exceptFrom && to == exceptTo
				|| !isDirected && from == exceptTo && to == exceptFrom)
				continue;
			result.unite(from, to);
		}
	return result;
}
