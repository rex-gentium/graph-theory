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

	/* O(e) */
	list<tuple<int, int, int>> getWeightedEdgesList() const override;
	/* O(e) */
	tuple<int, int, int> findMinEdge(bool * isMarked) const override;

	ConstEdgeIterator& edgeBegin() const override;
	ConstEdgeIterator& edgeEnd() const override;

private:
	set<pair<int, int>> edgeList;
	set<tuple<int, int, int>> weightedEdgeList;

	class EdgeListEdgeIterator : public ConstEdgeIterator {
	public:
		class InnerIterator : public set<pair<int, int>>::const_iterator {
			bool isValid = false;
			InnerIterator() : isValid(false) {}
		public:
			InnerIterator(set<pair<int, int>>::const_iterator it) : set<pair<int, int>>::const_iterator(it), isValid(true) {}
			static InnerIterator& null() { return *new InnerIterator(); }
			bool isNull() const { return !isValid; }
		};
		class InnerWeightedIterator : public set<tuple<int, int, int>>::const_iterator {
			bool isValid = false;
			InnerWeightedIterator() : isValid(false) {}
		public:
			InnerWeightedIterator(set<tuple<int, int, int>>::const_iterator it) : set<tuple<int, int, int>>::const_iterator(it), isValid(true) {}
			static InnerWeightedIterator& null() { return *new InnerWeightedIterator(); }
			bool isNull() const { return !isValid; }
		};
	private:
		const EdgeList * edgeList;
		InnerIterator it;
		InnerWeightedIterator wIt;
	public:
		EdgeListEdgeIterator(const EdgeList * graph, InnerIterator it);
		EdgeListEdgeIterator(const EdgeList * graph, InnerWeightedIterator it);
		InnerIterator& inner() { return it; }
		InnerWeightedIterator& innerWeighted() { return wIt; }
	};

	tuple<int, int, int> nextEdge(ConstEdgeIterator & iterator) const override;
	tuple<int, int, int> nextEdge(EdgeListEdgeIterator::InnerIterator & iterator) const;
	tuple<int, int, int> nextEdge(EdgeListEdgeIterator::InnerWeightedIterator & iterator) const;
};

