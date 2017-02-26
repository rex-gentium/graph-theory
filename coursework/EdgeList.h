#pragma once
#include "GraphContent.h"
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
	void addEdge(int from, int to, int weight) override;
	int changeEdge(int from, int to, int newWeight) override;
	void removeEdge(int from, int to) override;

	list<tuple<int, int, int>> getWeightedEdgesList() const override;
	set<pair<int, int>> getWeightedAdjacencies(int vertex) const override;

private:
	set<pair<int, int>> edgeList;
	set<tuple<int, int, int>> weightedEdgeList;
};

