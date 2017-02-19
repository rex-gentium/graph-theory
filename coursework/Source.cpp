#include "Graph.h"

int main() {
	Graph graph;

	graph.readGraph("D:\\test_graph.txt");
	
	Graph spaingTree = graph.getSpaingTreePrima();
	
	spaingTree.writeGraph("D:\\output_graph.txt");
	
	return 0;
}