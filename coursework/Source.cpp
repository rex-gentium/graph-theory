#include "Graph.h"
#include <iostream>
#include <fstream>
int main() {
	Graph graph;

	graph.readGraph("D:\\test_graph.txt");

	std::vector<char> marks;
	int isBipart = graph.checkBipart(marks);
	
	//graph.writeGraph("D:\\output_graph.txt");
	
	return 0;
}