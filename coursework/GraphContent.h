#pragma once
#include <iostream>

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
};

