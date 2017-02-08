#pragma once
#include <vector>
#include <set>

using namespace std;

class Graph {
public:
	enum RepresentationType { NONE, ADJMATRIX, ADJLIST, EDGELIST };

	Graph();
	~Graph();

	void readGraph(const string fileName);
	void addEdge(int from, int to, int weight);
	void removeEdge(int from, int to);
	// возвращает предыдущее значение веса ребра
	int changeEdge(int from, int to, int newWeight);
	void transformToAdjList();
	void transformToAdjMatrix();
	void transformToListOfEdges();
	void writeGraph(const string fileName);

private:
	RepresentationType graphForm;
	int vertexCount;
	bool directed;
	bool weighted;
	vector<vector<int>> adjacencyMatrix;
	vector<set<int>> adjacencyList;
	vector<set<pair<int, int>>> weightedAdjacencyList;
	set<pair<int, int>> edgeList;
	set<tuple<int, int, int>> weightedEdgeList;

	void readGraphAdjMatrix(ifstream & inpFile);
	void readGraphAdjList(ifstream & inpFile);
	void readGraphEdgeList(ifstream & inpFile);
};

