﻿#include "EdgeList.h"
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

list<tuple<int, int, int>> EdgeList::getWeightedEdgesList() const {
	return list<tuple<int, int, int>>(weightedEdgeList.cbegin(), weightedEdgeList.cend());
}

tuple<int, int, int> EdgeList::findMinEdge(bool * isMarked) const
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

GraphContent::ConstEdgeIterator& EdgeList::edgeBegin() const
{
	if (isWeighted)
		return (!weightedEdgeList.empty()) 
			? *new EdgeListEdgeIterator(this, EdgeListEdgeIterator::InnerWeightedIterator(weightedEdgeList.cbegin()))
			: edgeEnd();
	else
		return (!edgeList.empty())
			? *new EdgeListEdgeIterator(this, EdgeListEdgeIterator::InnerIterator(edgeList.cbegin()))
			: edgeEnd();
}

GraphContent::ConstEdgeIterator& EdgeList::edgeEnd() const
{
	return (isWeighted) 
		? *new EdgeListEdgeIterator(this, EdgeListEdgeIterator::InnerWeightedIterator::null())
		: *new EdgeListEdgeIterator(this, EdgeListEdgeIterator::InnerIterator::null());
}

tuple<int, int, int> EdgeList::nextEdge(ConstEdgeIterator & iterator) const
{
	EdgeListEdgeIterator edgeIt = dynamic_cast<EdgeListEdgeIterator&>(iterator);
	EdgeListEdgeIterator::InnerIterator innerIt = edgeIt.inner();
	if (!innerIt.isNull())
		return nextEdge(innerIt);
	EdgeListEdgeIterator::InnerWeightedIterator innerWIt = edgeIt.innerWeighted();
	if (!innerWIt.isNull())
		return nextEdge(innerWIt);
	return make_tuple(-1, -1, -1);
}

tuple<int, int, int> EdgeList::nextEdge(EdgeListEdgeIterator::InnerIterator & iterator) const
{
	// спускаемся дальше по списку ребер
	if (++iterator != edgeList.cend())
		return make_tuple(iterator->first, iterator->second, -1);
	// список закончился
	iterator = iterator.null();
	return make_tuple(-1, -1, -1);
}

tuple<int, int, int> EdgeList::nextEdge(EdgeListEdgeIterator::InnerWeightedIterator & iterator) const
{
	// спускаемся дальше по списку ребер
	if (++iterator != weightedEdgeList.cend())
		return *iterator;
	// список закончился
	iterator = iterator.null();
	return make_tuple(-1, -1, -1);
}

EdgeList::EdgeListEdgeIterator::EdgeListEdgeIterator(const EdgeList * graph, InnerIterator it) :
	ConstEdgeIterator(graph,
		(!it.isNull()) ? it->first : -1,
		(!it.isNull()) ? it->second : -1),
	edgeList(graph),
	it(it),
	wIt(InnerWeightedIterator::null())
{}

EdgeList::EdgeListEdgeIterator::EdgeListEdgeIterator(const EdgeList * graph, InnerWeightedIterator it) :
	ConstEdgeIterator(graph,
		(!it.isNull()) ? get<0>(*it) : -1,
		(!it.isNull()) ? get<1>(*it) : -1,
		(!it.isNull()) ? get<2>(*it) : -1),
	edgeList(graph),
	it(InnerIterator::null()),
	wIt(it)
{}

/*void EdgeList::EdgeListEdgeIterator::operator++()
{
	if (*this != graph->edgeEnd())
		edge = (graph->isWeighted)
			? edgeList->nextEdge(wIt)
			: edgeList->nextEdge(it);
}*/
