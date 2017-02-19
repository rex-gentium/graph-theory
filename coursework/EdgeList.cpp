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

GraphContent * EdgeList::getSpaingTreePrima() {
	EdgeList* result = new EdgeList();
	result->vertexCount = vertexCount;
	result->isWeighted = isWeighted;
	result->isDirected = isDirected;
	bool * isMarked = new bool[vertexCount];
	for (int i = 0; i < vertexCount; i++)
		isMarked[i] = false;

	isMarked[0] = true;
	bool hasUnmarked = (vertexCount == 1) ? false : true;

	while (hasUnmarked) {
		// ищем минимальное из ребёр, соединяющих помеченную вершину с непомеченной
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
		if (minI >= 0) { // нашли ребро
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

GraphContent * EdgeList::getSpaingTreeKruscal() {
	return nullptr;
}

GraphContent * EdgeList::getSpaingTreeBoruvka() {
	return nullptr;
}
