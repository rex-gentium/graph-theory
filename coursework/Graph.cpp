#include "Graph.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>

Graph::Graph() {}

Graph::~Graph() {}

void Graph::readGraph(const string fileName) {
	ifstream inpFile;
	inpFile.open(fileName);
	char mode;
	mode = inpFile.get();
	switch (mode) {
		case 'C' : // adjacency matrix input
			this->readGraphAdjMatrix(inpFile);
			break;
		case 'L' : // adjacency list input
			this->readGraphAdjList(inpFile);
			break;
		case 'E' : // edge list input
			this->readGraphEdgeList(inpFile);
			break;
	}
	inpFile.close();
}

void Graph::addEdge(int from, int to, int weight) {
	--from;	// поправка на нумерацию с нуля
	--to;
	switch (this->graphForm) {
	case RepresentationType::ADJMATRIX:
		return this->addEdgeAdjMatrix(from, to, weight);
	case RepresentationType::ADJLIST:
		return this->addEdgeAdjList(from, to, weight);
	case RepresentationType::EDGELIST:
		return this->addEdgeEdgeList(from, to, weight);
	}
}

void Graph::removeEdge(int from, int to) {
	--from;	// поправка на нумерацию с нуля
	--to;
	switch (this->graphForm) {
	case RepresentationType::ADJMATRIX:
		return this->removeEdgeAdjMatrix(from, to);
	case RepresentationType::ADJLIST:
		return this->removeEdgeAdjList(from, to);
	case RepresentationType::EDGELIST:
		return this->removeEdgeEdgeList(from, to);
	}
}

int Graph::changeEdge(int from, int to, int newWeight) {
	if (!weighted) return 0; // бессмысленная операция
	--from;	// поправка на нумерацию с нуля
	--to;
	switch (this->graphForm) {
	case RepresentationType::ADJMATRIX:
		return this->changeEdgeAdjMatrix(from, to, newWeight);
	case RepresentationType::ADJLIST:
		return this->changeEdgeAdjList(from, to, newWeight);
	case RepresentationType::EDGELIST:
		return this->changeEdgeEdgeList(from, to, newWeight);
	}
	return 0;
}

void Graph::transformToAdjList() {
	switch (this->graphForm) {
	case RepresentationType::ADJLIST: 
		return;
	case RepresentationType::ADJMATRIX:
		this->transformAdjMatrixToAdjList();
		break;
	case RepresentationType::EDGELIST:
		this->transformEdgeListToAdjList();
		break;
	}
	graphForm = RepresentationType::ADJLIST;
}

void Graph::transformToAdjMatrix() {
	switch (this->graphForm) {
	case RepresentationType::ADJMATRIX: 
		return;
	case RepresentationType::ADJLIST:
		this->transformAdjListToAdjMatrix();
		break;
	case RepresentationType::EDGELIST:
		this->transformEdgeListToAdjList();
		break;
	}
	graphForm = RepresentationType::ADJMATRIX;
}

void Graph::transformToListOfEdges() {
	switch (this->graphForm) {
	case RepresentationType::EDGELIST: 
		return;
	case RepresentationType::ADJMATRIX:
		this->transformAdjMatrixToEdgeList();
		break;
	case RepresentationType::ADJLIST:
		this->transformAdjListToEdgeList();
		break;
	}
	graphForm = RepresentationType::EDGELIST;
}

void Graph::writeGraph(const string fileName) {
	ofstream outFile;
	outFile.open(fileName);
	switch (this->graphForm) {
	case RepresentationType::ADJMATRIX: 
		this->writeGraphAdjMatrix(outFile);
		break;
	case RepresentationType::ADJLIST:
		this->writeGraphAdjList(outFile);
		break;
	case RepresentationType::EDGELIST:
		this->writeGraphEdgeList(outFile);
		break;
	}
	outFile.close();
}

void Graph::readGraphAdjMatrix(ifstream & inpFile) {
	int weightFlag;
	inpFile >> this->vertexCount;
	inpFile >> weightFlag;
	this->weighted = (weightFlag > 0);
	adjacencyMatrix.resize(vertexCount);
	for (int from = 0; from < vertexCount; from++) {
		adjacencyMatrix[from].resize(vertexCount);
		for (int to = 0; to < vertexCount; to++)
			inpFile >> adjacencyMatrix[from][to];
	}
	this->graphForm = RepresentationType::ADJMATRIX;
	// проверка на ориентированность
	this->directed = false;
	for (int from = 0; from < vertexCount; from++)
		for (int to = from; to < vertexCount; to++)
			if (adjacencyMatrix[from][to] != 0 || // наличие петли — достаточное условие
				adjacencyMatrix[from][to] != adjacencyMatrix[to][from]) { // несимметричность — достаточное условие
				this->directed = true;
				return; // этот цикл всегда должен стоять последним в методе
			}
}

void Graph::readGraphAdjList(ifstream & inpFile) {
	int orientationFlag, weightFlag;
	inpFile >> this->vertexCount;
	inpFile >> orientationFlag;
	this->directed = (orientationFlag > 0);
	inpFile >> weightFlag;
	this->weighted = (weightFlag > 0);
	string line;
	getline(inpFile, line); // force jump to next line
	if (weighted) {
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
	this->graphForm = RepresentationType::ADJLIST;
}

void Graph::readGraphEdgeList(ifstream & inpFile) {
	int orientationFlag, weightFlag;
	inpFile >> this->vertexCount;
	int edgeCount;
	inpFile >> edgeCount;
	inpFile >> orientationFlag;
	this->directed = (orientationFlag > 0);
	inpFile >> weightFlag;
	this->weighted = (weightFlag > 0);
	for (int edge = 0; edge < edgeCount; edge++) {
		int from, to;
		inpFile >> from;
		inpFile >> to;
		--from;
		--to; // поправка на нумерацию с нуля
		if (weighted) {
			int weight;
			inpFile >> weight;
			this->weightedEdgeList.insert(make_tuple(from, to, weight));
		}
		else
			this->edgeList.insert(make_pair(from, to));
	}
	this->graphForm = RepresentationType::EDGELIST;
}

void Graph::writeGraphAdjMatrix(ofstream & outFile) {
	outFile << 'C' << ' ' << vertexCount << ' ' << ((weighted) ? 1 : 0) << '\n';
	for (int from = 0; from < vertexCount; from++)
		for (int to = 0; to < vertexCount; to++)
			outFile << adjacencyMatrix[from][to] << ((to == vertexCount - 1) ? '\n' : ' ');
}

void Graph::writeGraphAdjList(ofstream & outFile) {
	outFile << 'L' << ' ' << vertexCount << '\n'
		<< ((directed) ? 1 : 0) << ' ' << ((weighted) ? 1 : 0) << '\n';
	if (weighted) {
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

void Graph::writeGraphEdgeList(ofstream & outFile) {
	outFile << 'E' << ' ' << vertexCount << ' '
		<< ((weighted) ? weightedEdgeList.size() : edgeList.size()) << '\n'
		<< ((directed) ? 1 : 0) << ' ' << ((weighted) ? 1 : 0) << '\n';
	if (weighted)
		for (const auto & edge : weightedEdgeList)
			outFile << get<0>(edge) + 1 << ' ' << get<1>(edge) + 1 << ' ' << get<2>(edge) << '\n';
	else
		for (const auto & edge : edgeList)
			outFile << edge.first + 1 << ' ' << edge.second + 1 << '\n';
}

void Graph::addEdgeAdjMatrix(int from, int to, int weight) {
	adjacencyMatrix[from][to] = (weighted) ? weight : 1;
	if (!directed) // поддержка симметрии матрицы в неориентированном графе
		adjacencyMatrix[to][from] = adjacencyMatrix[from][to];
}

void Graph::addEdgeAdjList(int from, int to, int weight) {
	if (weighted) {
		weightedAdjacencyList[from].insert(make_pair(to, weight));
		if (!directed) // неориентированный граф дублирует смежности
			weightedAdjacencyList[to].insert(make_pair(from, weight));
	}
	else {
		adjacencyList[from].insert(to);
		if (!directed) // неориентированный граф дублирует смежности
			adjacencyList[to].insert(from);
	}
}

void Graph::addEdgeEdgeList(int from, int to, int weight) {
	if (weighted)
		weightedEdgeList.insert(make_tuple(from, to, weight));
	else
		edgeList.insert(make_pair(from, to));
}

int Graph::changeEdgeAdjMatrix(int from, int to, int newWeight) {
	int oldWeight = adjacencyMatrix[from][to];
	adjacencyMatrix[from][to] = newWeight;
	if (!directed) // поддержка симметрии матрицы неориентированного графа
		adjacencyMatrix[to][from] = newWeight;
	return oldWeight;
}

int Graph::changeEdgeAdjList(int from, int to, int newWeight) {
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
	if (!directed) {
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

int Graph::changeEdgeEdgeList(int from, int to, int newWeight) {
	int oldWeight = 0;
	auto it = weightedEdgeList.lower_bound(make_tuple(from, to, 0));
	if (it != weightedEdgeList.end() && 
		get<0>(*it) == from && get<1>(*it) == to) 
	{
		oldWeight = get<2>(*it);
		auto insertionHint = it;
		++insertionHint;
		weightedEdgeList.erase(it);
		weightedEdgeList.insert(insertionHint, make_tuple(from, to, newWeight));
	}
	if (!directed) { // в неориентированном графе ребро может связывать вершины в обратном порядке
		it = weightedEdgeList.lower_bound(make_tuple(to, from, 0));
		if (it != weightedEdgeList.end() && 
			get<0>(*it) == to && get<1>(*it) == from) 
		{
			oldWeight = get<2>(*it);
			auto insertionHint = it;
			++insertionHint;
			weightedEdgeList.erase(it);
			weightedEdgeList.insert(insertionHint, make_tuple(to, from, newWeight));
		}
	}
	return oldWeight;
}

void Graph::removeEdgeAdjMatrix(int from, int to) {
	adjacencyMatrix[from][to] = 0;
	if (!directed) // поддержка симметрии матрицы неориентированного графа
		adjacencyMatrix[to][from] = 0;
}

void Graph::removeEdgeAdjList(int from, int to) {
	if (weighted) {
		set<pair<int, int>> * vertexAdjList = &(weightedAdjacencyList[from]);
		// вес не имеет значения, сравнение происходит по первому элементу пары
		auto it = vertexAdjList->lower_bound(make_pair(to, 0));
		if (it != vertexAdjList->end())
			vertexAdjList->erase(it);
		if (!directed) { // в неориентированном графе списки дублируются
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
		if (!directed) { // в неориентированном графе списки дублируются
			vertexAdjList = &(adjacencyList[to]);
			it = vertexAdjList->find(from);
			if (it != vertexAdjList->end())
				vertexAdjList->erase(it);
		}
	}
}

void Graph::removeEdgeEdgeList(int from, int to) {
	if (weighted) {
		auto it = weightedEdgeList.lower_bound(make_tuple(from, to, 0));
		if (it != weightedEdgeList.end() && 
			get<0>(*it) == from && get<1>(*it) == to)
			weightedEdgeList.erase(it);
		if (!directed) { // в неориентированном графе ребро может связывать вершины в обратном порядке
			it = weightedEdgeList.lower_bound(make_tuple(to, from, 0));
			if (it != weightedEdgeList.end() && 
				get<0>(*it) == to && get<1>(*it) == from)
				weightedEdgeList.erase(it);
		}
	}
	else {
		edgeList.erase(make_pair(from, to));
		if (!directed)
			edgeList.erase(make_pair(to, from));
	}
}

void Graph::transformAdjListToAdjMatrix() {
	adjacencyMatrix.resize(vertexCount);
	if (weighted) {
		for (int i = 0; i < vertexCount; i++) {
			adjacencyMatrix[i].resize(vertexCount, 0);
			for (const auto & adjacency : weightedAdjacencyList[i]) {
				int j = adjacency.first;
				int weight = adjacency.second;
				adjacencyMatrix[i][j] = weight;
			}
			weightedAdjacencyList[i].clear();
		}
		weightedAdjacencyList.clear();
	}
	else {
		for (int i = 0; i < vertexCount; i++) {
			adjacencyMatrix[i].resize(vertexCount, 0);
			for (const int & j : adjacencyList[i])
				adjacencyMatrix[i][j] = 1;
			adjacencyList[i].clear();
		}
		adjacencyList.clear();
	}
}

void Graph::transformEdgeListToAdjMatrix() {
	adjacencyMatrix.resize(vertexCount);
	for (int vertex = 0; vertex < vertexCount; vertex++)
		adjacencyMatrix[vertex].resize(vertexCount, 0);
	if (weighted) {
		for (const auto & edge : weightedEdgeList) {
			int from = get<0>(edge);
			int to = get<1>(edge);
			int weight = get<2>(edge);
			adjacencyMatrix[from][to] = weight;
			if (!directed)
				adjacencyMatrix[to][from] = weight;
		}
		weightedEdgeList.clear();
	}
	else {
		for (const auto & edge : edgeList) {
			int from = edge.first;
			int to = edge.second;
			adjacencyMatrix[from][to] = 1;
			if (!directed)
				adjacencyMatrix[to][from] = 1;
		}
		edgeList.clear();
	}
}

void Graph::transformAdjMatrixToAdjList() {
	(weighted)
		? weightedAdjacencyList.resize(vertexCount)
		: adjacencyList.resize(vertexCount);
	for (int i = 0; i < vertexCount; i++) {
		for (int j = 0; j < vertexCount; j++)
			if (adjacencyMatrix[i][j]) {
				if (weighted)
					weightedAdjacencyList[i].insert(make_pair(j, adjacencyMatrix[i][j]));
				else
					adjacencyList[i].insert(j);
			}
		adjacencyMatrix[i].clear();
	}
	adjacencyMatrix.clear();
}

void Graph::transformEdgeListToAdjList() {
	if (weighted) {
		weightedAdjacencyList.resize(vertexCount);
		for (auto & edge : weightedEdgeList) {
			int from = get<0>(edge);
			int to = get<1>(edge);
			int weight = get<2>(edge);
			weightedAdjacencyList[from].insert(make_pair(to, weight));
			if (!directed)
				weightedAdjacencyList[to].insert(make_pair(from, weight));
		}
	}
	else {
		adjacencyList.resize(vertexCount);
		for (auto & edge : edgeList) {
			int to = get<0>(edge);
			int from = get<1>(edge);
			adjacencyList[from].insert(to);
			if (!directed)
				adjacencyList[to].insert(from);
		}
	}
}

void Graph::transformAdjMatrixToEdgeList() {
	for (int from = 0; from < vertexCount; from++) {
		for (int to = (directed) ? 0 : from; to < vertexCount; to++)
			if (adjacencyMatrix[from][to]) {
				if (weighted)
					weightedEdgeList.insert(make_tuple(from, to, adjacencyMatrix[from][to]));
				else
					edgeList.insert(make_pair(from, to));
			}
		adjacencyMatrix[from].clear();
	}
	adjacencyMatrix.clear();
}

void Graph::transformAdjListToEdgeList() {
	if (weighted) {
		for (int from = 0; from < vertexCount; from++) {
			for (const auto & adjacency : weightedAdjacencyList[from]) {
				int to = adjacency.first;
				if (!directed && from > to) continue; // в неориентированном графе список смежности дублирует ребра
				int weight = adjacency.second;
				weightedEdgeList.insert(make_tuple(from, to, weight));
			}
			weightedAdjacencyList[from].clear();
		}
		weightedAdjacencyList.clear();
	}
	else {
		for (int from = 0; from < vertexCount; from++) {
			for (const int & to : adjacencyList[from]) {
				if (!directed && from > to) continue; // в неориентированном графе список смежности дублирует ребра
				edgeList.insert(make_pair(from, to));
			}
			adjacencyList[from].clear();
		}
		adjacencyList.clear();
	}
}
