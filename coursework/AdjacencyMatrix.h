#pragma once
#include "GraphContent.h"
#include <vector>

using namespace std;

class AdjacencyMatrix : 
	public GraphContent {
	friend class GraphTrasformer;
	friend class Algorithm;
public:
	AdjacencyMatrix();
	AdjacencyMatrix(int vertexCount); 
	~AdjacencyMatrix();

	void read(istream & inpFile) override;
	void write(ostream & outFile) override;
	void addEdge(int from, int to, int weight) override;
	int changeEdge(int from, int to, int newWeight) override;
	void removeEdge(int from, int to) override;

	/* O(v^2) */
	list<tuple<int, int, int>> getWeightedEdgesList() const override;
	/* O(v^2) */
	tuple<int, int, int> findMinEdge(bool * isMarked) const override;
	
private:
	vector<vector<int>> adjacencyMatrix;
};

