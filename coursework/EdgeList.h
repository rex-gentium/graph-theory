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
	void addEdge(int from, int to, int weight) override;
	int changeEdge(int from, int to, int newWeight) override;
	void removeEdge(int from, int to) override;

	/* O(e) */
	list<tuple<int, int, int>> getWeightedEdgesList() const override;
	/* O(e) */
	tuple<int, int, int> findMinEdge(bool * isMarked) const override;

	int getVertexDegree(int vertex) const override;		// O(e)
	vector<int> getVerticesDegrees() const override;	// O(e)
	int getVertexInDegree(int vertex) const override;	// O(e)
	int getVertexOutDegree(int vertex) const override;	// O(e)
	vector<int> getVerticesInDegrees() const override;	// O(e)
	vector<int> getVerticesOutDegrees() const override;	// O(e)

private:
	set<pair<int, int>> edgeList;
	set<tuple<int, int, int>> weightedEdgeList;
};

