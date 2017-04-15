#pragma once
#include "GraphContent.h"
#include <vector>
#include <set>

using namespace std;

class Graph {
public:
	enum RepresentationType { NONE, ADJMATRIX, ADJLIST, EDGELIST };
	Graph();
	~Graph();
	Graph& operator=(const Graph & rhs);

	/* ввод-вывод */
	// note: воспримет ориентированный граф без петель и симметричной матрицей как неориентированный
	void readGraph(const string fileName);
	void writeGraph(const string fileName);

	/* редактирование содержимого */
	void addEdge(int from, int to, int weight = 0);
	void removeEdge(int from, int to);
	// возвращает предыдущее значение веса ребра
	int changeEdge(int from, int to, int newWeight);

	/* трансформации представлений */
	void transformToAdjList();
	void transformToAdjMatrix();
	void transformToListOfEdges();
	
	/* остовные деревья */
	Graph getSpaingTreePrima();
	Graph getSpaingTreeKruscal();
	Graph getSpaingTreeBoruvka();

	/* эйлеровы пути и циклы */
	int checkEuler(bool &circleExist);
	vector<int> getEuleranTourFleri();
	vector<int> getEuleranTourEffective();

	/* паросочетания */
	int checkBipart(vector<char> &marks);
	vector<pair<int, int>> getMaximumMatchingBipart();

private:
	GraphContent * content;
	RepresentationType currentRepr;
};

