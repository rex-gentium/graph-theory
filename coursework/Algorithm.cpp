#include "Algorithm.h"
#include "DSU.h"
#include <algorithm>
#include <tuple>
#include <map>
#include <stack>

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

bool Algorithm::checkEuler(const GraphContent * graph, bool & isCircleExists, int & tourStart)
{
	bool result = checkEulerDegrees(graph, isCircleExists, tourStart);
	if (result == true) {
		// число компонент [слабой] связности, имеющих рёбра, не должно быть больше 1
		DSU unityComponents = graph->getUnityComponents(); // O(v^2) for AdjMatrix, O(v*log(v)) for AdjList, O(e) for EdgeList
		if (unityComponents.getSetCount() == 1)
			return true;
		int connectedComponent = -1; // запомним ту компоненту, которой можно иметь ребра
		for (int v = 0; v < graph->vertexCount; ++v) {
			int leader = unityComponents.find(v);
			if (leader != v)
				// вершина не является лидером компоненты, значит связана ребрами с лидером
				if (connectedComponent < 0)
					connectedComponent = leader;
				else if (connectedComponent != leader)
					return false;
		}
	}
	return result;
}

vector<int> Algorithm::getEuleranTour(const GraphContent * graph)
{
	GraphContent * copy = graph->getCopy();
	stack<int> st;
	vector<int> tour;
	st.push(0);
	while(!st.empty()) {
		int v = st.top();
		if (copy->getVertexDegree(v) == 0) {
			tour.push_back(v);
			st.pop();
		}
		else {
			int w = copy->getAdjacent(v);
			copy->removeEdge(v, w);
			st.push(w);
		}
	}
	delete copy;
	std::reverse(tour.begin(), tour.end()); // O(e)
	return tour;
}

set<pair<int, int>>::iterator Algorithm::chooseNextEdgeFleri(EdgeList * graph, int from) {
	auto chosen = graph->edgeList.end();
	auto bridge = graph->edgeList.end();
	auto edge = graph->edgeList.lower_bound(make_pair(from, 0)); // log(e)
	while (edge != graph->edgeList.end() && edge->first == from) {
		int to = edge->second;
		if (graph->isBridge(from, to)) // O(e)
			bridge = edge;
		else {
			chosen = edge;
			break;
		}
		++edge;
	}
	if (chosen == graph->edgeList.end() && bridge == graph->edgeList.end() && !graph->isDirected) {
		// редкий случай только в неориентированном графе: искомое ребро было инвертировано
		for (auto & edge = graph->edgeList.begin(); edge != graph->edgeList.end() && edge->second == from; ++edge) {
			int to = edge->first;
			if (graph->isBridge(from, to)) // O(e)
				bridge = edge;
			else {
				chosen = edge;
				break;
			}
			++edge;
		}
	}
	if (chosen == graph->edgeList.end()) // мост выбирается в последнюю очередь
		chosen = bridge;
	return chosen;
}

set<tuple<int, int, int>>::iterator Algorithm::chooseNextWeightedEdgeFleri(EdgeList * graph, int from) {
	auto chosen = graph->weightedEdgeList.end();
	auto bridge = graph->weightedEdgeList.end();
	auto edge = graph->weightedEdgeList.lower_bound(make_tuple(from, 0, 0)); // log(e)
	while (edge != graph->weightedEdgeList.end() && get<0>(*edge) == from) {
		int to = get<1>(*edge);
		if (graph->isBridge(from, to)) // O(e)
			bridge = edge;
		else {
			chosen = edge;
			break;
		}
		++edge;
	}
	if (chosen == graph->weightedEdgeList.end() && bridge == graph->weightedEdgeList.end()) {
		// редкий случай, только в неориентированном графе
		for (auto & edge = graph->weightedEdgeList.begin(); edge != graph->weightedEdgeList.end() && get<1>(*edge) == from; ++edge) {
			int to = get<0>(*edge);
			if (graph->isBridge(from, to)) // O(e)
				bridge = edge;
			else {
				chosen = edge;
				break;
			}
			++edge;
		}
	}
	if (chosen == graph->weightedEdgeList.end()) // мост выбирается в последнюю очередь
		chosen = bridge;
	return chosen;
}

vector<int> Algorithm::getEuleranTourFleri(EdgeList * graph)
{
	vector<int> tour;
	bool isEuleranCircle;
	int tourStart;
	if (!checkEuler(graph, isEuleranCircle, tourStart)) // O(max(v,e))
		return tour;
	if (isEuleranCircle)
		tourStart = 0;
	tour.push_back(tourStart);
	int currentVertex = tourStart;

	if (!graph->isWeighted) {
		stack<pair<int, int>> removedEdges;
		while (graph->hasEdges()) {
			set<pair<int, int>>::iterator chosen = chooseNextEdgeFleri(graph, currentVertex);
			int nextVertex = chosen->second;
			tour.push_back(nextVertex);
			removedEdges.push(*chosen);
			graph->edgeList.erase(chosen);
			currentVertex = nextVertex;
		}
		while (!removedEdges.empty()) {
			pair<int, int> edge = removedEdges.top();
			graph->addEdge(edge.first, edge.second, 0);
			removedEdges.pop();
		}
	}
	else {
		stack<tuple<int, int, int>> removedEdges;
		while (graph->hasEdges()) {
			set<tuple<int, int, int>>::iterator chosen = chooseNextWeightedEdgeFleri(graph, currentVertex);
			int nextVertex = get<1>(*chosen);
			tour.push_back(nextVertex);
			removedEdges.push(*chosen);
			graph->weightedEdgeList.erase(chosen);
			currentVertex = nextVertex;
		}
		while (!removedEdges.empty()) {
			tuple<int, int, int> edge = removedEdges.top();
			graph->addEdge(get<0>(edge), get<1>(edge), get<2>(edge));
			removedEdges.pop();
		}
	}

	return tour;
}

bool Algorithm::checkEulerDegrees(const GraphContent * graph, bool & isCircleExists, int & tourStart)
{
	int tourFinish = tourStart = -1;
	if (!graph->isDirected) {
		// неорграф является эйлеровым, если в нём 0 или 2 вершины нечётной степени
		vector<int> degrees = graph->getVerticesDegrees(); // O(v^2) for AdjMatrix, O(v*log(v)) for AdjList, O(e) for EdgeList
		for (int v = 0; v < graph->vertexCount; ++v)
			if (degrees[v] % 2 != 0) {
				if (tourStart < 0)
					tourStart = v;
				else if (tourFinish < 0)
					tourFinish = v;
				else return false;
			}
	}
	else {
		// орграф является эйлеровым, если полустепени захода равны полустепеням исхода у всех вершин,
		// кроме, может быть, двух: одна должна иметь бОльшую (на 1) степень исхода, вторая бОльшую (на 1) степень захода
		vector<int> inDegrees = graph->getVerticesInDegrees(); // O(v^2) for AdjMatrix, AdjList, O(e) for EdgeList
		vector<int> outDegrees = graph->getVerticesOutDegrees(); // O(v^2) for AdjMatrix, O(v) for AdjList, O(e) for EdgeList
		for (int v = 0; v < graph->vertexCount; ++v) {
			int sub = inDegrees[v] - outDegrees[v];
			switch (sub) {
			case -1:
				if (tourStart >= 0) return false;
				else tourStart = v;
				break;
			case 0: break;
			case 1:
				if (tourFinish >= 0) return false;
				else tourFinish = v;
				break;
			default: return false;
			}
		}
	}
	// в графе есть эйлеров цикл, если все вершины чётной степени
	isCircleExists = (tourStart < 0 && tourFinish < 0);
	// в графе есть эйлеров путь, если есть 2 вышеуказанных вершины
	if (tourStart < 0 != tourFinish < 0) // tourStartExists XOR tourFinishExists
		return false;
	return true;
}
