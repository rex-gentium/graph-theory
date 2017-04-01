#pragma once
#include "DSU.h"
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
	
	/* возвращает степень вершины */
	virtual int getVertexDegree(int vertex) const = 0;
	/* возвращает массив степеней вершин */
	virtual vector<int> getVerticesDegrees() const = 0;
	/* возвращает полустепень захода вершины */
	virtual int getVertexInDegree(int vertex) const = 0;
	/* возвращает полустепень исхода вершины */
	virtual int getVertexOutDegree(int vertex) const = 0;
	/* возвращает массив полустепеней заходов вершин */
	virtual vector<int> getVerticesInDegrees() const = 0;
	/* возвращает массив полустепеней исходов вершин */
	virtual vector<int> getVerticesOutDegrees() const = 0;
	/* возвращает компоненты связности в виде системы непересекающихся множеств */
	virtual DSU getUnityComponents() const = 0;
};

