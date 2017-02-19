#pragma once
#include "GraphContent.h"
#include <vector>

using namespace std;

class AdjacencyMatrix : 
	public GraphContent {
	friend class GraphTrasformer;
public:
	AdjacencyMatrix();
	AdjacencyMatrix(int vertexCount); 
	~AdjacencyMatrix();

	void read(istream & inpFile) override;
	void write(ostream & outFile) override;
	void addEdge(int from, int to, int weight) override;
	int changeEdge(int from, int to, int newWeight) override;
	void removeEdge(int from, int to) override;

	GraphContent* getSpaingTreePrima() override;
	GraphContent* getSpaingTreeKruscal() override;
	GraphContent* getSpaingTreeBoruvka() override;

private:
	vector<vector<int>> adjacencyMatrix;
};

