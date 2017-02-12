#pragma once
#include "GraphContent.h"
#include <vector>
#include <set>

using namespace std;

class Graph {
public:
	Graph();
	~Graph();

	// note: воспримет ориентированный граф без петель и симметричной матрицей как неориентированный
	void readGraph(const string fileName);
	void addEdge(int from, int to, int weight = 0);
	void removeEdge(int from, int to);
	// возвращает предыдущее значение веса ребра
	int changeEdge(int from, int to, int newWeight);
	void transformToAdjList();
	void transformToAdjMatrix();
	void transformToListOfEdges();
	void writeGraph(const string fileName);

private:
	GraphContent * representation;
};

