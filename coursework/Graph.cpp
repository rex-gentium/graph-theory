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
		adjacencyMatrix[from][to] = (weighted) ? weight : 1;
		if (!directed)
			adjacencyMatrix[to][from] = adjacencyMatrix[from][to];
		break;
	case RepresentationType::ADJLIST:
		if (weighted) {
			weightedAdjacencyList[from].insert(make_pair(to, weight));
			if (!directed)
				weightedAdjacencyList[to].insert(make_pair(from, weight));
		}
		else {
			adjacencyList[from].insert(to);
			if (!directed)
				adjacencyList[to].insert(from);
		}
		break;
	case RepresentationType::EDGELIST:
		if (weighted)
			weightedEdgeList.insert(make_tuple(from, to, weight));
		else
			edgeList.insert(make_pair(from, to));
		break;
	}
}

void Graph::removeEdge(int from, int to) {
	--from;	// поправка на нумерацию с нуля
	--to;
	switch (this->graphForm) {

	case RepresentationType::ADJMATRIX:
		adjacencyMatrix[from][to] = 0;
		if (!directed)
			adjacencyMatrix[to][from] = 0;
		break;

	case RepresentationType::ADJLIST: {
		if (weighted) {
			set<pair<int,int>> vertexAdjList = weightedAdjacencyList[from];
			// вес не имеет значения, сравнение происходит по первому элементу пары
			auto it = vertexAdjList.lower_bound(make_pair(to, 0));
			if (it != vertexAdjList.end())
				vertexAdjList.erase(it);
			if (!directed) {
				// в неориентированном графе списки дублируются
				vertexAdjList = weightedAdjacencyList[to];
				it = vertexAdjList.lower_bound(make_pair(from, 0));
				if (it != vertexAdjList.end())
					vertexAdjList.erase(it);
			}
		}
		else {
			set<int> * vertexAdjList = &(adjacencyList[from]);
			auto it = vertexAdjList->find(to);
			if (it != vertexAdjList->end())
				vertexAdjList->erase(it);
			if (!directed) {
				// в неориентированном графе списки дублируются
				vertexAdjList = &(adjacencyList[to]);
				it = vertexAdjList->find(from);
				if (it != vertexAdjList->end())
					vertexAdjList->erase(it);
			}
		}
		break;
	}

	case RepresentationType::EDGELIST: {
		if (weighted) {
			auto it = weightedEdgeList.lower_bound(make_tuple(from, to, 0));
			if (it != weightedEdgeList.end())
				weightedEdgeList.erase(it);
			if (!directed) {
				// в неориентированном графе ребро может связывать вершины в обратном порядке
				it = weightedEdgeList.lower_bound(make_tuple(to, from, 0));
				if (it != weightedEdgeList.end())
					weightedEdgeList.erase(it);
			}
		}
		else {
			edgeList.erase(make_pair(from, to));
			if (!directed)
				edgeList.erase(make_pair(to, from));
		}
		break;
	}
	}
}

int Graph::changeEdge(int from, int to, int newWeight) {
	if (!weighted) return 0; // бессмысленная операция
	--from;	// поправка на нумерацию с нуля
	--to;
	int oldWeight = 0;
	switch (this->graphForm) {
	
	case RepresentationType::ADJMATRIX:
		oldWeight = adjacencyMatrix[from][to];
		adjacencyMatrix[from][to] = newWeight;
		if (!directed)
			adjacencyMatrix[to][from] = newWeight;
		break;

	case RepresentationType::ADJLIST: {
		set<pair<int, int>> * vertexAdjList = &(weightedAdjacencyList[from]);
		auto it = vertexAdjList->lower_bound(make_pair(to, 0));
		if (it != vertexAdjList->end()) {
			oldWeight = it->second;
			vertexAdjList->insert(it, make_pair(to, newWeight));
		}
		if (!directed) {
			// неориентированный граф дублирует списки
			vertexAdjList = &(weightedAdjacencyList[to]);
			it = vertexAdjList->lower_bound(make_pair(from, 0));
			if (it != vertexAdjList->end()) {
				oldWeight = it->second;
				vertexAdjList->insert(it, make_pair(from, newWeight));
			}
		}
		break;
	}

	case RepresentationType::EDGELIST: {
		auto it = weightedEdgeList.lower_bound(make_tuple(from, to, 0));
		if (it != weightedEdgeList.end() && get<0>(*it) == from && get<1>(*it) == to) {
			oldWeight = get<2>(*it);
			weightedEdgeList.insert(it, make_tuple(from, to, newWeight));
		}
		if (!directed) {
			// в неориентированном графе ребро может связывать вершины в обратном порядке
			it = weightedEdgeList.lower_bound(make_tuple(to, from, 0));
			if (it != weightedEdgeList.end() && get<0>(*it) == to && get<1>(*it) == from) {
				oldWeight = get<2>(*it);
				weightedEdgeList.insert(it, make_tuple(to, from, newWeight));
			}
		}
		break;
	}
	}
	return oldWeight;
}

void Graph::transformToAdjList() {
	switch (this->graphForm) 
	{
	case RepresentationType::ADJLIST: break;
	case RepresentationType::ADJMATRIX:
		if (weighted)
			weightedAdjacencyList.resize(vertexCount);
		else 
			adjacencyList.resize(vertexCount);
		for (int i = 0; i < vertexCount; i++) {
			for (int j = 0; j < vertexCount; j++)
				if (adjacencyMatrix[i][j]) {
					if (weighted)
						weightedAdjacencyList[i].insert(make_pair(j, adjacencyMatrix[i][j]));
					else 
						adjacencyList[i].insert(j);
				};
			adjacencyMatrix[i].clear();
		}
		adjacencyMatrix.clear();
		break;

	case RepresentationType::EDGELIST:
		if (weighted) {
			weightedAdjacencyList.resize(vertexCount);
			for (auto & edge : weightedEdgeList) {
				int i = get<0>(edge);
				int j = get<1>(edge);
				int weight = get<2>(edge);
				weightedAdjacencyList[i].insert(make_pair(j, weight));
				if (!directed)
					weightedAdjacencyList[j].insert(make_pair(i, weight));
			}
		}
		else {
			adjacencyList.resize(vertexCount);
			for (auto & edge : edgeList) {
				int i = get<0>(edge);
				int j = get<1>(edge);
				adjacencyList[i].insert(j);
				if (!directed)
					adjacencyList[j].insert(i);
			}
		}
		break;
	}
	graphForm = RepresentationType::ADJLIST;
}

void Graph::transformToAdjMatrix() {
	switch (this->graphForm) {
	case RepresentationType::ADJMATRIX: break;
	case RepresentationType::ADJLIST:
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
		break;

	case RepresentationType::EDGELIST:
		adjacencyMatrix.resize(vertexCount);
		for (int i = 0; i < vertexCount; i++)
			adjacencyMatrix[i].resize(vertexCount, 0);
		if (weighted) {
			for (const auto & edge : weightedEdgeList) {
				int i = get<0>(edge);
				int j = get<1>(edge);
				int weight = get<2>(edge);
				adjacencyMatrix[i][j] = weight;
				if (!directed)
					adjacencyMatrix[j][i] = weight;
			}
			weightedEdgeList.clear();
		}
		else {
			for (const auto & edge : edgeList) {
				int i = edge.first;
				int j = edge.second;
				adjacencyMatrix[i][j] = 1;
				if (!directed)
					adjacencyMatrix[j][i] = 1;
			}
			edgeList.clear();
		}
		break;
	}
	graphForm = RepresentationType::ADJMATRIX;
}

void Graph::transformToListOfEdges() {
	switch (this->graphForm) {
	case RepresentationType::EDGELIST: break;
	case RepresentationType::ADJMATRIX:
		for (int i = 0; i < vertexCount; i++) {
			for (int j = (directed) ? 0 : i; j < vertexCount; j++)
				if (adjacencyMatrix[i][j]) {
					if (weighted)
						weightedEdgeList.insert(make_tuple(i, j, adjacencyMatrix[i][j]));
					else
						edgeList.insert(make_pair(i, j));
				}
			adjacencyMatrix[i].clear();
		}
		adjacencyMatrix.clear();
		break;

	case RepresentationType::ADJLIST:
		if (weighted) {
			for (int i = 0; i < vertexCount; i++) {
				for (const auto & adjacency : weightedAdjacencyList[i]) {
					int j = adjacency.first;
					if (!directed && i > j) continue; // в неориентированном графе список смежности дублирует ребра
					int weight = adjacency.second;
					weightedEdgeList.insert(make_tuple(i, j, weight));
				}
				weightedAdjacencyList[i].clear();
			}
			weightedAdjacencyList.clear();
		}
		else {
			for (int i = 0; i < vertexCount; i++) {
				for (const int & j : adjacencyList[i]) {
					if (!directed && i > j) continue; // в неориентированном графе список смежности дублирует ребра
					edgeList.insert(make_pair(i, j));
				}
				adjacencyList[i].clear();
			}
			adjacencyList.clear();
		}
		break;
	}
	graphForm = RepresentationType::EDGELIST;
}

void Graph::writeGraph(const string fileName) {
	ofstream outFile;
	outFile.open(fileName);
	switch (this->graphForm) {
	case RepresentationType::ADJMATRIX: 
		outFile << 'C' << ' ' << vertexCount << ' ' << ((weighted) ? 1 : 0) << '\n';
		for (int i = 0; i < vertexCount; i++)
			for (int j = 0; j < vertexCount; j++)
				outFile << adjacencyMatrix[i][j] << ((j == vertexCount - 1) ? '\n' : ' ');
		break;
	case RepresentationType::ADJLIST:
		outFile << 'L' << ' ' << vertexCount << '\n'
			<< ((directed) ? 1 : 0) << ' ' << ((weighted) ? 1 : 0) << '\n';
		if (weighted) {
			for (int i = 0; i < vertexCount; i++) {
				for (const auto & adjacency : weightedAdjacencyList[i])
					outFile << (adjacency.first + 1) << ' ' << adjacency.second << ' ';
				outFile << '\n';
			}
		}
		else {
			for (int i = 0; i < vertexCount; i++) {
				for (const int & adjacent : adjacencyList[i])
					outFile << (adjacent + 1) << ' ';
				outFile << '\n';
			}
		}
		break;
	case RepresentationType::EDGELIST:
		outFile << 'E' << ' ' << vertexCount << ' ' 
			<< ((weighted) ? weightedEdgeList.size() : edgeList.size()) << '\n'
			<< ((directed) ? 1 : 0) << ' ' << ((weighted) ? 1 : 0) << '\n';
		if (weighted)
			for (const auto & edge : weightedEdgeList)
					outFile << get<0>(edge) + 1 << ' ' << get<1>(edge) + 1 << ' ' << get<2>(edge) << '\n';
		else
			for (const auto & edge : edgeList)
				outFile << edge.first + 1 << ' ' << edge.second + 1 << '\n';
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
	for (int i = 0; i < vertexCount; i++) {
		adjacencyMatrix[i].resize(vertexCount);
		for (int j = 0; j < vertexCount; j++)
			inpFile >> adjacencyMatrix[i][j];
	}
	this->graphForm = RepresentationType::ADJMATRIX;
	// проверка на ориентированность
	this->directed = false;
	for (int i = 0; i < vertexCount; i++)
		for (int j = i; j < vertexCount; j++)
			if (adjacencyMatrix[i][j] != adjacencyMatrix[j][i]) {
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
		for (int i = 0; i < vertexCount; i++) {
			set<pair<int, int>> listContent;
			getline(inpFile, line);
			int j, weight;
			istringstream iss(line);
			while (iss >> j) {
				--j; // поправка на нумерацию с нуля
				iss >> weight;
				listContent.insert(make_pair(j, weight));
			}
			weightedAdjacencyList.push_back(listContent);
		}
	}
	else {
		for (int i = 0; i < vertexCount; i++) {
			set<int> listContent;
			getline(inpFile, line);
			//cout << line;
			istringstream iss(line);
			int j;
			while (iss >> j) {
				--j; // поправка на нумерацию с нуля
				listContent.insert(j);
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
	for (int e = 0; e < edgeCount; e++) {
		int i, j;
		inpFile >> i;
		inpFile >> j;
		--i;
		--j; // поправка на нумерацию с нуля
		if (weighted) {
			int weight;
			inpFile >> weight;
			this->weightedEdgeList.insert(make_tuple(i, j, weight));
		}
		else
			this->edgeList.insert(make_pair(i, j));
	}
	this->graphForm = RepresentationType::EDGELIST;
}
