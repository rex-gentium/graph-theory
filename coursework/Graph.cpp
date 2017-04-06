﻿#include "Graph.h"
#include "AdjacencyMatrix.h"
#include "AdjacencyList.h"
#include "EdgeList.h"
#include "GraphTrasformer.h"
#include "Algorithm.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>

Graph::Graph() {}

Graph::Graph(int vertexCount) {
	content = new AdjacencyMatrix(vertexCount);
	content->isDirected = false;
	content->isWeighted = true;
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
			currentRepr = ADJMATRIX;
			break;
		case 'L' : 
			content = new AdjacencyList();
			currentRepr = ADJLIST;
			break;
		case 'E' : 
			content = new EdgeList();
			currentRepr = EDGELIST;
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
	
	switch (currentRepr) {
	case ADJMATRIX: 
		isTransformed = GraphTrasformer::transferContent(dynamic_cast<AdjacencyMatrix*>(content), adjListRepr);
	case EDGELIST: 
		isTransformed = GraphTrasformer::transferContent(dynamic_cast<EdgeList*>(content), adjListRepr);
	}

	if (isTransformed) {
		delete content;
		content = adjListRepr;
		currentRepr = ADJLIST;
	}
}

void Graph::transformToAdjMatrix() {
	AdjacencyMatrix * adjMatrixRepr = new AdjacencyMatrix();
	bool isTransformed = false;
	
	switch (currentRepr) {
	case ADJLIST:
		isTransformed = GraphTrasformer::transferContent(dynamic_cast<AdjacencyList*>(content), adjMatrixRepr);
	case EDGELIST:
		isTransformed = GraphTrasformer::transferContent(dynamic_cast<EdgeList*>(content), adjMatrixRepr);
	}
	
	if (isTransformed) {
		delete content;
		content = adjMatrixRepr;
		currentRepr = ADJMATRIX;
	}
}

void Graph::transformToListOfEdges() {
	EdgeList * edgeListRepr = new EdgeList();
	bool isTransformed = false;

	switch (currentRepr) {
	case ADJMATRIX:
		isTransformed = GraphTrasformer::transferContent(dynamic_cast<AdjacencyMatrix*>(content), edgeListRepr);
	case ADJLIST:
		isTransformed = GraphTrasformer::transferContent(dynamic_cast<AdjacencyList*>(content), edgeListRepr);
	}

	if (isTransformed) {
		delete content;
		content = edgeListRepr;
		currentRepr = EDGELIST;
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
	result.content = Algorithm::getSpaingTreePrima(this->content);
	return result;
}

Graph Graph::getSpaingTreeKruscal()
{
	Graph result(content->vertexCount);
	if (content->isDirected || !content->isWeighted)
		return result; // бессмысленная операция
	result.content = Algorithm::getSpaingTreeKruscal(this->content);
	return result;
}

Graph Graph::getSpaingTreeBoruvka()
{
	Graph result(content->vertexCount);
	if (content->isDirected || !content->isWeighted)
		return result; // бессмысленная операция
	result.content = Algorithm::getSpaingTreeBoruvka(this->content);
	return result;
}

int Graph::checkEuler(bool & circleExist)
{
	int result;
	return (Algorithm::checkEuler(this->content, circleExist, result)) ? result + 1 : 0;
}

vector<int> Graph::getEuleranTourFleri()
{
	vector<int> result;
	switch (currentRepr) {
	case ADJMATRIX:
		result = Algorithm::getEuleranTourFleri(dynamic_cast<AdjacencyMatrix*>(content)); break;
	case ADJLIST:
		result = Algorithm::getEuleranTourFleri(dynamic_cast<AdjacencyList*>(content)); break;
	case EDGELIST:
		result = Algorithm::getEuleranTourFleri(dynamic_cast<EdgeList*>(content)); break;
	}
	for (int i = 0; i < result.size(); ++i)
		++result[i];
	return result;
}
