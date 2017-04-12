#include "Graph.h"
#include "AdjacencyMatrix.h"
#include "AdjacencyList.h"
#include "EdgeList.h"
#include "GraphTrasformer.h"
#include "Algorithm.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>

Graph::Graph() {
	content = nullptr;
	currentRepr = NONE;
}

/*Graph::Graph(int vertexCount) {
	content = new AdjacencyMatrix(vertexCount);
	content->isDirected = false;
	content->isWeighted = true;
}*/

Graph::~Graph() {
	if (content != nullptr)
		delete content;
}

Graph & Graph::operator=(const Graph & rhs)
{
	// assignment operator always creates a copy, it's a c++ contract, we can't avoid creating another graph here
	currentRepr = rhs.currentRepr;
	if (content == rhs.content)
		return *this;
	if (content != nullptr)
		delete content;
	switch (rhs.currentRepr) {
	case ADJMATRIX: content = new AdjacencyMatrix(*dynamic_cast<AdjacencyMatrix*>(rhs.content)); break;
	case ADJLIST: content = new AdjacencyList(*dynamic_cast<AdjacencyList*>(rhs.content)); break;
	case EDGELIST: content = new EdgeList(*dynamic_cast<EdgeList*>(rhs.content)); break;
	}
	return *this;
}

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
		isTransformed = GraphTrasformer::transferContent(dynamic_cast<AdjacencyMatrix*>(content), adjListRepr); break;
	case EDGELIST: 
		isTransformed = GraphTrasformer::transferContent(dynamic_cast<EdgeList*>(content), adjListRepr); break;
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
		isTransformed = GraphTrasformer::transferContent(dynamic_cast<AdjacencyList*>(content), adjMatrixRepr); break;
	case EDGELIST:
		isTransformed = GraphTrasformer::transferContent(dynamic_cast<EdgeList*>(content), adjMatrixRepr); break;
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
		isTransformed = GraphTrasformer::transferContent(dynamic_cast<AdjacencyMatrix*>(content), edgeListRepr); break;
	case ADJLIST:
		isTransformed = GraphTrasformer::transferContent(dynamic_cast<AdjacencyList*>(content), edgeListRepr); break;
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
	Graph * result = new Graph();
	if (content->isDirected || !content->isWeighted) 
		return *result; // бессмысленная операция
	result->content = Algorithm::getSpaingTreePrima(this->content);
	if (dynamic_cast<AdjacencyMatrix*>(result->content))
		result->currentRepr = ADJMATRIX;
	else if (dynamic_cast<AdjacencyList*>(result->content))
		result->currentRepr = ADJLIST;
	else if (dynamic_cast<EdgeList*>(result->content))
		result->currentRepr = EDGELIST;
	return *result;
}

Graph Graph::getSpaingTreeKruscal()
{
	Graph * result = new Graph();
	if (content->isDirected || !content->isWeighted)
		return *result; // бессмысленная операция
	RepresentationType repr = this->currentRepr;
	transformToListOfEdges();
	result->content = Algorithm::getSpaingTreeKruscal(dynamic_cast<EdgeList *>(content));
	if (dynamic_cast<AdjacencyMatrix*>(result->content))
		result->currentRepr = ADJMATRIX;
	else if (dynamic_cast<AdjacencyList*>(result->content))
		result->currentRepr = ADJLIST;
	else if (dynamic_cast<EdgeList*>(result->content))
		result->currentRepr = EDGELIST;
	// back transform
	switch (repr) {
	case ADJMATRIX: transformToAdjMatrix(); break;
	case ADJLIST: transformToAdjList();  break;
	}
	return *result;
}

Graph Graph::getSpaingTreeBoruvka()
{
	Graph * result = new Graph();
	if (content->isDirected || !content->isWeighted)
		return *result; // бессмысленная операция
	result->content = Algorithm::getSpaingTreeBoruvka(this->content);
	if (dynamic_cast<AdjacencyMatrix*>(result->content))
		result->currentRepr = ADJMATRIX;
	else if (dynamic_cast<AdjacencyList*>(result->content))
		result->currentRepr = ADJLIST;
	else if (dynamic_cast<EdgeList*>(result->content))
		result->currentRepr = EDGELIST;
	return *result;
}
