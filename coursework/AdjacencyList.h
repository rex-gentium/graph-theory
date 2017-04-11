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

	ConstEdgeIterator& edgeBegin() const override;
	ConstEdgeIterator& edgeEnd() const override;

private:
	vector<set<int>> adjacencyList;
	vector<set<pair<int, int>>> weightedAdjacencyList;
public:
	class AdjListEdgeIterator : public ConstEdgeIterator {
	public:
		class InnerIterator {
			set<int>::const_iterator val;
			bool isValid = false;
			InnerIterator() : isValid(false) {}
		public:
			InnerIterator(set<int>::const_iterator it) : val(it), isValid(true) {}
			static InnerIterator& null() { return * new InnerIterator(); }
			bool isNull() const { return !isValid; }
			const int & operator*() { return *val; }
			InnerIterator& operator++() { ++val; return *this; }
			bool operator==(const set<int>::const_iterator & rhs) { return val == rhs; }
			bool operator!=(const set<int>::const_iterator & rhs) { return val != rhs; }
			const int * operator->() { return val.operator->(); }
		};
		class InnerWeightedIterator {
			set<pair<int, int>>::const_iterator val;
			bool isValid = false;
			InnerWeightedIterator() : isValid(false) {}
		public:
			InnerWeightedIterator(set<pair<int, int>>::const_iterator it) : val(it), isValid(true) {}
			static InnerWeightedIterator& null() { return *new InnerWeightedIterator(); }
			bool isNull() const { return !isValid; }
			const pair<int, int> & operator*() { return *val; }
			InnerWeightedIterator& operator++() { ++val; return *this; }
			bool operator==(const set<pair<int, int>>::const_iterator & rhs) { return val == rhs; }
			bool operator!=(const set<pair<int, int>>::const_iterator & rhs) { return val != rhs; }
			const pair<int, int> * operator->() { return val.operator->(); }
		};
	private:
		const AdjacencyList * adjList;
		InnerIterator it;
		InnerWeightedIterator wIt;
	public:
		AdjListEdgeIterator(const AdjacencyList * graph, int fromV, InnerIterator it);
		AdjListEdgeIterator(const AdjacencyList * graph, int fromV, InnerWeightedIterator it);
		InnerIterator& inner() { return it; }
		InnerWeightedIterator& innerWeighted() { return wIt; }
	};

	tuple<int, int, int> nextEdge(ConstEdgeIterator & iterator) const override;
	tuple<int, int, int> nextEdge(int fromV, AdjListEdgeIterator::InnerIterator & iterator) const;
	tuple<int, int, int> nextEdge(int fromV, AdjListEdgeIterator::InnerWeightedIterator & iterator) const;
};

