#include "Algorithm.h"
#include "DSU.h"
#include <tuple>
#include <map>

GraphContent * Algorithm::getSpaingTreePrima(const GraphContent * graph) {
	GraphContent * result = new EdgeList;
	result->vertexCount = graph->vertexCount;
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

/* компаратор ребёр по весам */
struct compareWeight {
	bool operator() (const tuple<int, int, int> & leftEdge, const tuple<int, int, int> & rightEdge) const {
		// сначала по весам
		int leftWeight = get<2>(leftEdge), rightWeight = get<2>(rightEdge);
		if (leftWeight != rightWeight)
			return leftWeight < rightWeight;
		else { // потом лексигорафически
			int leftFrom = get<0>(leftEdge), rightFrom = get<0>(rightEdge);
			if (leftFrom != rightFrom)
				return leftFrom < rightFrom;
			else {
				int leftTo = get<1>(leftEdge), rightTo = get<1>(rightEdge);
				return leftTo < rightTo;
			}
		}
	}
};

GraphContent * Algorithm::getSpaingTreeKruscal(EdgeList * graph) {
	EdgeList* result = new EdgeList();
	result->isDirected = graph->isDirected;
	result->isWeighted = graph->isWeighted;
	result->vertexCount = graph->vertexCount;
	// сортировка рёбер e*log(e)
	set<tuple<int, int, int>, compareWeight> sortedEdges;
	while (!graph->weightedEdgeList.empty()) {
		auto edge = graph->weightedEdgeList.begin();
		sortedEdges.insert(*edge);
		graph->weightedEdgeList.erase(edge);
	}
	// распределение по компонентам связности
	DSU components(graph->vertexCount);
	while (!sortedEdges.empty()) {
		auto edge = *sortedEdges.begin();
		int from = get<0>(edge), to = get<1>(edge), weight = get<2>(edge);
		int leftComponent = components.find(from);
		int rightComponent = components.find(to);
		if (leftComponent != rightComponent) {
			// добавление ребра не образует цикл
			result->addEdge(from, to, weight);
			components.unite(leftComponent, rightComponent);
		}
		graph->weightedEdgeList.insert(edge);
		sortedEdges.erase(edge);
	}
	return result;
}

GraphContent * Algorithm::getSpaingTreeBoruvka(const EdgeList * graph)
{
	EdgeList * result = new EdgeList;
	result->isDirected = graph->isDirected;
	result->isWeighted = graph->isWeighted;
	result->vertexCount = graph->vertexCount;

	DSU components(graph->vertexCount);
	map<int, tuple<int, int, int>> edgesToAdd; // к какой компоненте связности какое ребро пришивать
	
	while (components.getSetCount() > 1) {
		// на каждой итерации будем добавлять к каждой из компонент по ребру
		// ищем среди ребер минимальные, связующие разные компоненты
		for (auto edge = graph->weightedEdgeList.begin(); edge != graph->weightedEdgeList.end(); ++edge) {
			int from = get<0>(*edge), to = get<1>(*edge), weight = get<2>(*edge);
			int fromComponent = components.find(from);
			int toComponent = components.find(to);
			if (fromComponent != toComponent) {
				int currWeight = (edgesToAdd.find(fromComponent) != edgesToAdd.end()) ? get<2>(edgesToAdd[fromComponent]) : INT_MAX;
				if (weight < currWeight)
					edgesToAdd[fromComponent] = *edge; // !
				currWeight = (edgesToAdd.find(toComponent) != edgesToAdd.end()) ? get<2>(edgesToAdd[toComponent]) : INT_MAX;
				if (weight < currWeight)
					edgesToAdd[toComponent] = *edge;
			}
		}
		bool didEdgeAdding = false;
		for (auto & edge : edgesToAdd) {
			int from = get<0>(edge.second), to = get<1>(edge.second), weight = get<2>(edge.second);
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
