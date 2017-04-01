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

	/* O(v*min(e, v)) ~ O(v^2) */
	list<tuple<int, int, int>> getWeightedEdgesList() const override;
	/* O(v*e) */
	tuple<int, int, int> findMinEdge(bool * isMarked) const override;

	int getVertexDegree(int vertex) const override;		// O(log(v))
	vector<int> getVerticesDegrees() const override;	// O(v*log(v))
	int getVertexInDegree(int vertex) const override;	// O(v*log(v)) 
	int getVertexOutDegree(int vertex) const override;	// O(1)
	vector<int> getVerticesInDegrees() const override;	// O(v^2)
	vector<int> getVerticesOutDegrees() const override;	// O(v)

	DSU getUnityComponents() const override; // O(v*log(v))

private:
	vector<set<int>> adjacencyList;
	vector<set<pair<int, int>>> weightedAdjacencyList;
};

