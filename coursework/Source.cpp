#include "Graph.h"
#include <iostream>
#include <fstream>
int main() {
	Graph graph;

	graph.readGraph("D:\\test_graph.txt");

	auto route = graph.getMaximumMatchingBipart();
	
	//graph.writeGraph("D:\\output_graph.txt");
	
	return 0;
}