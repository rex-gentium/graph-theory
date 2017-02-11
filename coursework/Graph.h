#pragma once
#include <vector>
#include <set>

using namespace std;

class Graph {
public:
	enum RepresentationType { NONE, ADJMATRIX, ADJLIST, EDGELIST };

	Graph();
	~Graph();

	// note: воспримет ориентированный граф без петель и симметричной матрицей как неориентированный
	void readGraph(const string fileName);
	void addEdge(int from, int to, int weight = 0);
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
	bool isDirected;
	bool isWeighted;
	vector<vector<int>> adjacencyMatrix;
	vector<set<int>> adjacencyList;
	vector<set<pair<int, int>>> weightedAdjacencyList;
	set<pair<int, int>> edgeList;
	set<tuple<int, int, int>> weightedEdgeList;

	void readGraphAdjMatrix(ifstream & inpFile);
	void readGraphAdjList(ifstream & inpFile);
	void readGraphEdgeList(ifstream & inpFile);

	void writeGraphAdjMatrix(ofstream & outFile);
	void writeGraphAdjList(ofstream & outFile);
	void writeGraphEdgeList(ofstream & outFile);

	void addEdgeAdjMatrix(int from, int to, int weight);
	void addEdgeAdjList(int from, int to, int weight);
	void addEdgeEdgeList(int from, int to, int weight);

	int changeEdgeAdjMatrix(int from, int to, int newWeight);
	int changeEdgeAdjList(int from, int to, int newWeight);
	int changeEdgeEdgeList(int from, int to, int newWeight);

	void removeEdgeAdjMatrix(int from, int to);
	void removeEdgeAdjList(int from, int to);
	void removeEdgeEdgeList(int from, int to);

	void transformAdjListToAdjMatrix();
	void transformEdgeListToAdjMatrix();
	void transformAdjMatrixToAdjList();
	void transformEdgeListToAdjList();
	void transformAdjMatrixToEdgeList();
	void transformAdjListToEdgeList();
};

