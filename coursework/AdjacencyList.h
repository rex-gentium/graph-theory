#pragma once
#include "GraphContent.h"
#include <vector>
#include <set>

class AdjacencyList :
	public GraphContent {
	friend class GraphTrasformer;
	friend class Algorithm;
public:
	AdjacencyList();
	AdjacencyList(int vertexCount, bool isWeigted, bool isDirected);
	~AdjacencyList();

	void read(istream & inpFile) override;
	void write(ostream & outFile) override;
	void addEdge(int from, int to, int weight) override;
	int changeEdge(int from, int to, int newWeight) override;
	void removeEdge(int from, int to) override;

private:
	vector<set<int>> adjacencyList;
	vector<set<pair<int, int>>> weightedAdjacencyList;
};

