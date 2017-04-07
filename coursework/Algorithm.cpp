#include "Algorithm.h"
#include "DSU.h"
#include <tuple>
#include <map>

GraphContent * Algorithm::getSpaingTreePrima(const GraphContent * graph) {
	AdjacencyMatrix* result = new AdjacencyMatrix(graph->vertexCount);
	result->isDirected = graph->isDirected;
	result->isWeighted = graph->isWeighted;
	bool * isMarked = new bool[graph->vertexCount];
	for (int i = 0; i < graph->vertexCount; i++)
		isMarked[i] = false;

	isMarked[0] = true;
	bool hasUnmarked = (graph->vertexCount == 1) ? false : true;
	while (hasUnmarked) {
		// ищем минимальное из ребёр, соединяющих помеченную вершину с непомеченной
		tuple<int, int, int> edge = graph->findMinEdge(isMarked);
		int from = get<0>(edge), to = get<1>(edge), weight = get<2>(edge);
		if (from >= 0 && to >= 0) {
			result->addEdge(from, to, weight);
			isMarked[from] = isMarked[to] = true;
		}
		else {
			// если не нашли никакого ребра, помечаем любую недостижимую вершину
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

/* компаратор ребёр по весам */
bool compareWeight(const tuple<int, int, int> leftEdge, const tuple<int, int, int> rightEdge) {
	return get<2>(leftEdge) < get<2>(rightEdge);
}

GraphContent * Algorithm::getSpaingTreeKruscal(const GraphContent * graph) {
	EdgeList* result = new EdgeList();
	result->isDirected = graph->isDirected;
	result->isWeighted = graph->isWeighted;
	result->vertexCount = graph->vertexCount;
	
	list<tuple<int, int, int>> edges = graph->getWeightedEdgesList(); // O(e*v) for adjlist, O(v^2) for adjmatrix, O(e) for edgelist
	// сортировка рёбер e*log(e)
	edges.sort(compareWeight);
	// распределение по компонентам связности
	DSU components(graph->vertexCount);
	while (!edges.empty()) {
		auto edge = edges.front();
		int from = get<0>(edge), to = get<1>(edge), weight = get<2>(edge);
		int leftComponent = components.find(from);
		int rightComponent = components.find(to);
		if (leftComponent != rightComponent) {
			// добавление ребра не образует цикл
			result->addEdge(to, from, weight);
			components.unite(leftComponent, rightComponent);
		}
		// если добавление ребра образует цикл, оно непригодно для постройки остова
		edges.pop_front();
	}
	return result;
}

GraphContent * Algorithm::getSpaingTreeBoruvka(const GraphContent * graph)
{
	EdgeList * result = new EdgeList;
	result->isDirected = graph->isDirected;
	result->isWeighted = graph->isWeighted;
	result->vertexCount = graph->vertexCount;

	DSU components(graph->vertexCount);
	auto edges = graph->getWeightedEdgesList();
	map<int, tuple<int, int, int>*> edgesToAdd;
	
	while (components.getSetCount() > 1) {
		// на каждой итерации будем добавлять к каждой из компонент по ребру
		// ищем среди ребер минимальные, связующие разные компоненты
		for (auto & edge : edges) {
			int from = get<0>(edge), to = get<1>(edge), weight = get<2>(edge);
			int fromComponent = components.find(from);
			int toComponent = components.find(to);
			if (fromComponent != toComponent) {
				int currWeight = (edgesToAdd.find(fromComponent) != edgesToAdd.end()) ? get<2>(*edgesToAdd[fromComponent]) : INT_MAX;
				if (weight < currWeight)
					edgesToAdd[fromComponent] = &edge;
				currWeight = (edgesToAdd.find(toComponent) != edgesToAdd.end()) ? get<2>(*edgesToAdd[toComponent]) : INT_MAX;
				if (weight < currWeight)
					edgesToAdd[toComponent] = &edge;
			}
		}
		bool didEdgeAdding = false;
		for (auto & edge : edgesToAdd) {
			int from = get<0>(*edge.second), to = get<1>(*edge.second), weight = get<2>(*edge.second);
			//if (from < 0 || to < 0) continue;
			result->addEdge(from, to, weight);
			components.unite(from, to);
			didEdgeAdding = true;
		}
		if (!didEdgeAdding)
			break; // случай, когда в остове несколько компонент связности
		edgesToAdd.clear();
	}
	return result;
}
