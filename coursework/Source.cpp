#include "Graph.h"
#include <iostream>
#include <fstream>
int main() {
	Graph graph;

	graph.readGraph("D:\\test_graph.txt");

	Graph spaingTree = graph.getSpaingTreePrima();
	spaingTree.writeGraph("D:\\output_graph.txt");

	spaingTree = graph.getSpaingTreeKruscal();
	spaingTree.writeGraph("D:\\output_graph.txt");

	spaingTree = graph.getSpaingTreeBoruvka();
	spaingTree.writeGraph("D:\\output_graph.txt");
	
	return 0;
}