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

Graph::Graph(int vertexCount) {
	content = new EdgeList();
	content->isDirected = false;
	content->isWeighted = true;
	content->vertexCount = vertexCount;
}

Graph::~Graph() {}

void Graph::readGraph(const string fileName) {
	ifstream inpFile;
	inpFile.open(fileName);
	char mode;
	mode = inpFile.get();
	switch (mode) {
		case 'C' : 
			content = new AdjacencyMatrix();
			break;
		case 'L' : 
			content = new AdjacencyList();
			break;
		case 'E' : 
			content = new EdgeList();
			break;
	}
	content->read(inpFile);
	inpFile.close();
}

void Graph::addEdge(int from, int to, int weight) {
	--from;
	--to;
	content->addEdge(from, to, weight);
}

void Graph::removeEdge(int from, int to) {
	--from;
	--to;
	content->removeEdge(from, to);
}

int Graph::changeEdge(int from, int to, int newWeight) {
	if (!content->isWeighted) return 0; // бессмысленная операция
	--from;
	--to;
	return content->changeEdge(from, to, newWeight);
}

void Graph::transformToAdjList() {
	AdjacencyList * adjListRepr = new AdjacencyList();
	bool isTransformed = false;
	if (AdjacencyMatrix* adjMatrixRepr = dynamic_cast<AdjacencyMatrix*>(content))
		isTransformed = GraphTrasformer::transferContent(adjMatrixRepr, adjListRepr);
	else if (EdgeList* edgeListRepr = dynamic_cast<EdgeList*>(content))
		isTransformed = GraphTrasformer::transferContent(edgeListRepr, adjListRepr);
	if (isTransformed) {
		delete content;
		content = adjListRepr;
	}
}

void Graph::transformToAdjMatrix() {
	AdjacencyMatrix * adjMatrixRepr = new AdjacencyMatrix();
	bool isTransformed = false;
	if (AdjacencyList* adjListRepr = dynamic_cast<AdjacencyList*>(content))
		isTransformed = GraphTrasformer::transferContent(adjListRepr, adjMatrixRepr);
	else if (EdgeList* edgeListRepr = dynamic_cast<EdgeList*>(content))
		isTransformed = GraphTrasformer::transferContent(edgeListRepr, adjMatrixRepr);
	if (isTransformed) {
		delete content;
		content = adjMatrixRepr;
	}
}

void Graph::transformToListOfEdges() {
	EdgeList * edgeListRepr = new EdgeList();
	bool isTransformed = false;
	if (AdjacencyMatrix* adjMatrixRepr = dynamic_cast<AdjacencyMatrix*>(content))
		isTransformed = GraphTrasformer::transferContent(adjMatrixRepr, edgeListRepr);
	else if (AdjacencyList* adjListRepr = dynamic_cast<AdjacencyList*>(content))
		isTransformed = GraphTrasformer::transferContent(adjListRepr, edgeListRepr);
	if (isTransformed) {
		delete content;
		content = edgeListRepr;
	}
}

void Graph::writeGraph(const string fileName) {
	ofstream outFile;
	outFile.open(fileName);
	content->write(outFile);
	outFile.close();
}

Graph Graph::getSpaingTreePrima() {
	Graph result(content->vertexCount);
	if (content->isDirected || !content->isWeighted) 
		return result; // бессмысленная операция
	result.content = content->getSpaingTreePrima();
}
