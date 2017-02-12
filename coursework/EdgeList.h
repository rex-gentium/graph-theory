#pragma once
#include "GraphContent.h"
#include <set>

class EdgeList :
	public GraphContent {
	friend class GraphTrasformer;
public:
	EdgeList();
	~EdgeList();

	void read(istream & inpFile) override;
	void write(ostream & outFile) override;
	void addEdge(int from, int to, int weight) override;
	int changeEdge(int from, int to, int newWeight) override;
	void removeEdge(int from, int to) override;

private:
	set<pair<int, int>> edgeList;
	set<tuple<int, int, int>> weightedEdgeList;
};

