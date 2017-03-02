﻿#include "Algorithm.h"
#include "DSU.h"
#include <tuple>

AdjacencyMatrix * Algorithm::getSpaingTreePrima(const AdjacencyMatrix * graph) {
	AdjacencyMatrix* result = new AdjacencyMatrix(graph->vertexCount);
	result->isDirected = graph->isDirected;
	result->isWeighted = graph->isWeighted;
	bool * isMarked = new bool[graph->vertexCount];
	for (int i = 0; i < graph->vertexCount; i++)
		isMarked[i] = false;

	isMarked[0] = true;
	bool hasUnmarked = (graph->vertexCount == 1) ? false : true;

	while (hasUnmarked) {
		// ищем минимальное из ребёр, соединяющих помеченную вершину i с непомеченной j
		int minWeight = INT_MAX, minI = -1, minJ = -1;
		for (int i = 0; i < graph->vertexCount; i++) {
			if (!isMarked[i]) continue;
			for (int j = i + 1; j < graph->vertexCount; j++) {
				if (isMarked[j]) continue;
				if (graph->adjacencyMatrix[i][j] != 0 && graph->adjacencyMatrix[i][j] < minWeight) {
					minWeight = graph->adjacencyMatrix[i][j];
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
			for (int i = 0; i < graph->vertexCount; i++)
				if (!isMarked[i]) {
					isMarked[i] = true;
					break;
				};
		}
		// смотрим, есть ли ещё непомеченные
		hasUnmarked = false;
		for (int i = 0; i < graph->vertexCount; i++)
			if (!isMarked[i]) {
				hasUnmarked = true;
				break;
			}
	}

	delete[] isMarked;
	return result;
}

AdjacencyList * Algorithm::getSpaingTreePrima(const AdjacencyList * graph) {
	AdjacencyList* result = new AdjacencyList(graph->vertexCount, graph->isWeighted, graph->isDirected);
	bool * isMarked = new bool[graph->vertexCount];
	for (int i = 0; i < graph->vertexCount; i++)
		isMarked[i] = false;

	isMarked[0] = true;
	bool hasUnmarked = (graph->vertexCount == 1) ? false : true;

	while (hasUnmarked) {
		// ищем минимальное из ребёр, соединяющих помеченную вершину i с непомеченной j
		int minWeight = INT_MAX, minI = -1, minJ = -1;
		for (int i = 0; i < graph->vertexCount; i++) {
			if (!isMarked[i]) continue;
			for (const auto & adjacency : graph->weightedAdjacencyList[i]) {
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

		if (minI >= 0) {
			// нашли ребро
			result->addEdge(minI, minJ, minWeight);
			isMarked[minI] = isMarked[minJ] = true;
		}
		else { // если не нашли никакого ребра, помечаем любую недостижимую вершину
			for (int i = 0; i < graph->vertexCount; i++)
				if (!isMarked[i]) {
					isMarked[i] = true;
					break;
				};
		}
		// смотрим, есть ли ещё непомеченные
		hasUnmarked = false;
		for (int i = 0; i < graph->vertexCount; i++)
			if (!isMarked[i]) {
				hasUnmarked = true;
				break;
			}
	}

	delete[] isMarked;
	return result;
}

EdgeList * Algorithm::getSpaingTreePrima(const EdgeList * graph) {
	EdgeList* result = new EdgeList();
	result->vertexCount = graph->vertexCount;
	result->isWeighted = graph->isWeighted;
	result->isDirected = graph->isDirected;
	bool * isMarked = new bool[graph->vertexCount];
	for (int i = 0; i < graph->vertexCount; i++)
		isMarked[i] = false;

	isMarked[0] = true;
	bool hasUnmarked = (graph->vertexCount == 1) ? false : true;

	while (hasUnmarked) {
		// ищем минимальное из ребёр, соединяющих помеченную вершину с непомеченной
		int minWeight = INT_MAX, minI = -1, minJ = -1;
		for (const auto & edge : graph->weightedEdgeList) {
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
			for (int i = 0; i < graph->vertexCount; i++)
				if (!isMarked[i]) {
					isMarked[i] = true;
					break;
				};
		}
		// смотрим, есть ли ещё непомеченные
		hasUnmarked = false;
		for (int i = 0; i < graph->vertexCount; i++)
			if (!isMarked[i]) {
				hasUnmarked = true;
				break;
			}
	}

	delete[] isMarked;
	return result;
}

bool compareWeight(const tuple<int, int, int> leftEdge, const tuple<int, int, int> rightEdge) {
	return get<2>(leftEdge) < get<2>(rightEdge);
}

EdgeList * Algorithm::getSpaingTreeKruscal(const GraphContent * graph) {
	EdgeList* result = new EdgeList();
	result->isDirected = graph->isDirected;
	result->isWeighted = graph->isWeighted;
	result->vertexCount = graph->vertexCount;
	
	list<tuple<int, int, int>> edges = graph->getWeightedEdgesList(); // O(e*v) for adjlist, O(v^2) for adjmatrix, O(e) for edgelist
	// сортировка рёбер e*log(e)
	edges.sort(compareWeight);
	// распределение по компонентам связности
	DSU unityComponents(graph->vertexCount);
	while (!edges.empty()) {
		auto edge = edges.front();
		int from = get<0>(edge);
		int to = get<1>(edge);
		int weight = get<2>(edge);
		int leftUnityComponent = unityComponents.find(from);
		int rightUnityComponent = unityComponents.find(to);
		if (leftUnityComponent != rightUnityComponent) {
			// добавление ребра не образует цикл
			result->addEdge(to, from, weight);
			unityComponents.unite(leftUnityComponent, rightUnityComponent);
		}
		// если добавление ребра образует цикл, оно непригодно для постройки остова
		edges.pop_front();
	}
	return result;
}

EdgeList * Algorithm::getSpaingTreeBoruvka(const GraphContent * graph)
{
	EdgeList * result = new EdgeList;
	result->isDirected = graph->isDirected;
	result->isWeighted = graph->isWeighted;
	result->vertexCount = graph->vertexCount;

	DSU unityComponents(graph->vertexCount);
	auto edges = graph->getWeightedEdgesList();
	vector<tuple<int, int, int>> edgesToAdd;
	
	while (unityComponents.getSetCount() > 1) {
		// на каждой итерации будем добавлять к каждой из компонент по ребру
		edgesToAdd.resize(graph->vertexCount, make_tuple(-1, -1, INT_MAX));
		// для ищем среди ребер минимальные, связующие разные компоненты
		for (auto & edge : edges) {
			int from = get<0>(edge);
			int to = get<1>(edge);
			int weight = get<2>(edge);
			int fromComponent = unityComponents.find(from);
			int toComponent = unityComponents.find(to);
			if (fromComponent != toComponent) {
				if (weight < get<2>(edgesToAdd[fromComponent]))
					edgesToAdd[fromComponent] = make_tuple(from, to, weight);
				if (weight < get<2>(edgesToAdd[toComponent]))
					edgesToAdd[toComponent] = make_tuple(from, to, weight);
			}
		}
		bool didEdgeAdding = false;
		for (auto & edge : edgesToAdd) {
			int from = get<0>(edge), to = get<1>(edge), weight = get<2>(edge);
			if (from < 0 || to < 0) continue;
			result->addEdge(from, to, weight);
			unityComponents.unite(from, to);
			didEdgeAdding = true;
		}
		if (!didEdgeAdding)
			break; // случай, когда в остове несколько компонент связности
		edgesToAdd.clear();
	}
	return result;
}
