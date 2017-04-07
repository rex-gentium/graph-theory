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

	GraphContent * getCopy() const override;

	bool hasEdges() const;										// O(v)
	void addEdge(int from, int to, int weight) override;		// O(log(v))
	int changeEdge(int from, int to, int newWeight) override;	// O(log(v))
	void removeEdge(int from, int to) override;					// O(log(v))
	int getWeight(int from, int to) const override;				// O(log(v))
	int getAdjacent(int from) const override;					// O(1)

	list<tuple<int, int, int>> getWeightedEdgesList() const override; // O(v*min(e, v)) ~ O(v^2)
	tuple<int, int, int> findMinEdge(bool * isMarked) const override; // O(v*e)

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

	DSU getUnityComponents(int exceptFrom, int exceptTo) const override;
};

