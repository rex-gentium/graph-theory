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

	GraphContent * getCopy() const override;

	bool hasEdges() const override;								// O(v^2)
	void addEdge(int from, int to, int weight) override;		// O(1)
	int changeEdge(int from, int to, int newWeight) override;	// O(1)
	void removeEdge(int from, int to) override;					// O(1)
	int getWeight(int from, int to) const override;				// O(1)
	int getAdjacent(int from) const override;					// O(v)

	list<tuple<int, int, int>> getWeightedEdgesList() const override; // O(v^2)
	tuple<int, int, int> findMinEdge(bool * isMarked) const override; // O(v^2)
	
	int getVertexDegree(int vertex) const override;		// O(v)
	vector<int> getVerticesDegrees() const override;	// O(v^2)
	int getVertexInDegree(int vertex) const override;	// O(v)
	int getVertexOutDegree(int vertex) const override;	// O(v)
	vector<int> getVerticesInDegrees() const override;	// O(v^2)
	vector<int> getVerticesOutDegrees() const override;	// O(v^2)

	DSU getUnityComponents() const override;	// O(v^2)

private:
	vector<vector<int>> adjacencyMatrix;

	DSU getUnityComponents(int exceptFrom, int exceptTo) const override;
};

