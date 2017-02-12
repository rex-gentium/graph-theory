#include "GraphTrasformer.h"
#include <tuple>

inline bool isValidPointers(GraphContent * given, GraphContent * dest) {
	return given != nullptr && dest != nullptr && given != dest;
}

inline void copyBaseData(GraphContent * given, GraphContent * dest) {
	dest->isDirected = given->isDirected;
	dest->isWeighted = given->isWeighted;
	dest->vertexCount = given->vertexCount;
}

bool GraphTrasformer::transferContent(AdjacencyMatrix * given, AdjacencyList * dest) {
	if (!isValidPointers(given, dest)) return false;
	copyBaseData(given, dest);
	(given->isWeighted)
		? dest->weightedAdjacencyList.resize(given->vertexCount)
		: dest->adjacencyList.resize(given->vertexCount);

	for (int from = 0; from < given->vertexCount; from++) {
		for (int to = 0; to < given->vertexCount; to++)
			if (given->adjacencyMatrix[from][to]) {
				if (given->isWeighted)
					dest->weightedAdjacencyList[from].insert(make_pair(to, given->adjacencyMatrix[from][to]));
				else
					dest->adjacencyList[from].insert(to);
			}
		given->adjacencyMatrix[from].clear();
	}
	given->adjacencyMatrix.clear();
	return true;
}

bool GraphTrasformer::transferContent(AdjacencyMatrix * given, EdgeList * dest) {
	if (!isValidPointers(given, dest)) return false;
	copyBaseData(given, dest);
	for (int from = 0; from < given->vertexCount; from++) {
		int to = (given->isDirected) ? 0 : from;
		for (to; to < given->vertexCount; to++)
			if (given->adjacencyMatrix[from][to]) {
				if (given->isWeighted)
					dest->weightedEdgeList.insert(make_tuple(from, to, given->adjacencyMatrix[from][to]));
				else
					dest->edgeList.insert(make_pair(from, to));
			}
		given->adjacencyMatrix[from].clear();
	}
	given->adjacencyMatrix.clear();
	return true;
}

bool GraphTrasformer::transferContent(AdjacencyList * given, AdjacencyMatrix * dest) {
	if (!isValidPointers(given, dest)) return false;
	copyBaseData(given, dest);
	dest->adjacencyMatrix.resize(given->vertexCount);
	
	if (given->isWeighted) {
		for (int vertex = 0; vertex < given->vertexCount; vertex++) {
			dest->adjacencyMatrix[vertex].resize(given->vertexCount, 0);
			for (const auto & adjacency : given->weightedAdjacencyList[vertex]) {
				int adjacent = adjacency.first;
				int weight = adjacency.second;
				dest->adjacencyMatrix[vertex][adjacent] = weight;
			}
			given->weightedAdjacencyList[vertex].clear();
		}
		given->weightedAdjacencyList.clear();
	}
	else {
		for (int vertex = 0; vertex < given->vertexCount; vertex++) {
			dest->adjacencyMatrix[vertex].resize(given->vertexCount, 0);
			for (const int & adjacent : given->adjacencyList[vertex])
				dest->adjacencyMatrix[vertex][adjacent] = 1;
			given->adjacencyList[vertex].clear();
		}
		given->adjacencyList.clear();
	}
	return true;
}

bool GraphTrasformer::transferContent(AdjacencyList * given, EdgeList * dest) {
	if (!isValidPointers(given, dest)) return false;
	copyBaseData(given, dest);

	if (given->isWeighted) {
		for (int from = 0; from < given->vertexCount; from++) {
			for (const auto & adjacency : given->weightedAdjacencyList[from]) {
				int to = adjacency.first;
				if (!given->isDirected && from > to) continue; // в неориентированном графе список смежности дублирует ребра
				int weight = adjacency.second;
				dest->weightedEdgeList.insert(make_tuple(from, to, weight));
			}
			given->weightedAdjacencyList[from].clear();
		}
		given->weightedAdjacencyList.clear();
	}
	else {
		for (int from = 0; from < given->vertexCount; from++) {
			for (const int & to : given->adjacencyList[from]) {
				if (!given->isDirected && from > to) continue; // в неориентированном графе список смежности дублирует ребра
				dest->edgeList.insert(make_pair(from, to));
			}
			given->adjacencyList[from].clear();
		}
		given->adjacencyList.clear();
	}
	return true;
}

bool GraphTrasformer::transferContent(EdgeList * given, AdjacencyMatrix * dest) {
	if (!isValidPointers(given, dest)) return false;
	copyBaseData(given, dest);
	dest->adjacencyMatrix.resize(given->vertexCount);
	for (int vertex = 0; vertex < given->vertexCount; vertex++)
		dest->adjacencyMatrix[vertex].resize(given->vertexCount, 0);
	
	if (given->isWeighted) {
		for (const auto & edge : given->weightedEdgeList) {
			int from = get<0>(edge);
			int to = get<1>(edge);
			int weight = get<2>(edge);
			dest->adjacencyMatrix[from][to] = weight;
			if (!given->isDirected)
				dest->adjacencyMatrix[to][from] = weight;
		}
		given->weightedEdgeList.clear();
	}
	else {
		for (const auto & edge : given->edgeList) {
			int from = edge.first;
			int to = edge.second;
			dest->adjacencyMatrix[from][to] = 1;
			if (!given->isDirected)
				dest->adjacencyMatrix[to][from] = 1;
		}
		given->edgeList.clear();
	}
	return true;
}

bool GraphTrasformer::transferContent(EdgeList * given, AdjacencyList * dest) {
	if (!isValidPointers(given, dest)) return false;
	copyBaseData(given, dest);
	
	if (given->isWeighted) {
		dest->weightedAdjacencyList.resize(given->vertexCount);
		for (auto & edge : given->weightedEdgeList) {
			int from = get<0>(edge);
			int to = get<1>(edge);
			int weight = get<2>(edge);
			dest->weightedAdjacencyList[from].insert(make_pair(to, weight));
			if (!given->isDirected)
				dest->weightedAdjacencyList[to].insert(make_pair(from, weight));
		}
	}
	else {
		dest->adjacencyList.resize(given->vertexCount);
		for (auto & edge : given->edgeList) {
			int from = get<0>(edge);
			int to = get<1>(edge);
			dest->adjacencyList[from].insert(to);
			if (!given->isDirected)
				dest->adjacencyList[to].insert(from);
		}
	}
	return true;
}
