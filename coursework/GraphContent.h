#pragma once
#include <iostream>
#include <list>
#include <set>
#include <tuple>

using namespace std;

class GraphContent {
public:
	int vertexCount;
	bool isDirected;
	bool isWeighted; 
	
	GraphContent();
	virtual ~GraphContent();

	virtual void read(istream & inpFile) = 0;
	virtual void write(ostream & outFile) = 0;

	virtual void addEdge(int from, int to, int weight = 0) = 0;
	virtual int changeEdge(int from, int to, int newWeight) = 0;
	virtual void removeEdge(int from, int to) = 0;

	/* возвращает список взвешенных рёбер графа <from, to, weight>*/
	virtual list<tuple<int, int, int>> getWeightedEdgesList() const = 0;
	/* возвращает минимальное по весу ребро <from, to, weight>, соединяющее любую помеченную вершину с любой непомеченной */
	virtual tuple<int, int, int> findMinEdge(bool * isMarked) const = 0;

	/* итератор по рёбрам. не позволяет изменять значения (хранит копию ребра) */
	class ConstEdgeIterator {
	protected:
		const GraphContent * graph;
		tuple<int, int, int> edge;
	public:
		ConstEdgeIterator(const GraphContent* graph, int from, int to, int weight = -1): graph(graph), edge(make_tuple(from, to, weight)) {}
		virtual ~ConstEdgeIterator() {}
		tuple<int, int, int> operator*() const { return (*this != graph->edgeEnd()) ? edge : make_tuple(-1,-1,-1); }
		ConstEdgeIterator& operator++() { 
			if (*this != graph->edgeEnd()) 
				edge = graph->nextEdge(*this); 
			return *this; }
		bool operator==(const ConstEdgeIterator & rhs) const { return graph == rhs.graph && edge == rhs.edge; }
		bool operator!=(const ConstEdgeIterator & rhs) const { return !(*this == rhs); }
		int from() const { return get<0>(this->operator*()); }
		int to() const { return get<1>(this->operator*()); }
		int weight() const { return get<2>(this->operator*()); }
	};

	/* возвращает итератор по ребрам */
	virtual ConstEdgeIterator& edgeBegin() const = 0;
	/* возвращает итератор по ребрам, сигнализирующий конец коллекции */
	virtual ConstEdgeIterator& edgeEnd() const = 0;
private:
	/* возвращает следующее за данным ребро (порядок определяется представлением). если граф не взвешен, третий элемент кортежа равен -1 */
	virtual tuple<int, int, int> nextEdge(ConstEdgeIterator & iterator) const = 0;
};

