#include "Algorithm.h"
#include "DSU.h"
#include <tuple>

GraphContent * Algorithm::getSpaingTreePrima(const GraphContent * graph) {
	AdjacencyMatrix* result = new AdjacencyMatrix(graph->vertexCount);
	result->isDirected = graph->isDirected;
	result->isWeighted = graph->isWeighted;
	bool * isMarked = new bool[graph->vertexCount];
	for (int i = 0; i < graph->vertexCount; i++)
		isMarked[i] = false;

	isMarked[0] = true;
	bool hasUnmarked = (graph->vertexCount == 1) ? false : true;
	auto edges = graph->getWeightedEdgesList();
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

GraphContent * Algorithm::getSpaingTreeBoruvka(const GraphContent * graph)
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

bool Algorithm::checkEuler(const GraphContent * graph, bool & isCircleExists, int & tourStart)
{
	bool result = checkEulerDegrees(graph, isCircleExists, tourStart);
	if (result == true) {
		// число компонент [слабой] связности, имеющих рёбра, не должно быть больше 1
		DSU unityComponents = graph->getUnityComponents();
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

bool Algorithm::checkEulerDegrees(const GraphContent * graph, bool & isCircleExists, int & tourStart)
{
	int tourFinish = tourStart = -1;
	if (graph->isDirected) {
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
		int tourFinish = tourStart = -1;
		vector<int> inDegrees = graph->getVerticesInDegrees(); // O(v^2) for AdjMatrix, AdjList, O(e) for EdgeList
		vector<int> outDegrees = graph->getVerticesOutDegrees(); // O(v^2) for AdjMatrix, O(v) for AdjList, O(e) for EdgeList
		for (int v = 0; v < graph->vertexCount; ++v) {
			int sub = inDegrees[v] - outDegrees[v];
			switch (sub) {
			case 1:
				if (tourStart >= 0) return false;
				else tourStart == v;
				break;
			case 0: break;
			case -1:
				if (tourFinish >= 0) return false;
				else tourFinish == v;
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
}
