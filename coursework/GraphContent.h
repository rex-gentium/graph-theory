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
};

