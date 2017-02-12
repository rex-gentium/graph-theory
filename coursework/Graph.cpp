#include "Graph.h"
#include "AdjacencyMatrix.h"
#include "AdjacencyList.h"
#include "EdgeList.h"
#include "GraphTrasformer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>

Graph::Graph() {}

Graph::~Graph() {}

void Graph::readGraph(const string fileName) {
	ifstream inpFile;
	inpFile.open(fileName);
	char mode;
	mode = inpFile.get();
	switch (mode) {
		case 'C' : 
			representation = new AdjacencyMatrix();
			break;
		case 'L' : 
			representation = new AdjacencyList();
			break;
		case 'E' : 
			representation = new EdgeList();
			break;
	}
	representation->read(inpFile);
	inpFile.close();
}

void Graph::addEdge(int from, int to, int weight) {
	--from;
	--to;
	representation->addEdge(from, to, weight);
}

void Graph::removeEdge(int from, int to) {
	--from;
	--to;
	representation->removeEdge(from, to);
}

int Graph::changeEdge(int from, int to, int newWeight) {
	if (!representation->isWeighted) return 0; // бессмысленная операция
	--from;
	--to;
	return representation->changeEdge(from, to, newWeight);
}

void Graph::transformToAdjList() {
	AdjacencyList * adjListRepr = new AdjacencyList();
	bool isTransformed = false;
	if (AdjacencyMatrix* adjMatrixRepr = dynamic_cast<AdjacencyMatrix*>(representation))
		isTransformed = GraphTrasformer::transferContent(adjMatrixRepr, adjListRepr);
	else if (EdgeList* edgeListRepr = dynamic_cast<EdgeList*>(representation))
		isTransformed = GraphTrasformer::transferContent(edgeListRepr, adjListRepr);
	if (isTransformed) {
		delete representation;
		representation = adjListRepr;
	}
}

void Graph::transformToAdjMatrix() {
	AdjacencyMatrix * adjMatrixRepr = new AdjacencyMatrix();
	bool isTransformed = false;
	if (AdjacencyList* adjListRepr = dynamic_cast<AdjacencyList*>(representation))
		isTransformed = GraphTrasformer::transferContent(adjListRepr, adjMatrixRepr);
	else if (EdgeList* edgeListRepr = dynamic_cast<EdgeList*>(representation))
		isTransformed = GraphTrasformer::transferContent(edgeListRepr, adjMatrixRepr);
	if (isTransformed) {
		delete representation;
		representation = adjMatrixRepr;
	}
}

void Graph::transformToListOfEdges() {
	EdgeList * edgeListRepr = new EdgeList();
	bool isTransformed = false;
	if (AdjacencyMatrix* adjMatrixRepr = dynamic_cast<AdjacencyMatrix*>(representation))
		isTransformed = GraphTrasformer::transferContent(adjMatrixRepr, edgeListRepr);
	else if (AdjacencyList* adjListRepr = dynamic_cast<AdjacencyList*>(representation))
		isTransformed = GraphTrasformer::transferContent(adjListRepr, edgeListRepr);
	if (isTransformed) {
		delete representation;
		representation = edgeListRepr;
	}
}

void Graph::writeGraph(const string fileName) {
	ofstream outFile;
	outFile.open(fileName);
	representation->write(outFile);
	outFile.close();
}