#pragma once
#include "GraphContent.h"
#include <vector>
#include <set>
#include <list>

class EdgeList :
	public GraphContent {
	friend class GraphTrasformer;
	friend class Algorithm;
public:
	EdgeList();
	~EdgeList();

	void read(istream & inpFile) override;
	void write(ostream & outFile) override;

	GraphContent * getCopy() const override;

	bool hasEdges() const override;
	void addEdge(int from, int to, int weight) override;		// O(log(e))
	int changeEdge(int from, int to, int newWeight) override;	// O(log(e))
	void removeEdge(int from, int to) override;					// O(log(e))
	int getWeight(int from, int to) const override;				// O(log(e))
	int getAdjacent(int from) const override;					// O(e)

	list<tuple<int, int, int>> getWeightedEdgesList() const override; // O(e)
	tuple<int, int, int> findMinEdge(char * isMarked) const override; // O(e)

	int getVertexDegree(int vertex) const override;		// O(e)
	vector<int> getVerticesDegrees() const override;	// O(e)
	int getVertexInDegree(int vertex) const override;	// O(e)
	int getVertexOutDegree(int vertex) const override;	// O(e)
	vector<int> getVerticesInDegrees() const override;	// O(e)
	vector<int> getVerticesOutDegrees() const override;	// O(e)

	DSU getUnityComponents() const override; // O(e)

private:
	set<pair<int, int>> edgeList;
	set<tuple<int, int, int>> weightedEdgeList;

	DSU getUnityComponents(int exceptFrom, int exceptTo) const override;
};

