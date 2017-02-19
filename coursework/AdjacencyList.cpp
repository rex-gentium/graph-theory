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

GraphContent * AdjacencyList::getSpaingTreePrima() {
	AdjacencyList* result = new AdjacencyList(vertexCount, isWeighted, isDirected);
	bool * isMarked = new bool[vertexCount];
	for (int i = 0; i < vertexCount; i++)
		isMarked[i] = false;

	isMarked[0] = true;
	bool hasUnmarked = (vertexCount == 1) ? false : true;

	while (hasUnmarked) {
		// ищем минимальное из ребёр, соединяющих помеченную вершину i с непомеченной j
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

		if (minI >= 0) {
			// нашли ребро
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

GraphContent * AdjacencyList::getSpaingTreeKruscal() {
	return nullptr;
}

GraphContent * AdjacencyList::getSpaingTreeBoruvka() {
	return nullptr;
}
