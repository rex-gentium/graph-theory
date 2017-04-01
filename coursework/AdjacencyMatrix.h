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
	
	int getVertexDegree(int vertex) const override;		// O(v)
	vector<int> getVerticesDegrees() const override;	// O(v^2)
	int getVertexInDegree(int vertex) const override;	// O(v)
	int getVertexOutDegree(int vertex) const override;	// O(v)
	vector<int> getVerticesInDegrees() const override;	// O(v^2)
	vector<int> getVerticesOutDegrees() const override;	// O(v^2)

	DSU getUnityComponents() const override;	// O(v^2)

private:
	vector<vector<int>> adjacencyMatrix;
};

